import sys
import os

# 自定义库目录名称
handsclapping_lib_dir = '../../build/lib'

# 获取脚本所在目录
script_dir = os.path.dirname(os.path.abspath(__file__))

# 动态生成路径
sys.path.append(os.path.join(script_dir, handsclapping_lib_dir))

import handsclapping as hc

hc.InitActions()
player_names = ['Alice', 'Bob', 'Charlie', 'David']
battlefield = hc.BattleField(player_names)

for i in range(4):
    print(battlefield.GetPlayerName(i)) # should print player_names[i]

print(battlefield.GetMemberNum())

battlefield.PrintBattleField(1)