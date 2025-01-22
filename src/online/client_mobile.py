import socket
import threading
import sys
import time

global waiting_time

def client_confirm(client_socket):
    response = input()
    if response != 'n' and response != 'y':
        print("Invalid response. Please enter 'y' or 'n'.")
        response = input()
    client_socket.send(response.encode('utf-8'))

def client_action(client_socket):
    global waiting_time
    if waiting_time > 0:
        print(f"Enter action in {waiting_time} seconds")
    response = input()
    while response == '':
        print("Invalid action. Please enter a valid action.")
        response = input()
    client_socket.send(response.encode('utf-8'))

def client_set_waiting_time(client_socket):
    global waiting_time
    client_socket.send("GET_READY".encode('utf-8'))
    waiting_time = int(client_socket.recv(1024).decode('utf-8'))

def start_client(server_ip, client_name):
    # create a socket object
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    try:
        # connect to the server
        client_socket.connect((server_ip, 38921))
        
        # send the client name to the server
        message = client_name
        client_socket.send(message.encode('utf-8'))
        
        # receive the server's response consecutively
        buffer = ""
        receiving = True
        while receiving:
            data = client_socket.recv(1024).decode('utf-8')
            buffer += data
            while "\n" in buffer:
                message, buffer = buffer.split("\n", 1)
                if message == "END":
                    receiving = False
                    break
                elif message == "CONFIRM":
                    client_response_thread = threading.Thread(target=client_confirm, args=(client_socket,))
                    client_response_thread.start()
                elif message == "WAITING_TIME":
                    client_set_waiting_time(client_socket)
                elif message == "ACTION":
                    client_action_thread = threading.Thread(target=client_action, args=(client_socket,))
                    client_action_thread.start()
                    if waiting_time > 0:
                        for i in range(waiting_time, 0, -1):
                            if client_action_thread.is_alive():
                                if i % 5 == 0:
                                    print(f"Remaining time: {i} seconds")
                                time.sleep(1)
                            else:
                                break
                        if client_action_thread.is_alive():
                            print("Time's up! Your action is committed as TIMEOUT.")
                elif message == "":
                    print("")
                else:
                    print(f"[server] {message}")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        client_socket.close()
        print("Client closed.")

if __name__ == "__main__":
    server_ip = input("Enter server's IP: ")
    client_name = input("Enter your name: ")
    start_client(server_ip, client_name)