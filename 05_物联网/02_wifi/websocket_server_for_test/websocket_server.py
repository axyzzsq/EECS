import asyncio
import websockets
import socket

async def handle_message(websocket, message):
    print(f"Received message: {message}")
    await websocket.send(f"Server received: {message}")

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

# 获取主机的IP地址
host = socket.gethostbyname(socket.gethostname())
port = 8080

# 启动WebSocket服务器
start_server = websockets.serve(websocket_server, host, port)

# 打印服务器信息
print(f"WebSocket server is running on {host}:{port}")

# 运行事件循环
asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()

