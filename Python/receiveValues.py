import socket
import sys	
import struct 
import csv
import msvcrt

UDP_IP = "0.0.0.0"  # Listen on all available network interfaces
UDP_PORT = 1234  # Specify the UDP port you want to listen on

data_rows = []

#create an INET, STREAMing socket
try:
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
except socket.error:
	print("Failed to create socket")
	sys.exit()
print ("Socket Created")

try: 
    sock.bind((UDP_IP, UDP_PORT))
except socket.error:
    print("Failed to bind")
    sys.exit()
print("Bind to IP and Port")

setpoint = ''

while True:
    data, addr = sock.recvfrom(1024)  # Adjust buffer size as needed

    if msvcrt.kbhit():
        setpoint += str(msvcrt.getch()).lstrip("b'").rstrip("'")
        if(str(setpoint[len(setpoint)-2:])==str('\\r')):
            sock.sendto(setpoint.encode(), ("192.168.137.172", UDP_PORT))
            print("MANDANDO nuevo setpoint: ",setpoint)
            setpoint=''
    time = str(data.decode())[0:4]
    y = str(data.decode())[4:8]
    u = str(data.decode())[8:]
    print(time,y,u)