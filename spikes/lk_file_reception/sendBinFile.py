import serial
import binascii
import time
import os

print("Starting script to send bin file\n")

serial = serial.Serial(port='/dev/tty.usbmodem14203', baudrate = 115200)
time.sleep(1)

len_data = serial.in_waiting
s = serial.read(len_data)

path = '/some/path/to/file.bin'
file = open(path,'rb')
size = os.path.getsize(path)
print("File size in byte: ", size)
print((size).to_bytes(4, byteorder='big'))
print("File size in hex : ", hex(size))

# Send size first
serial.write((size).to_bytes(4, byteorder='big'))

counter = 0
packet_size = 1
s = serial.read(packet_size)
while s != b'K':
    s = serial.read(packet_size)
    time.sleep(0.01)
text = b'K'
packet_size = 200
while counter < size:
    packet_size = min(packet_size, size - counter)

    text = file.read(packet_size)
    counter += packet_size

    serial.write(text)

    s = serial.read(1) # Wait ACK
    progression = 100 * counter / size
    print(f"Progression: {progression:.2f}%")
    if s == b'F':
        break

if s == b'F' :
    print("Error occurs, try again")
else:
    print("Finished send")
serial.close()

file.close()
