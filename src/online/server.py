import time
import socket
import threading
import sys
import argparse

server_running = True
game_start = False
current_players = 0
confirmed_players = 0
registered_clients = []
registered_ip = [("127.0.0.1", 1)]
registered_clients_lock = threading.Lock()

def get_ip_address():
    try:
        # get the hostname
        hostname = socket.gethostname()
        # get the IP address
        ip_address = socket.gethostbyname(hostname)
    except Exception:
        ip_address = '127.0.0.1'
    return ip_address

def waiting_for_confirmation(client_socket, addr, name):
    global current_players
    global confirmed_players
    global registered_clients
    global registered_ip
    global max_players
    global max_ip_player
    global registered_clients_lock
    client_socket.send("Enter y/n to confirm to the game...\n".encode('utf-8'))
    client_socket.send("CONFIRM\n".encode('utf-8'))
    response = client_socket.recv(1024).decode('utf-8')
    if (response == 'n'):
        client_socket.send("You have refused to confirm to the game.\n".encode('utf-8'))
        client_socket.send("END\n".encode('utf-8'))
        print(f"[client] Player {name} refused to confirm.")
        current_players -= 1
        client_socket.close()
        return
    
    ip = addr[0]
    
    with registered_clients_lock:
        if len(registered_clients) >= max_players:
            client_socket.send("The game is full. Please try again later.\n".encode('utf-8'))
            client_socket.send("END\n".encode('utf-8'))
            print(f"[client] Player {name} is removed due to the full game.")
            current_players -= 1
            client_socket.close()
            return
        ip_is_registered = False
        for (player_socket, player_name, player_ip) in registered_clients:
            if player_name == name:
                client_socket.send("There is already a player with the same name ".encode('utf-8') + name.encode('utf-8') + ". Please log in with another name.\n".encode('utf-8'))
                client_socket.send("END\n".encode('utf-8'))
                print(f"[client] Player {name} is removed due to the same name with another player.")
                current_players -= 1
                client_socket.close()
                return
            if player_ip == ip:
                ip_is_registered = True
                for (client_ip, num) in registered_ip:
                    if client_ip == ip:
                        if num >= max_ip_player:
                            client_socket.send(f"There are too many players from the same IP address {ip}. Please try again later.\n".encode('utf-8'))
                            client_socket.send("END\n".encode('utf-8'))
                            print(f"[client] Player {name} is removed due to the same IP address with too many players.")
                            current_players -= 1
                            client_socket.close()
                            return
                        else:
                            num += 1
                            break
    if not ip_is_registered:
        registered_ip.append((ip, 1))
        print(f"{ip} is added to the registered IP list.")
    registered_clients.append((client_socket, name, ip))
    client_socket.send(name.encode('utf-8') + " Confirmed\n".encode('utf-8'))
    confirmed_players += 1
    print(f"[client] Player {name} confirmed.")

def player_detection():
    global server_running
    global game_start
    global max_players
    global current_players
    global confirmed_players
    previous_players = current_players
    previous_confirmed_players = confirmed_players
    while server_running and (confirmed_players < current_players or current_players < 2) and len(registered_clients) < max_players:
        if previous_players != current_players or previous_confirmed_players != confirmed_players:
            print(f"{current_players} players online. {confirmed_players} players confirmed...")
            previous_players = current_players
            previous_confirmed_players = confirmed_players
        time.sleep(0.1)
    print(f"{current_players} players online. {confirmed_players} players confirmed...")
    print("Game is starting...")
    game_start = True

def ClientRegistration(client_socket, addr):
    global server_running
    global game_start
    global max_players
    global current_players
    global confirmed_players

    current_players += 1
    print(f"[client] Connect from: {addr}")
    # receive the player's name
    name = client_socket.recv(1024).decode('utf-8')
    if not name:
        client_socket.close()
        return
    print(f"[client] Player registration: {name}")

    if len(name) > max_name_length:
        client_socket.send(f"The length of the player's name is too long. Please try again with a shorter name ({max_name_length} characters).\n".encode('utf-8'))
        client_socket.send("END\n".encode('utf-8'))
        print(f"[client] Player {name} is removed due to the long name.")
        current_players -= 1
        client_socket.close()
        return

    for char in name:
        if char == ' ' or char == '&' or char == '#' or char == '\n':
            client_socket.send("The player's name cannot contain space, enter, &, and #. Please try again with another name.\n".encode('utf-8'))
            client_socket.send("END\n".encode('utf-8'))
            print(f"[client] Player {name} is removed due to the space in the name.")
            current_players -= 1
            client_socket.close()
            return

    client_socket.send("Hello, ".encode('utf-8') + name.encode('utf-8') + "\n".encode('utf-8'))
    if waiting_time > 0:
        client_socket.send("Waiting time for each action is set to ".encode('utf-8') + str(waiting_time).encode('utf-8') + " seconds.\n".encode('utf-8'))
    else:
        client_socket.send("Waiting time for each action is not set in this game.\n".encode('utf-8'))
    client_socket.send("WAITING_TIME\n".encode('utf-8'))
    get_ready_message = client_socket.recv(1024).decode('utf-8')
    while get_ready_message != "GET_READY":
        get_ready_message = client_socket.recv(1024).decode('utf-8')
    client_socket.send(str(waiting_time).encode('utf-8') + "\n".encode('utf-8'))
    confirmation_thread = threading.Thread(target=waiting_for_confirmation, args=(client_socket, addr, name))
    confirmation_thread.start()

    previous_players = current_players
    previous_confirmed_players = confirmed_players
    client_socket.send((str(current_players) + " players online. " + str(confirmed_players) + " players confirmed...\n").encode('utf-8'))
    while server_running and (confirmed_players < current_players or current_players < 2):
        if previous_players != current_players or previous_confirmed_players != confirmed_players:
            client_socket.send((str(current_players) + " players online. " + str(confirmed_players) + " players confirmed...\n").encode('utf-8'))
            previous_players = current_players
            previous_confirmed_players = confirmed_players
        time.sleep(0.1)
    client_socket.send((str(current_players) + " players online. " + str(confirmed_players) + " players confirmed...\n").encode('utf-8'))
    client_socket.send("Game is starting...\n".encode('utf-8'))

def start_server():
    global server_running
    global game_start
    global max_players
    global current_players
    global registered_clients

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    server_socket.bind(('0.0.0.0', 38921))  # 0.0.0.0 means all available interfaces
    
    ip_address = get_ip_address()
    print(f"Server starts with IP: {ip_address}, port: 38921")
    
    server_socket.listen(5)
    print("Waiting to be connected...")

    try:
        player_detection_thread = threading.Thread(target=player_detection)
        player_detection_thread.start()
        while server_running and len(registered_clients) < max_players:
            try:
                client_socket, addr = server_socket.accept()
                if game_start:
                    client_socket.send("Game has already started.\n".encode('utf-8'))
                    client_socket.send("END\n".encode('utf-8'))
                    client_socket.close()
                    print("[client] Game has already started. Client's connection is refused.")
                    break
                client_thread = threading.Thread(target=ClientRegistration, args=(client_socket, addr))
                client_thread.start()
            except socket.timeout:
                if game_start:
                    print("Game has already started. Server entry is closed.")
                    break
                else:
                    print("No more players are joining the game. Server entry is closed.")
                    break
    except KeyboardInterrupt:
        print("Server has been forced to terminate...")
    finally:
        server_running = False
        server_socket.close()

default_max_players = 10
default_max_ip_player = 1
default_waiting_time = 0
default_max_name_length = 16

def parse_arguments():
    parser = argparse.ArgumentParser(description="Server configuration")
    parser.add_argument('-player', type=int, default=default_max_players, help='Maximum number of players')
    parser.add_argument('-ip', type=int, default=default_max_ip_player, help='Maximum number of players per IP')
    parser.add_argument('-wait', type=int, default=default_waiting_time, help='Waiting time in seconds')
    parser.add_argument('-name', type=int, default=default_max_name_length, help='Maximum length of the player name')
    args = parser.parse_args()

    if args.player <= 0:
        print("Invalid number of players. (Expected > 0 integer) Set to 10.")
        args.player = default_max_players
    if args.ip <= 0:
        print("Invalid number of players per IP. (Expected > 0 integer) Set to 1.")
        args.ip = default_max_ip_player
    if args.wait < 0:
        print("Invalid waiting time. (Expected >= 0 int) Set to 0.")
        args.wait = default_waiting_time
    if args.name <= 0:
        print("Invalid length of the player name. (Expected > 0 integer) Set to 16.")
        args.name = default_max_name_length

    return args

if __name__ == "__main__":
    if len(sys.argv) > 1:
        args = parse_arguments()
        max_players = args.player
        max_ip_player = args.ip
        waiting_time = args.wait
        max_name_length = args.name
        print(f"Setting max_players to {max_players}.")
        print(f"Setting max_ip_player to {max_ip_player}.")
        print(f"Setting waiting_time to {waiting_time} seconds.")
        print(f"Setting max_name_length to {max_name_length} characters.")
    else:
        print(f"Default settings: max_players = {default_max_players}, max_ip_player = {default_max_ip_player}, waiting_time = {default_waiting_time}, max_name_length = {default_max_name_length}.")
        YesOrNo = input("No arguments are provided. Are you sure you want to continue with the default settings? (y/n): ")
        if YesOrNo == 'n':
            while True:
                try:
                    max_players = int(input("Enter the maximum number of players: "))
                    if max_players <= 0:
                        print("Invalid number of players. (Expected > 0 integer) Set to 10.")
                        max_players = default_max_players
                    break
                except ValueError:
                    print("Invalid input. Please enter a valid integer.")
            
            while True:
                try:
                    max_ip_player = int(input("Enter the maximum number of players per IP address: "))
                    if max_ip_player <= 0:
                        print("Invalid number of players per IP. (Expected > 0 integer) Set to 1.")
                        max_ip_player = default_max_ip_player
                    break
                except ValueError:
                    print("Invalid input. Please enter a valid integer.")
            
            while True:
                try:
                    waiting_time = int(input("Enter the waiting time for each round in seconds (0 means no limitation): "))
                    if waiting_time < 0:
                        print("Invalid waiting time. (Expected >= 0 int) Set to 0.")
                        waiting_time = default_waiting_time
                    break
                except ValueError:
                    print("Invalid input. Please enter a valid integer.")
            
            while True:
                try:
                    max_name_length = int(input("Enter the maximum length of the players' name: "))
                    if max_name_length <= 0:
                        print("Invalid length of the player name. (Expected > 0 integer) Set to 16.")
                        max_name_length = default_max_name_length
                    break
                except ValueError:
                    print("Invalid input. Please enter a valid integer.")

            print(f"Setting max_players to {max_players}.")
            print(f"Setting max_ip_player to {max_ip_player}.")
            print(f"Setting waiting_time to {waiting_time} seconds.")
            print(f"Setting max_name_length to {max_name_length} characters.")
        else:
            args = parse_arguments()
            max_players = args.player
            max_ip_player = args.ip
            waiting_time = args.wait
            max_name_length = args.name
            print(f"Setting max_players to {max_players}.")
            print(f"Setting max_ip_player to {max_ip_player}.")
            print(f"Setting waiting_time to {waiting_time} seconds.")
            print(f"Setting max_name_length to {max_name_length} characters.")

    start_server_thread = threading.Thread(target=start_server)
    start_server_thread.start()
    while (not game_start):
        time.sleep(1.0)
    if len(registered_clients) == 0:
        print("No player is online. Game is terminated.")
        start_server_thread.join()
        sys.exit()
    elif len(registered_clients) == 1:
        registered_clients[0].player[0].send("Only one player is online. Game is terminated.\n".encode('utf-8'))
        registered_clients[0].player[0].send("END\n".encode('utf-8'))
        print("Only one player is online. Game is terminated.")
        start_server_thread.join()
        sys.exit()
    for player in registered_clients:
        player[0].send(player[1].encode('utf-8') + ": Getting ready for the game...\n".encode('utf-8'))
        print(f"Contact {player[1]} to start the game...")
    print("Game is starting...")

    '''Game starts here'''

    from handsclapping_game_online import game_core

    game_core(registered_clients, waiting_time)
    
    start_server_thread.join()
