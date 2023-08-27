import serial
import time
import sys
from binascii import b2a_hex

ser = serial.Serial('/dev/ttyUSB0')
print(ser.name)
ser.write(b'\xff\xff\x01\x05\x03\0x1e\0x64\0x00\0x84')
d = ser.read()
print(d)





