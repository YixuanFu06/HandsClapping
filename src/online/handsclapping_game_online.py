import sys
import os

handsclapping_lib_dir = '../../build/lib/Release' # use '../../build/lib/release' instead in windows

script_dir = os.path.dirname(os.path.abspath(__file__))

sys.path.append(os.path.join(script_dir, handsclapping_lib_dir))

import handsclapping as hc

hc.InitActions()

player_names = []
print('Enter player names (empty line to finish):')
while True:
    name = input()
    if name == '':
        break
    player_names.append(name)

battle_field = hc.BattleField(player_names)

while battle_field.GetMemberNum() > 1:
    battle_field.TurnUpdate()
    battle_field.PrintBattleField(2)
    player_actions = []
    for i in range(battle_field.GetMemberNum()):
        print(battle_field.GetPlayerName(i), end='')
        print("'s action: ", end='')
        action = input()
        player_actions.append(action)
    battle_field.BattleFieldUpdate(player_actions)

if battle_field.GetMemberNum() == 1:
    print("Game over! Winner is:", battle_field.GetPlayerName(0))
else:
    print("Game over! No winner!")