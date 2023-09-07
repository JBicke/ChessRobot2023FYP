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



matrix1 = [
    [646, 783, 889, 960, 1002, 1023, 1026, 1018],
    [571, 683, 774, 842, 888, 915, 926, 925],
    [506, 600, 679, 741, 787, 816, 832, 835],
    [446, 527, 596, 653, 696, 725, 743, 748],
    [388, 458, 520, 571, 611, 639, 657, 663],
    [330, 393, 447, 494, 530, 556, 572, 578],
    [270, 326, 376, 417, 450, 474, 488, 491],
    [207, 258, 303, 340, 369, 389, 400, 400]
]

matrix2 = [
    [734, 732, 723, 710, 692, 671, 647, 622],
    [706, 704, 697, 685, 669, 650, 629, 604],
    [677, 676, 670, 659, 645, 628, 607, 584],
    [648, 646, 641, 632, 619, 602, 583, 561],
    [617, 616, 611, 603, 590, 575, 556, 534],
    [585, 584, 580, 572, 560, 545, 527, 505],
    [552, 551, 546, 538, 527, 512, 493, 471],
    [515, 514, 509, 501, 490, 475, 455, 431]
]

servo_angles_matrix = []

for i in range(8):
    row = []
    for j in range(8):
        entry = [matrix1[i][j], matrix2[i][j]]
        row.append(entry)
    servo_angles_matrix.append(row)

def movePiece(move):
	
	moveNumber = convertToNumber(move)
	print(moveNumber)
	col1, row1, col2, row2 = moveNumber
	row1 = int(row1)
	col1 = int(col1)	
	row2 = int(row2)
	col2 = int(col2)
	
	row1 = 9-row1
	row2 = 9-row2
			
	servo1_angle1, servo2_angle1 = servo_angles_matrix[row1-1][col1-1]
	servo1_angle2, servo2_angle2 = servo_angles_matrix[row2-1][col2-1]
	
	print(str(servo1_angle1) + " " + str(servo2_angle1) + " " + str(servo1_angle2) + " " + str(servo2_angle2))
	GPIO.output(motorDownPin, GPIO.LOW)
	GPIO.output(motorUpPin, GPIO.HIGH)

	time.sleep(2)

	is_moving = driver.getReg(1, P_MOVING,1)

	speed1 = 50
	speed2 = 30
	driver.setReg(1,P_GOAL_SPEED_L, [speed1%256,speed1>>8])
	driver.setReg(2,P_GOAL_SPEED_L, [speed2%256,speed2>>8])


	p1 = servo1_angle1
	p2 = servo2_angle1

	driver.setReg(1,P_GOAL_POSITION_L, [p1%256,p1>>8])
	driver.setReg(2,P_GOAL_POSITION_L, [p2%256,p2>>8])


	time.sleep(5)

	# This move down and pick up

	motorDown()

	GPIO.output(magnetPin, GPIO.HIGH)

	time.sleep(2)

	motorUp()

	#Piece is picked up

	p1 = servo1_angle2
	p2 = servo2_angle2

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
