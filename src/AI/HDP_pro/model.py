import torch
import torch.nn as nn

class HDP(nn.Module):
    def __init__(self, max_health, max_energy, action_num, transition_function, init_policy, init_wr, exhausting_mask, wr_update_iter=5, wr_update_iter_adversarial=100):
        '''
        transition_function: the transition function of states and actions, size = state * state * action * action
        self.policy: the policy for HDP, size = state * action
        self.wr: the winning rate, size = state
        '''
        super().__init__()
        self.max_health = max_health
        self.max_energy = max_energy
        self.action_num = action_num
        self.state_num = (max_health + 1) * (max_health + 1) * (max_energy + 1) * (max_energy + 1)
        self.wr_update_iter = wr_update_iter
        self.wr_update_iter_adversarial = wr_update_iter_adversarial

        # 将 policy 定义为模型的可训练参数，并使用均匀值初始化
        self.policy = nn.Parameter(init_policy)
        
        # 将 transition function 和 wr 定义为模型的缓冲区，它会随模型移动(如.to(device))但不是可训练参数
        self.register_buffer('wr', init_wr)
        self.register_buffer('F', transition_function.long())
        self.register_buffer('exhausting_mask', exhausting_mask)


    @torch.no_grad()
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

    
    @torch.no_grad()
    def _get_transition_matrix(self, opponent_policy):
        """
        高效计算转移矩阵: sum_{j,k} transposed_opponent_policy_{ij} * policy_{ik} * F_{lijk}
        
        参数:
        - transposed_policy: shape (state_num, action_num)  
        - policy: shape (state_num, action_num)
        
        返回:
        - transition_matrix: shape (state_num, state_num)
        """
        
        opponent_policy = opponent_policy.view(
            self.max_health + 1,
            self.max_health + 1,
            self.max_energy + 1,
            self.max_energy + 1,
            self.action_num
        )
        transposed_opponent_policy = opponent_policy.permute(1, 0, 3, 2, 4)
        transposed_opponent_policy = transposed_opponent_policy.contiguous().view(self.state_num, self.action_num)
        
        # 计算权重矩阵
        weights = torch.einsum('ij,ik->ijk', transposed_opponent_policy, self.policy)  # (state_num, action_num, action_num)

        # 使用 scatter_add 进行累加
        # 我们需要将三维的 (i,j,k) 索引映射到二维的 (i,l) 
        state_indices = torch.arange(self.state_num, device=self.policy.device).view(-1, 1, 1).expand(-1, self.action_num, self.action_num)
        target_indices = self.F  # shape: (state_num, action_num, action_num)
        
        # 展平所有维度
        flat_state_indices = state_indices.reshape(-1)
        flat_target_indices = target_indices.reshape(-1)
        flat_weights = weights.reshape(-1)
        
        # 使用 scatter_add 累加
        linear_indices = flat_state_indices * self.state_num + flat_target_indices
        transition_matrix_flat = torch.zeros(self.state_num * self.state_num, device=self.policy.device, dtype=self.policy.dtype)
        transition_matrix_flat.scatter_add_(0, linear_indices, flat_weights)
        
        return transition_matrix_flat.view(self.state_num, self.state_num)


    @torch.no_grad()
    def wr_update(self, iter_num=None):
        '''
        使用模型内部的 self.policy 和 self.wr 进行计算。
        iter_num: 进行多少次 wr 更新迭代。如果为 None，则使用初始化时的迭代次数 self.wr_update_iter。
        '''

        if iter_num is None:
            iter_num = self.wr_update_iter

        policy_dist = torch.softmax(self.policy, dim=1)

        transposed_policy_dist = self._transpose(policy_dist)

        # # --- 方法一: 矩阵求幂 (通常更高效) ---
        # wr_transition_function = torch.einsum('ij,ik,lijk->il', transposed_policy_dist, policy_dist, self.F)
        # total_wr_transition_function = torch.matrix_power(wr_transition_function, iter_num)
        # updated_wr = torch.matmul(total_wr_transition_function, self.wr)
        # # --- 方法二: 循环迭代 (更灵活) ---
        # updated_wr = self.wr.clone()
        # for _ in range(self.iter_num):
        #     temp = torch.einsum('l,lijk->ijk', updated_wr, self.F)
        #     updated_wr = torch.einsum('ij,ik,ijk->i', transposed_policy_dist, policy_dist, temp)

        # However, the simplest four-dimensional transition matrix is not available due to memory limits
        updated_wr = self.wr.clone()
        for _ in range(iter_num):
            # temp = torch.einsum('l,lijk->ijk', updated_wr, self.F) # The original code
            temp = updated_wr[self.F]
            updated_wr = torch.einsum('ij,ik,ijk->i', transposed_policy_dist, policy_dist, temp)
            # updated_wr = torch.clamp(updated_wr, 0.0, 1.0) # Optional

        # 更新内部的 wr 缓冲区
        self.wr.copy_(updated_wr)


    @torch.no_grad()
    def polish_policy(self, iter_num=None):
        if iter_num is None:
            iter_num = self.wr_update_iter_adversarial

        self.policy.data = self.policy.view(self.max_health + 1, self.max_health + 1, self.max_energy + 1, self.max_energy + 1, self.action_num)
        # Apply the mask to the policy tensor.
        # The mask is broadcasted across the first 3 dimensions of the policy tensor.
        self.policy[:, :, :, self.exhausting_mask] = -torch.inf
        self.policy.data = self.policy.view(self.state_num, self.action_num)
        self.wr_update(iter_num=iter_num)

    
    @torch.no_grad()
    def advanced_polish_policy(self, iter_num=None):
        if iter_num is None:
            iter_num = self.wr_update_iter_adversarial

        wr_transition_function = self.wr[self.F]

        # 获取 wr_transition_function 的形状: [state_num, action_num, action_num]
        # 其中 wr_transition_function[s, opp_a, my_a] 表示在状态s，对手选择opp_a，我选择my_a时的胜率

        # 1. 扩展维度以便进行比较
        # wr_expanded: [state_num, action_num, action_num, 1] (my_action维度)
        # wr_compare: [state_num, action_num, 1, action_num] (my_action_to_be_dominated维度)
        wr_expanded = wr_transition_function.unsqueeze(-1)  # [state, opp_action, my_action, 1]
        wr_compare = wr_transition_function.unsqueeze(-2)   # [state, opp_action, 1, my_action_to_be_dominated]

        # 2. 计算支配关系
        # greater_equal: [state, opp_action, my_action, my_action_to_be_dominated]
        # greater_equal[s, opp_a, my_a, dominated_a] = True 如果 wr[s,opp_a,my_a] >= wr[s,opp_a,dominated_a]
        greater_equal = wr_expanded >= wr_compare

        # strictly_greater: [state, opp_action, my_action, my_action_to_be_dominated]
        # strictly_greater[s, opp_a, my_a, dominated_a] = True 如果 wr[s,opp_a,my_a] > wr[s,opp_a,dominated_a]
        strictly_greater = wr_expanded > wr_compare

        # 3. 检查支配条件
        # 对于每个 (state, my_action, my_action_to_be_dominated) 组合：
        # - 所有对手动作下，my_action 的胜率都 >= my_action_to_be_dominated 的胜率
        # - 至少一个对手动作下，my_action 的胜率 > my_action_to_be_dominated 的胜率

        # all_greater_equal: [state, my_action, my_action_to_be_dominated]
        # 在所有对手动作下，my_action >= my_action_to_be_dominated
        all_greater_equal = torch.all(greater_equal, dim=1)

        # any_strictly_greater: [state, my_action, my_action_to_be_dominated]
        # 在至少一个对手动作下，my_action > my_action_to_be_dominated
        any_strictly_greater = torch.any(strictly_greater, dim=1)

        # is_dominated: [state, my_action, my_action_to_be_dominated]
        # my_action 支配 my_action_to_be_dominated
        is_dominated = all_greater_equal & any_strictly_greater

        # 4. 避免自我支配 (my_action == my_action_to_be_dominated)
        action_indices = torch.arange(self.action_num, device=self.policy.device)
        not_self = action_indices.unsqueeze(0) != action_indices.unsqueeze(1)  # [my_action, my_action_to_be_dominated]
        not_self = not_self.unsqueeze(0).expand(self.state_num, -1, -1)  # [state, my_action, my_action_to_be_dominated]

        # 只考虑非自我支配的情况
        is_dominated = is_dominated & not_self

        # 5. 找到被支配的动作
        # dominated_actions: [state, my_action_to_be_dominated]
        # 如果某个动作被任何其他动作支配，则为True
        dominated_actions = torch.any(is_dominated, dim=1)

        # 应用掩码
        self.policy[dominated_actions] = -torch.inf

        self.wr_update(iter_num=iter_num)


    @torch.no_grad()
    def forward(self, opponent_policy, iter_num=None):
        '''
        使用模型内部的 self.policy 和 self.wr，传入的 opponent_policy 进行计算对战模式下的胜率。
        opponent_policy: (可选) 一个固定的对手策略，用于评估。如果为None，则进行自博弈。
        iter_num: 进行多少次 wr 更新迭代。如果为 None，则使用初始化时的迭代次数 self.wr_update_iter_adversarial。
        返回: 更新后的胜率。
        '''

        if iter_num is None:
            iter_num = self.wr_update_iter_adversarial
        
        policy_dist = torch.softmax(self.policy, dim=1)
        
        # 评估模式：对手是外部传入的固定策略
        opponent_policy_dist = torch.softmax(opponent_policy, dim=1)
        transposed_opponent_policy_dist = self._transpose(opponent_policy_dist)

        updated_wr = self.wr.clone()
        for _ in range(iter_num):
            temp = updated_wr[self.F]
            if decline_function is not None:
                temp = decline_function(temp)
            updated_wr = torch.einsum('ij,ik,ijk->i', transposed_opponent_policy_dist, policy_dist, temp)
            # updated_wr = torch.clamp(updated_wr, 0.0, 1.0)

        return updated_wr
