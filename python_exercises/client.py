import socket

HOST = '10.0.0.8'
PORT = 44444

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))
s.sendall(b'Hello World')
exit