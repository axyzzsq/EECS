import asyncio
import websockets
import socket
import netifaces

async def handle_message(websocket, message):
    print(f"Received message: {message}")
    await websocket.send(f"Server received: {message}")  # 回显客户端发送的消息

async def websocket_server(websocket, path):
    # 获取客户端的IP地址
    client_ip = websocket.remote_address[0]
    print(f"New connection from {client_ip}")

    try:
        # 监听客户端发来的消息
        async for message in websocket:
            await handle_message(websocket, message)
    except websockets.exceptions.ConnectionClosedError:
        print(f"Connection closed from {client_ip}")

# 获取本机的IP地址
def get_local_ip():
    interfaces = netifaces.interfaces()
    for interface in interfaces:
        if interface == 'lo':
            continue
        addresses = netifaces.ifaddresses(interface)
        ip_info = addresses.get(netifaces.AF_INET)
        if ip_info:
            for info in ip_info:
                ip = info.get('addr')
                if ip and not ip.startswith('127.'):
                    return ip
    return None

# 获取本机的IP地址
host = get_local_ip()
if host is None:
    print("Failed to get local IP address")
    exit()

port = 8080

# 启动WebSocket服务器
start_server = websockets.serve(websocket_server, host, port)

# 打印服务器信息
print(f"WebSocket server is running on {host}:{port}")

# 运行事件循环
asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()

