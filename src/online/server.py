import time
import socket
import threading
import sys

server_running = True
game_start = False
current_players = 0
confirmed_players = 0
registered_clients = []
registered_clients_lock = threading.Lock()

if len(sys.argv) > 1:
    try:
        max_players = int(sys.argv[1])
        print("Setting max_players to " + str(max_players) + ".")
    except ValueError:
        print("Invalid max_players value. Using default value 10.")
        max_players = 10
else:
    max_players = 10

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
    global registered_clients_lock
    client_socket.send("Enter y/n to confirm to the game...\n".encode('utf-8'))
    client_socket.send("RESPOND\n".encode('utf-8'))
    response = client_socket.recv(1024).decode('utf-8')
    if (response == 'n'):
        client_socket.send("You have refused to confirm to the game.\n".encode('utf-8'))
        client_socket.send("END\n".encode('utf-8'))
        print(f"[client] Player {name} refused to confirm.")
        current_players -= 1
        client_socket.close()
        return
    
    with registered_clients_lock:
        if len(registered_clients) >= max_players:
            client_socket.send("The game is full. Please try again later.\n".encode('utf-8'))
            client_socket.send("END\n".encode('utf-8'))
            print(f"[client] Player {name} is removed due to the full game.")
            current_players -= 1
            client_socket.close()
            return
        for (player_socket, player_name) in registered_clients:
            if player_name == name:
                client_socket.send("There is already a player with the same name ".encode('utf-8') + name.encode('utf-8') + ". Please log in with another name.\n".encode('utf-8'))
                client_socket.send("END\n".encode('utf-8'))
                print(f"[client] Player {name} is removed due to the same name with another player.")
                current_players -= 1
                client_socket.close()
                return
    registered_clients.append((client_socket, name))
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
        return
    print(f"[client] Player registration: {name}")

    client_socket.send("Hello, ".encode('utf-8') + name.encode('utf-8') + "\n".encode('utf-8'))
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
    print(f"服务器已启动，IP地址: {ip_address}, 端口号: 12345")
    
    server_socket.listen(5)
    print("等待连接...")

    server_socket.settimeout(30.0)
    
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
        print("Server has been enforced to terminate...")
    finally:
        server_running = False
        server_socket.close()

if __name__ == "__main__":
    start_server_thread = threading.Thread(target=start_server)
    start_server_thread.start()
    while (not game_start):
        time.sleep(1.0)
    if len(registered_clients) == 0:
        print("No player is online. Game is terminated.")
        start_server_thread.join()
        exit
    elif len(registered_clients) == 1:
        registered_clients[0].player[0].send("Only one player is online. Game is terminated.\n".encode('utf-8'))
        registered_clients[0].player[0].send("END\n".encode('utf-8'))
        print("Only one player is online. Game is terminated.")
        start_server_thread.join()
        exit
    for player in registered_clients:
        player[0].send(player[1].encode('utf-8') + ": Getting ready for the game...\n".encode('utf-8'))
        print(f"Contact {player[1]} to start the game...")
        player[0].send("END\n".encode('utf-8'))
    print("Game is starting...")

    '''Game starts here'''
    
    start_server_thread.join()
