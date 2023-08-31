import time
import datetime
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

hallpin = 17


GPIO.setup(hallpin, GPIO.IN)

while True:
	
	if (GPIO.input(hallpin) == False):
		print("Magnet Detected")
