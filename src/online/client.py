import socket
import threading
import sys
import time
import tkinter as tk

global waiting_time

def client_confirm(client_socket):
    response = input()
    if response != 'n' and response != 'y' and response != '':
        print("Invalid response. Please enter 'y' or 'n'.")
        response = input()
    if response == '':
        response = 'y'
    client_socket.send(response.encode('utf-8'))

def label_update(label, remaining):
    if remaining > 0:
        label.config(text=f"Enter action in {remaining:02d} seconds")
        label.after(1000, label_update, label, remaining - 1)
    else:
        label.config(text="Time's up! Please enter your action now!")

def client_action(client_socket):
    global waiting_time
    if waiting_time <= 0:
        response = input()
        while response == '':
            print("Invalid action. Please enter a valid action.")
            response = input()
    else:
        root = tk.Tk()
        root.title("Countdown Timer")

        label = tk.Label(root, text="", font=("Helvetica", 16))
        label.pack(padx=20, pady=20)

        label_update(label, waiting_time)

        response = input()
        while response == '':
            print("Invalid action. Please enter a valid action.")
            response = input()
        try:
            root.destroy()
        except tk.TclError:
            pass

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
                    client_action(client_socket)
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
    if len(sys.argv) != 3:
        server_ip = input("Enter the server IP address: ")
        client_name = input("Enter your name: ")
    else: 
        server_ip = sys.argv[1]
        client_name = sys.argv[2]
    
    start_client(server_ip, client_name)