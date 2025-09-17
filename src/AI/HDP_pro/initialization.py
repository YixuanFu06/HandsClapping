import sys
import os
import numpy as np
import torch
from tqdm import tqdm
import yaml

handsclapping_lib_dirs = [
    '../../../lib',  # for releases
    '../../../build/lib', # for developing
    '../../../build/lib/Release', # for Release developing
    '../../../build/lib/Debug' # for Debug developing
]

# Add each directory to the system path
for lib_dir in handsclapping_lib_dirs:
    if os.path.exists(lib_dir):
        sys.path.append(lib_dir)

import handsclapping as hc # pylint: disable=import-error
hc.InitActions()

with open('config.yaml', 'r') as f:
    config = yaml.safe_load(f)

MAX_HEALTH = config['max_health']
MAX_ENERGY = config['max_energy']
ACTION_NUM = config['action_num']
STATE_NUM = (MAX_HEALTH + 1) * (MAX_HEALTH + 1) * (MAX_ENERGY + 1) * (MAX_ENERGY + 1)

SAVE_DIR = config['initialization_dir']

def state_reshape(opp_health, my_health, opp_energy, my_energy):
    """
    将状态从 (my_health, opp_health, my_energy, opp_energy) 转换为线性索引。
    """
    return (opp_health * (MAX_HEALTH + 1) + my_health) * (MAX_ENERGY + 1) * (MAX_ENERGY + 1) + \
           (opp_energy * (MAX_ENERGY + 1) + my_energy)

def calculate_transitions_for_state(state_params):
    """
    为单个初始状态计算所有动作组合的转移结果。
    """
    opp_health, my_health, opp_energy, my_energy = state_params
    
    # 如果任一方生命值为0，直接返回固定的转移结果
    if my_health == 0 or opp_health == 0:
        current_state_idx = state_reshape(opp_health, my_health, opp_energy, my_energy)
        # 返回一个填满当前状态索引的数组
        return np.full((ACTION_NUM, ACTION_NUM), current_state_idx, dtype=np.uint32)

    # 创建一个数组来存储这个状态的所有转移结果
    transitions = np.zeros((ACTION_NUM, ACTION_NUM), dtype=np.uint32)
    
    # 优化：只创建一个 BattleField 对象
    battle_field = hc.BattleField()
    
    for opp_action in range(ACTION_NUM):
        for my_action in range(ACTION_NUM):
            battle_field.AddPlayer('me', my_health, my_energy)
            battle_field.AddPlayer('opponent', opp_health, opp_energy)
            
            # 执行游戏逻辑
            battle_field.ActionUpdate([my_action, opp_action])
            battle_field.PositionUpdate()
            battle_field.EnergyUpdate()
            battle_field.HealthUpdate()

            # 获取转移后的状态
            new_opp_health = battle_field.GetPlayerHealth(battle_field.GetPlayerId('opponent'))
            new_my_health = battle_field.GetPlayerHealth(battle_field.GetPlayerId('me'))
            new_opp_energy = battle_field.GetPlayerEnergy(battle_field.GetPlayerId('opponent'))
            new_my_energy = battle_field.GetPlayerEnergy(battle_field.GetPlayerId('me'))

            # 判断暴死
            if new_opp_energy < 0 or new_my_energy < 0:
                new_opp_health = 0 if new_opp_energy < 0 else opp_health
                new_my_health = 0 if new_my_energy < 0 else my_health

            else:
                new_opp_health = min(MAX_HEALTH, max(0, new_opp_health))
                new_my_health = min(MAX_HEALTH, max(0, new_my_health))

            new_opp_energy = min(MAX_ENERGY, max(0, new_opp_energy))
            new_my_energy = min(MAX_ENERGY, max(0, new_my_energy))

            transitions[opp_action, my_action] = state_reshape(
                new_opp_health, new_my_health, new_opp_energy, new_my_energy
            )

            battle_field.RemovePlayer('me')
            battle_field.RemovePlayer('opponent')

    return transitions

def generate_transition_function():
    '''
    生成状态转移函数（并行版本）。
    '''

    # 创建所有状态参数的列表
    state_params_list = [
        (oh, mh, oe, me)
        for oh in range(MAX_HEALTH + 1)
        for mh in range(MAX_HEALTH + 1)
        for oe in range(MAX_ENERGY + 1)
        for me in range(MAX_ENERGY + 1)
    ]

    print("--- Starting parallel generation of transition function ---")
    
    results = []
    for params in tqdm(state_params_list):
        results.append(calculate_transitions_for_state(params))

    # 将结果组合成最终的 transition_function 数组
    # results 是一个列表，每个元素是一个 (36, 36) 的数组
    transition_function = np.array(results, dtype=np.uint32)
    
    # 将 transition_function 转换为 torch 张量
    # 最终形状已经是 (STATE_NUM, ACTION_NUM, ACTION_NUM)
    transition_function = torch.from_numpy(transition_function)
    torch.save(transition_function, os.path.join(SAVE_DIR, 'transition_function.pth'))

    print(f"Shape of transition_function: {transition_function.shape}")
    print("--- Parallel generation finished ---")

def generate_initial_winning_rate():
    """
    胜率初始化计算为 wr = my_health * my_energy / (my_health * my_energy + opp_health * opp_energy)
    """

    print("--- Starting generation of initial winning rate ---")

    # 1. 创建代表每个维度的一维向量
    # 注意：根据你的循环顺序，维度是 (opp_health, my_health, opp_energy, my_energy)
    opp_h_range = np.arange(MAX_HEALTH + 1, dtype=np.float32)
    my_h_range = np.arange(MAX_HEALTH + 1, dtype=np.float32)
    opp_e_range = np.arange(MAX_ENERGY + 1, dtype=np.float32) + 1
    my_e_range = np.arange(MAX_ENERGY + 1, dtype=np.float32) + 1

    # 2. 使用广播机制计算每个组合的乘积
    # 通过 np.newaxis (或 None) 扩展维度，使它们能够广播到 (6, 6, 16, 16) 的形状
    # opp_h_range[:, None, None, None] -> shape (6, 1, 1, 1)
    # my_h_range[None, :, None, None] -> shape (1, 6, 1, 1)
    # ...以此类推
    my_product = my_h_range[None, :, None, None] * my_e_range[None, None, None, :]
    opp_product = opp_h_range[:, None, None, None] * opp_e_range[None, None, :, None]

    # 3. 直接在整个广播后的数组上计算胜率
    # 为了防止除以零，在分母上加上一个很小的数
    denominator = my_product + opp_product + 1e-9
    init_wr = my_product / denominator
    
    # 4. 将最终的 numpy 数组转换为 torch 张量并保存
    init_wr = init_wr.reshape(STATE_NUM)
    init_wr_tensor = torch.from_numpy(init_wr)
    torch.save(init_wr_tensor, os.path.join(SAVE_DIR, 'init_winning_rate.pth'))

    print(f"Shape of initial winning rate: {init_wr_tensor.shape}")
    print("--- Initial winning rate generation finished ---")

def generate_initial_policy():
    """
    生成初始策略，所有状态下的动作均匀分布。
    """

    print("--- Starting generation of initial policy ---")

    init_policy = torch.zeros(STATE_NUM, ACTION_NUM) # softmax 后为均匀分布
    torch.save(init_policy, os.path.join(SAVE_DIR, 'init_policy.pth'))

    print(f"Shape of initial policy: {init_policy.shape}")
    print("--- Initial policy generation finished ---")


def generate_exhausting_mask():
    """
    生成耗尽掩码，表示在每个状态下哪些动作是不可行的。
    """

    print("--- Starting generation of exhausting mask ---")

    battle_field = hc.BattleField()

    # Create a tensor of action costs
    action_costs = torch.tensor([battle_field.GetActionEnergy(a) for a in range(ACTION_NUM)], dtype=torch.float32)
    
    # Create a tensor representing the 'me' (my energy) dimension
    me_values = torch.arange(MAX_ENERGY + 1, dtype=torch.float32)
    
    # Create a mask where my energy is less than the action cost
    # me_values shape: [MAX_ENERGY + 1, 1]
    # action_costs shape: [1, ACTION_NUM]
    # mask shape by broadcasting: [MAX_ENERGY + 1, ACTION_NUM]
    mask = me_values.unsqueeze(1) < action_costs.unsqueeze(0)

    torch.save(mask, os.path.join(SAVE_DIR, 'energy_exhausting_mask.pth'))

    print(f"Shape of energy exhausting mask: {mask.shape}")
    print("--- Energy exhausting mask generation finished ---")
    
    # Apply the mask to the policy tensor.
    # The mask is broadcasted across the first 3 dimensions of the policy tensor.
    # policy[:, :, :, mask] = -torch.inf


if __name__ == "__main__":

    os.makedirs(SAVE_DIR, exist_ok=True)

    generate_transition_function()

    generate_initial_winning_rate()

    generate_initial_policy()

    generate_exhausting_mask()
