import socket
import struct 
import csv
import msvcrt

UDP_IP = "0.0.0.0"  # Listen on all available network interfaces
UDP_PORT = 1234  # Specify the UDP port you want to listen on

data_rows = []


sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

setpoint = ''

while True:
    data, addr = sock.recvfrom(1024)  # Adjust buffer size as needed
    print("Received data:", data.decode())
    if msvcrt.kbhit():
        setpoint += str(msvcrt.getch()).lstrip("b'").rstrip("'")