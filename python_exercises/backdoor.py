""" A python module for accessing directory contents and CPU name, and then exfiltrating them to a remote server. """

import socket
import os
import platform


def _get_cpu_name():
    """ Gets processor name """
    return platform.processor()


def _get_pwd_contents():
    """ Gets contents of current directory """
    return os.listdir() # "." is default


def _get_host():
    """ Gets the server address to send the data """
    return input("Enter remote server address: ")


def _get_port():
    """ Gets the server port """
    return int(input("Enter the remote server port: "))


def _get_socket():
    """ Creates the socket to connect on """
    return socket.socket(socket.AF_INET, socket.SOCK_STREAM)


def start_server():
    """ Starts a server on user specified HOST and PORT """
    HOST = _get_host()
    PORT = _get_port()
    server = _get_socket()
    server.bind((HOST, PORT))
    server.listen(1)
    print("Server started! Waiting for connections...")
    while True:
        try:
            connection, address = server.accept()
            print("Client connected with address:", address)
            while 1:
                # When client connects, send the data we want straight to the client
                tosend = _get_cpu_name() + "\n"
                for item in _get_pwd_contents():
                    tosend += item + "\n"
                connection.sendall(tosend.encode())
        except:
            connection.close()
        



def start_client():
    """ Starts a client """
    HOST = _get_host()
    PORT = _get_port()

    connection = _get_socket()
    connection.connect((HOST,PORT))
    data = connection.recv(4096)
    print(data.decode('utf-8'))
    exit


function_switch = {
    1: start_server,
    2: start_client
}

user = int(input("(1) Server , (2) Client : "))

if user in function_switch:
    function_switch[user]()
else:
    print('Wrong input. Exiting')