import sys
import os
import numpy as np
import torch
import time
import yaml

torch.manual_seed(int(time.time() * 1000) % (2**32))

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

model_dir = config['model_dir']

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model_name = 'best_hdp_model_10000_0.8batch_50iter_0.01'

if __name__ == "__main__":

    model_state_dict = torch.load(f'{model_dir}/{model_name}.pth', map_location=device)
    policy = model_state_dict['policy']
    wr = model_state_dict['wr']

    policy = torch.softmax(policy, dim=1)
    policy = policy.view(MAX_HEALTH + 1, MAX_HEALTH + 1, MAX_ENERGY + 1, MAX_ENERGY + 1, ACTION_NUM)

    battle_field = hc.BattleField(['You', 'HDP-pro'])

    while battle_field.GetMemberNum() > 1:
        battle_field.TurnUpdate()
        print("Round", battle_field.GetTurn())

        my_action = input("Enter your action: ")

        opp_health = int(battle_field.GetPlayerHealth(battle_field.GetPlayerId('HDP-pro')))
        my_health = int(battle_field.GetPlayerHealth(battle_field.GetPlayerId('You')))
        opp_energy = int(battle_field.GetPlayerEnergy(battle_field.GetPlayerId('HDP-pro')))
        my_energy = int(battle_field.GetPlayerEnergy(battle_field.GetPlayerId('You')))

        action_distribution = policy[my_health, opp_health, my_energy, opp_energy]
        # print(f"Action probabilities (top 5): {torch.topk(action_distribution, 5)}")
        action = torch.multinomial(action_distribution, num_samples=1).item()
        opp_action = battle_field.GetActionFormalName(action)

        battle_field.ActionUpdate([my_action, opp_action])
        battle_field.PrintBattleField(0)
        battle_field.RemoveDead()
        battle_field.PositionUpdate()
        battle_field.EnergyUpdate()
        battle_field.RemoveDead()
        battle_field.HealthUpdate()
        battle_field.RemoveDead()
        battle_field.MemberNumUpdate()
