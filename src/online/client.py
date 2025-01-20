import socket
import threading
import sys

def client_confirm(client_socket):
    response = input()
    if (response != 'n' and response != 'y'):
        print("Invalid response. Please enter 'y' or 'n'.")
        response = input()
    client_socket.send(response.encode('utf-8'))

def client_action(client_socket):
    response = input()
    client_socket.send(response.encode('utf-8'))

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
                elif message == "ACTION":
                    client_response_thread = threading.Thread(target=client_action, args=(client_socket,))
                    client_response_thread.start()
                else:
                    print(f"[server] {message}")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        client_socket.close()
        print("Client closed.")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python client.py <server IP> <client name>")
        sys.exit(1)
    
    server_ip = sys.argv[1]
    client_name = sys.argv[2]
    start_client(server_ip, client_name)