import RPi.GPIO as GPIO
import time

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(13, GPIO.OUT)

GPIO.output(13, GPIO.HIGH)
time.sleep(10)

GPIO.cleanup()

print("ran")
