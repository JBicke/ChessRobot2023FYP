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


servo_angles_matrix = []
x=1
y=40

for _ in range(8):
	row = []
	for _ in range(8):
		square_angles = [x, y]
		x=x+2
		y=y+1
		row.append(square_angles)
	servo_angles_matrix.append(row)

print(servo_angles_matrix)



def movePiece(move):
	
	moveNumber = convertToNumber(move)
	print(moveNumber)
	row1, col1, row2, col2 = moveNumber
	row1 = int(row1)
	col1 = int(col1)	
	row2 = int(row2)
	col2 = int(col2)		
	servo1_angle1, servo1_angle2 = servo_angles_matrix[row1-1][col1-1]
	servo2_angle1, servo2_angle2 = servo_angles_matrix[row2-1][col2-1]
	
	print(str(servo1_angle1) + " " + str(servo1_angle2) + " " + str(servo2_angle1) + " " + str(servo2_angle2))
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


def convertToNumber(input_string):
	result = ""
    
	for char in input_string:
		if char.isalpha():
			result += str(ord(char.lower()) - ord('a') + 1)
		else:
			result += char # Add any remaining numbers to the result
			
	return result
