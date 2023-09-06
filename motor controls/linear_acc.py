import RPi.GPIO as GPIO
import time

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
time.sleep(2)
GPIO.setup(4, GPIO.OUT)

magnetPin = 20;
motorDownPin = 21;
motorUpPin = 26;


GPIO.setup(motorDownPin, GPIO.OUT)
GPIO.setup(magnetPin, GPIO.OUT)
GPIO.setup(motorUpPin, GPIO.OUT)

# GPIO.output(4, GPIO.HIGH)
# time.sleep(2)
# GPIO.output(21, GPIO.HIGH)
# time.sleep(2)
# GPIO.output(20, GPIO.HIGH)
# time.sleep(2)
# GPIO.output(26, GPIO.HIGH)
# time.sleep(2)


# GPIO.output(4, GPIO.LOW)
# time.sleep(2)
# GPIO.output(21, GPIO.LOW)
# time.sleep(2)
# GPIO.output(20, GPIO.LOW)
# time.sleep(2)
# GPIO.output(26, GPIO.LOW)
# time.sleep(2)

GPIO.output(motorDownPin, GPIO.HIGH)
GPIO.output(motorUpPin, GPIO.LOW)
time.sleep(2)

GPIO.output(magnetPin, GPIO.HIGH)
time.sleep(2)


GPIO.output(motorDownPin, GPIO.LOW)
GPIO.output(motorUpPin, GPIO.HIGH)
time.sleep(2)



GPIO.output(motorDownPin, GPIO.HIGH)
GPIO.output(motorUpPin, GPIO.LOW)
time.sleep(2)

GPIO.output(magnetPin, GPIO.LOW)
time.sleep(2)

GPIO.output(motorDownPin, GPIO.LOW)
GPIO.output(motorUpPin, GPIO.HIGH)
time.sleep(2)

GPIO.cleanup()

print("ran")

def move_Down():
	GPIO.output(motorDownPin, GPIO.LOW)
	GPIO.output(motorUpPin, GPIO.HIGH)
	time.sleep(2)
	
def move_Up():
	
