import socket
import threading
import sys

def client_response(client_socket):
    response = input()
    if (response != 'n' and response != 'y'):
        print("Invalid response. Please enter 'y' or 'n'.")
        response = input()
    client_socket.send(response.encode('utf-8'))

def start_client(server_ip, client_name):
    # 创建一个socket对象
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    try:
        # 连接服务器
        client_socket.connect((server_ip, 38921))
        
        # 发送数据
        message = client_name
        client_socket.send(message.encode('utf-8'))
        
        # 持续接收数据，直到收到结束信号
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
                elif message == "RESPOND":
                    client_response_thread = threading.Thread(target=client_response, args=(client_socket,))
                    client_response_thread.start()
                else:
                    print(f"[server] {message}")
    except Exception as e:
        print(f"发生错误: {e}")
    finally:
        client_socket.close()
        print("客户端已关闭")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("用法: python client.py <服务器IP地址> <客户端名称>")
        sys.exit(1)
    
    server_ip = sys.argv[1]
    client_name = sys.argv[2]
    start_client(server_ip, client_name)