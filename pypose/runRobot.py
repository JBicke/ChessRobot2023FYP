from pypose.driver import Driver
import time
import RPi.GPIO as GPIO
from pypose.ax12 import P_MOVING, P_GOAL_SPEED_L, P_GOAL_POSITION_L, P_BAUD_RATE, P_ID

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
time.sleep(1)
GPIO.setup(4, GPIO.OUT)

magnetPin = 20;
motorDownPin = 21;
motorUpPin = 26;

GPIO.setup(motorDownPin, GPIO.OUT)
GPIO.setup(magnetPin, GPIO.OUT)
GPIO.setup(motorUpPin, GPIO.OUT)

driver = Driver(port='/dev/ttyUSB0')


#record = driver.getReg(1,P_ID,1)
#print(record)

def movePiece():

	GPIO.output(motorDownPin, GPIO.LOW)
	GPIO.output(motorUpPin, GPIO.HIGH)

	time.sleep(2)

	is_moving = driver.getReg(1, P_MOVING,1)

	speed1 = 50
	speed2 = 30
	driver.setReg(1,P_GOAL_SPEED_L, [speed1%256,speed1>>8])
	driver.setReg(2,P_GOAL_SPEED_L, [speed2%256,speed2>>8])


	p1 = 200
	p2 = 505

	driver.setReg(1,P_GOAL_POSITION_L, [p1%256,p1>>8])
	driver.setReg(2,P_GOAL_POSITION_L, [p2%256,p2>>8])


	time.sleep(5)

	# This move down and pick up

	motorDown()

	GPIO.output(magnetPin, GPIO.HIGH)

	time.sleep(2)

	motorUp()

	#Piece is picked up

	p1 = 360
	p2 = 575

	driver.setReg(1,P_GOAL_POSITION_L, [p1%256,p1>>8])
	driver.setReg(2,P_GOAL_POSITION_L, [p2%256,p2>>8])

	time.sleep(5)

	#Drop Piece

	motorDown()

	GPIO.output(magnetPin, GPIO.LOW)

	time.sleep(2)

	motorUp()
	return "Move Completed"

def motorUp():
	GPIO.output(motorDownPin, GPIO.LOW)
	GPIO.output(motorUpPin, GPIO.HIGH)
	time.sleep(3)

def motorDown():
	GPIO.output(motorDownPin, GPIO.HIGH)
	GPIO.output(motorUpPin, GPIO.LOW)
	time.sleep(3)
