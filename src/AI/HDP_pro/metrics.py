import torch
import torch.nn as nn

class Exploitability(nn.Module):
    """
    计算一个策略的可利用性（Exploitability）。

    这个模块会计算一个固定的输入策略（policy）的最佳应对策略（Best Response），
    并基于最佳应对策略的胜率来评估原始策略的可利用性。
    一个完美（纳什均衡）的策略，其可利用性为0。
    """
    def __init__(self, max_health, max_energy, action_num, transition_function, init_wr, mask_std=0.8, iter_num=50):
        """
        初始化计算器。
        transition_function: 游戏的状态转移函数。
        iter_num: 用于计算最佳应对胜率的价值迭代次数。
        """
        super().__init__()
        self.max_health = max_health
        self.max_energy = max_energy
        self.action_num = action_num
        self.state_num = (max_health + 1) * (max_health + 1) * (max_energy + 1) * (max_energy + 1)
        self.mask_std = mask_std
        self.iter_num = iter_num

        # 使用 register_buffer，这样当模块被移动到不同设备时，F也会被移动
        self.register_buffer('F', transition_function.long())
        self.register_buffer('wr', init_wr)


    def _transpose(self, policy):
        '''
        policy: input standard policy, shape (state_num, action_num)
        return: transposed policy on the state dimension
        This function swaps the perspective of 'me' and 'opponent' in the state representation.
        '''

        # 状态被假定为从 (my_health, opponent_health, my_energy, opponent_energy) 展平而来
        transposed_policy = policy.view(
            self.max_health + 1,
            self.max_health + 1,
            self.max_energy + 1,
            self.max_energy + 1,
            self.action_num
        )

        # (my_health, opp_health, my_energy, opp_energy, action) -> (opp_health, my_health, opp_energy, my_energy, action)
        # 这对应于交换维度 0 和 1，以及维度 2 和 3。
        transposed_policy = transposed_policy.permute(1, 0, 3, 2, 4)
        transposed_policy = transposed_policy.contiguous().view(self.state_num, self.action_num)

        return transposed_policy

    def _create_mask(self, wr):
        """
        根据给定的概率，在与wr相同的设备上创建一个随机掩码。
        """
        if self.mask_std is None or self.mask_std >= 1.0:
            # ones_like 会自动在与 wr 相同的设备上创建张量
            return torch.ones_like(wr)
        
        # rand_like 会自动在与 wr 相同的设备上创建张量
        random_tensor = torch.rand_like(wr)
        mask = (random_tensor < self.mask_std).float()
        return mask

    def forward(self, policy, iter_num=None):
        """
        计算给定策略的可利用性。
        policy: 需要被评估的策略，大小为 (state_num, action_num)。
        返回: 一个标量，代表策略的可利用性。以及达到该可利用性的最佳应对策略。
        """

        if iter_num is None:
            iter_num = self.iter_num

        policy_dist = torch.softmax(policy, dim=1)
        transposed_policy_dist = self._transpose(policy_dist)

        temp_wr = self.wr.clone()

        # 价值迭代 - 完全可微分
        for _ in range(iter_num):
            # temp(i, k): 在状态i，我方出招k，对手按其策略出招，我方的期望胜率
            # temp = E_{j~opponent_policy}[ E_{l~F}[ temp_wr[l] ] ]
            # temp = torch.einsum('ij,l,lijk->ik', opponent_policy, temp_wr, self.F)
            temp = torch.einsum('ij,ijk->ik', transposed_policy_dist, temp_wr[self.F])
            # Bellman 最优更新：在每个状态i，选择能最大化期望胜率的出招k
            temp_wr = torch.max(temp, dim=1).values

        temp_wr = temp_wr.view(
            self.max_health + 1,
            self.max_health + 1,
            self.max_energy + 1,
            self.max_energy + 1
        )

        exploitability_upper_bound = 1 - (1 - temp_wr[1, 1, 0, 0]) / (temp_wr[1, 1, 0, 0] + 1e-9)

        mask = self._create_mask(temp_wr)
        loss = torch.sum(mask * temp_wr)

        with torch.no_grad():
            # 使用 argmax 获得最优策略，但使用 detach() 阻止梯度流经 opt_policy
            best_actions = torch.argmax(temp, dim=1)
            opt_policy_dist = torch.zeros_like(policy)
            opt_policy_dist[torch.arange(policy.shape[0]), best_actions] = 1

            wr = self.wr.clone()
            
            for _ in range(iter_num):
                wr = torch.einsum('ij,ijk,ik->i', transposed_policy_dist, wr[self.F], opt_policy_dist)

            wr = wr.view(
                self.max_health + 1,
                self.max_health + 1,
                self.max_energy + 1,
                self.max_energy + 1
            )

            exploitability_lower_bound = 1 - (1 - wr[1, 1, 0, 0]) / (wr[1, 1, 0, 0] + 1e-9)

        return exploitability_upper_bound, exploitability_lower_bound, opt_policy_dist, temp_wr, loss
