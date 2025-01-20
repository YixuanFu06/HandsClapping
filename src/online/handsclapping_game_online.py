import sys
import os

handsclapping_lib_dir = '../../build/lib/Release' # use '../../build/lib/Release' instead in windows

script_dir = os.path.dirname(os.path.abspath(__file__))

sys.path.append(os.path.join(script_dir, handsclapping_lib_dir))

import time
import socket
import threading
import queue
import handsclapping as hc

def get_player_action(action_name, client_socket, name):
    client_socket.send(name.encode('utf-8') + ", please enter your action:\n".encode('utf-8'))
    client_socket.send("ACTION\n".encode('utf-8'))
    action = client_socket.recv(1024).decode('utf-8')
    action_name.put(action)
    client_socket.send(f"Your action: {action}, is committed.\n".encode('utf-8'))
    print(f"[client] {name} action received: {action}")

def game_core(registered_clients):
    hc.InitActions()

    player_names = []
    for player in registered_clients:
        player_names.append(player[1])

    battle_field = hc.BattleField(player_names)

    while battle_field.GetMemberNum() > 1:
        battle_field.TurnUpdate()
        print("Round", battle_field.GetTurn())

        battle_field.PrintBattleField(2)

        print("Collecting actions...")
        player_actions = [""] * battle_field.GetMemberNum()
        get_player_action_thread = [None] * battle_field.GetMemberNum()
        action_name_queue = [queue.Queue() for _ in range(battle_field.GetMemberNum())]
        for i in range(battle_field.GetMemberNum()):
            player_name = battle_field.GetPlayerName(i)
            if player_name != registered_clients[i][1]:
                print(f"Error: player name mismatch in name {player_name} and {registered_clients[i][1]}")
                return
            get_player_action_thread[i] = threading.Thread(target=get_player_action, args=(action_name_queue[i], registered_clients[i][0], player_name))
            get_player_action_thread[i].start()
        for i in range(battle_field.GetMemberNum()):
            get_player_action_thread[i].join()
            player_actions[i] = action_name_queue[i].get()


        battle_field.ActionUpdate(player_actions)
        battle_field.PrintBattleField(0)
        battle_field.PositionUpdate()
        battle_field.EnergyUpdate()
        battle_field.HealthUpdate()
        battle_field.MemberNumUpdate()

        for player in registered_clients:
            name = player[1]
            IsDie = True
            for i in range(battle_field.GetMemberNum()):
                if battle_field.GetPlayerName(i) == name:
                    player[0].send(f"Your health: {battle_field.GetPlayerHealth(i)}\n".encode('utf-8'))
                    player[0].send(f"Your energy: {battle_field.GetPlayerEnergy(i)}\n".encode('utf-8'))
                    IsDie = False
                    break
            if IsDie:
                player[0].send("You are dead!\n".encode('utf-8'))
                player[0].send("END\n".encode('utf-8'))
                registered_clients.remove(player)

    if battle_field.GetMemberNum() == 1:
        print("Game over! Winner is:", battle_field.GetPlayerName(0))
        registered_clients[0][0].send(f"Game over! You are the winner!\n".encode('utf-8'))
        registered_clients[0][0].send("END\n".encode('utf-8'))
    else:
        print("Game over! No winner!")