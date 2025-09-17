import torch
from metrics import Exploitability
import yaml
import os

with open('config.yaml', 'r') as f:
    config = yaml.safe_load(f)

MAX_HEALTH = config['max_health']
MAX_ENERGY = config['max_energy']
ACTION_NUM = config['action_num']
STATE_NUM = (MAX_HEALTH + 1) * (MAX_HEALTH + 1) * (MAX_ENERGY + 1) * (MAX_ENERGY + 1)

initialization_path = config['initialization_dir']
if not os.path.exists(initialization_path):
    raise FileNotFoundError(f"Initialization directory '{initialization_path}' does not exist.")
    
visualization_path = config['visualization_dir']

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"Using device: {device}")

model_name = 'best_hdp_model_8000_0.8batch_50iter_0.03'

if __name__ == "__main__":

    # 从data目录读取初始化数据
    transition_function = torch.load(os.path.join(initialization_path, 'transition_function.pth'))
    if transition_function.shape != (STATE_NUM, ACTION_NUM, ACTION_NUM):
        raise ValueError(f"Transition function shape mismatch: expected {(STATE_NUM, ACTION_NUM, ACTION_NUM)}, got {transition_function.shape}")
    init_wr = torch.load(os.path.join(initialization_path, 'init_winning_rate.pth'))
    if init_wr.shape != (STATE_NUM,):
        raise ValueError(f"Initial winning rate shape mismatch: expected {(STATE_NUM,)}, got {init_wr.shape}")
    print("Initialization data loaded successfully.")

    exploitability_calculator = Exploitability(MAX_HEALTH, MAX_ENERGY, ACTION_NUM, transition_function, init_wr, iter_num=100).to(device)

    model_state_dict = torch.load(f'model/{model_name}.pth', map_location=device)
    policy = model_state_dict['policy']
    wr = model_state_dict['wr']

    exploitability_upper_bound, exploitability_lower_bound, _, _, _ = exploitability_calculator(policy, iter_num=100)

    print(f"Exploitability: [{exploitability_upper_bound.item():.4f}, {exploitability_lower_bound.item():.4f}]")


    policy_dist = torch.softmax(policy, dim=1).view(
        MAX_HEALTH + 1,
        MAX_HEALTH + 1,
        MAX_ENERGY + 1,
        MAX_ENERGY + 1,
        ACTION_NUM
    )

    wr = wr.view(
        MAX_HEALTH + 1,
        MAX_HEALTH + 1,
        MAX_ENERGY + 1,
        MAX_ENERGY + 1
    )

    # 使用 with open 将输出写入文件
    with open(f'{visualization_path}/{model_name}_policy_visualization.txt', 'w', encoding='utf-8') as f:
        for opp_health in range(1, MAX_HEALTH + 1):
            for my_health in range(1, MAX_HEALTH + 1):
                for opp_energy in range(0, MAX_ENERGY + 1):
                    for my_energy in range(0, MAX_ENERGY + 1):
                        # 写入状态头信息
                        f.write(f"State (opp_h={opp_health}, my_h={my_health}, opp_e={opp_energy}, my_e={my_energy}):\n")
                        for my_action in range(38):
                            # 使用 f.write 替代 print
                            f.write(f"{policy_dist[opp_health, my_health, opp_energy, my_energy, my_action]:.4f} ")
                        f.write("\n")
                    f.write("\n")
                f.write("\n")
            f.write("\n")
        f.write("\n")

    print(f"Policy visualization has been written to {visualization_path}/{model_name}_policy_visualization.txt")

    with open(f'{visualization_path}/{model_name}_wr_visualization.txt', 'w', encoding='utf-8') as f:
        for opp_health in range(1, MAX_HEALTH + 1):
            for my_health in range(1, MAX_HEALTH + 1):
                for opp_energy in range(0, MAX_ENERGY + 1):
                    for my_energy in range(0, MAX_ENERGY + 1):
                        f.write(f"State (opp_h={opp_health}, my_h={my_health}, opp_e={opp_energy}, my_e={my_energy}): ")
                        f.write(f"{wr[opp_health, my_health, opp_energy, my_energy]:.4f}\n")
                    f.write("\n")
                f.write("\n")
            f.write("\n")
        f.write("\n")

    print(f"Winning rate visualization has been written to {visualization_path}/{model_name}_wr_visualization.txt")
