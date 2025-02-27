import sys
import os

handsclapping_lib_dirs = [
    '../../lib',  # for releases
    '../../build/lib', # for developing
    '../../build/lib/Release', # for Release developing
    '../../build/lib/Debug' # for Debug developing
]

# Add each directory to the system path
for lib_dir in handsclapping_lib_dirs:
    if os.path.exists(lib_dir):
        sys.path.append(lib_dir)

import time
import socket
import threading
import queue
import handsclapping as hc

def send_global_message(client_socket, message):
    client_socket.send(message.encode('utf-8'))

def get_player_action(action_name, client_socket, name, waiting_time):
    try:
        if waiting_time > 0:
            client_socket.settimeout(waiting_time)
        else:
            client_socket.settimeout(None)
        
        client_socket.send(name.encode('utf-8') + ", please enter your action:\n".encode('utf-8'))
        client_socket.send("ACTION\n".encode('utf-8'))
        action = client_socket.recv(1024).decode('utf-8')
        action_name.put(action)
        client_socket.send(f"Your action: {action}, is committed.\n".encode('utf-8'))
        print(f"[client] {name} action received: {action}")
    except socket.timeout:
        print(f"[client] {name} did not respond in time. Set action to NONE and remove from battle field.")
        client_socket.send("Timeout: No action received.\n".encode('utf-8'))
        client_socket.send("You are removed from the battle field.\n".encode('utf-8'))
        action_name.put("TIMEOUT")
    finally:
        client_socket.settimeout(None)

def game_core(registered_clients, waiting_time):
    hc.InitActions()

    player_names = []
    for player in registered_clients:
        player_names.append(player[1])

    battle_field = hc.BattleField(player_names)

    while battle_field.GetMemberNum() > 1:
        battle_field.TurnUpdate()
        print("Round", battle_field.GetTurn())

        global_message = battle_field.GetBattleFieldMessage(2)
        send_global_message_thread = [None] * battle_field.GetMemberNum()
        for i in range(battle_field.GetMemberNum()):
            send_global_message_thread[i] = threading.Thread(target=send_global_message, args=(registered_clients[i][0], global_message))
            send_global_message_thread[i].start()
        battle_field.PrintBattleField(2)
        for i in range(battle_field.GetMemberNum()):
            send_global_message_thread[i].join()
            print(f"[client] Global message received: {registered_clients[i][1]}")

        print("Collecting actions...")
        player_actions = [""] * battle_field.GetMemberNum()
        get_player_action_thread = [None] * battle_field.GetMemberNum()
        action_name_queue = [queue.Queue() for _ in range(battle_field.GetMemberNum())]
        for i in range(battle_field.GetMemberNum()):
            player_name = battle_field.GetPlayerName(i)
            if player_name != registered_clients[i][1]:
                print(f"Error: player name mismatch in name {player_name} and {registered_clients[i][1]}")
                return
            get_player_action_thread[i] = threading.Thread(target=get_player_action, args=(action_name_queue[i], registered_clients[i][0], player_name, waiting_time))
            get_player_action_thread[i].start()
        for i in range(battle_field.GetMemberNum()):
            get_player_action_thread[i].join()
            player_actions[i] = action_name_queue[i].get()

        battle_field.ActionUpdate(player_actions, 0)

        global_message = battle_field.GetBattleFieldMessage(0)
        send_global_message_thread = [None] * battle_field.GetMemberNum()
        for i in range(battle_field.GetMemberNum()):
            send_global_message_thread[i] = threading.Thread(target=send_global_message, args=(registered_clients[i][0], global_message))
            send_global_message_thread[i].start()
        battle_field.PrintBattleField(1)
        for i in range(battle_field.GetMemberNum()):
            send_global_message_thread[i].join()
            print(f"[client] Global message received: {registered_clients[i][1]}")

        battle_field.PositionUpdate()
        battle_field.EnergyUpdate(0)
        battle_field.HealthUpdate(0)
        battle_field.MemberNumUpdate()

        dead_players = []
        for player in registered_clients:
            name = player[1]
            IsDie = True
            for i in range(battle_field.GetMemberNum()):
                if battle_field.GetPlayerName(i) == name:
                    IsDie = False
                    break
            if IsDie:
                player[0].send("You are dead!\n".encode('utf-8'))
                player[0].send("END\n".encode('utf-8'))
                dead_players.append(player)
        for player in dead_players:
            registered_clients.remove(player)

    if battle_field.GetMemberNum() == 1:
        print("Game over! Winner is:", battle_field.GetPlayerName(0))
        registered_clients[0][0].send(f"Game over! You are the winner!\n".encode('utf-8'))
        registered_clients[0][0].send("END\n".encode('utf-8'))
    else:
        print("Game over! No winner!")