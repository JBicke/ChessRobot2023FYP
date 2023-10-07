from pypose.driver import Driver
import time
import re
import RPi.GPIO as GPIO
from pypose.ax12 import P_MOVING, P_GOAL_SPEED_L, P_GOAL_POSITION_L, P_BAUD_RATE, P_ID
import pyautogui

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

servo1_angle_graveyard = 600
servo2_angle_graveyard = 400

servo1_angle_WQ = 649
servo1_angle_WR = 563
servo1_angle_WB = 474
servo1_angle_WN = 376
servo1_angle_BQ = 734
servo1_angle_BR = 818
servo1_angle_BN = 905
servo1_angle_BB = 992

servo2_angle_WQ = 511
servo2_angle_WR = 481
servo2_angle_WB = 445
servo2_angle_WN = 402
servo2_angle_BQ = 538
servo2_angle_BR = 561
servo2_angle_BN = 581
servo2_angle_BB = 599

castle_p1 = ""
castle_p2 = ""
	
promotion_colour = ''
	
#record = driver.getReg(1,P_ID,1)
#print(record)



matrix1 = [
    [601, 739, 850, 928, 977, 1003, 1012, 1007],
    [535, 647, 741, 813, 864, 895, 911, 913],
    [475, 569, 650, 716, 764, 797, 816, 823],
    [419, 499, 570, 629, 675, 707, 727, 736],
    [363, 434, 497, 549, 591, 622, 641, 650],
    [306, 370, 426, 473, 511, 539, 558, 566],
    [248, 305, 355, 398, 432, 457, 473, 479],
    [185, 237, 283, 321, 351, 373, 385, 387]
]

matrix2 = [
    [729, 729, 722, 710, 693, 673, 650, 625],
    [701, 701, 695, 685, 670, 652, 631, 607],
    [672, 672, 667, 658, 645, 628, 609, 586],
    [643, 643, 638, 630, 618, 603, 584, 563],
    [613, 612, 608, 601, 589, 575, 557, 536],
    [580, 580, 576, 569, 558, 544, 527, 505],
    [546, 546, 542, 535, 525, 510, 493, 471],
    [509, 509, 505, 498, 487, 473, 454, 431]
]

servo_angles_matrix = []

promotion = False
promoted_Piece = ''

for i in range(8):
    row = []
    for j in range(8):
        entry = [matrix1[i][j], matrix2[i][j]]
        row.append(entry)
    servo_angles_matrix.append(row)

def movePiece(move):
	if len(move) == 5:
		promoted_Piece = move[4]
		promotion = True
		move = move[:4]
	moveNumber = convertToNumber(move)
	# print(moveNumber)
	col1, row1, col2, row2 = moveNumber
	row1 = int(row1)
	col1 = int(col1)	
	row2 = int(row2)
	col2 = int(col2)
	
	row1 = 9-row1
	row2 = 9-row2
			
	servo1_angle1, servo2_angle1 = servo_angles_matrix[row1-1][col1-1]
	servo1_angle2, servo2_angle2 = servo_angles_matrix[row2-1][col2-1]
	
	# print(str(servo1_angle1) + " " + str(servo2_angle1) + " " + str(servo1_angle2) + " " + str(servo2_angle2))
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


def takePiece(row,col):
	row = int(row)
	col = int(col)	
			
	servo1_angle, servo2_angle = servo_angles_matrix[row-1][col-1]
	
	GPIO.output(motorDownPin, GPIO.LOW)
	GPIO.output(motorUpPin, GPIO.HIGH)

	time.sleep(2)

	is_moving = driver.getReg(1, P_MOVING,1)

	speed1 = 50
	speed2 = 30
	driver.setReg(1,P_GOAL_SPEED_L, [speed1%256,speed1>>8])
	driver.setReg(2,P_GOAL_SPEED_L, [speed2%256,speed2>>8])


	p1 = servo1_angle
	p2 = servo2_angle

	driver.setReg(1,P_GOAL_POSITION_L, [p1%256,p1>>8])
	driver.setReg(2,P_GOAL_POSITION_L, [p2%256,p2>>8])


	time.sleep(5)

	# This move down and pick up

	motorDown()

	GPIO.output(magnetPin, GPIO.HIGH)

	time.sleep(2)

	motorUp()

	#Piece is picked up

	p1 = servo1_angle_graveyard
	p2 = servo2_angle_graveyard

	driver.setReg(1,P_GOAL_POSITION_L, [p1%256,p1>>8])
	driver.setReg(2,P_GOAL_POSITION_L, [p2%256,p2>>8])

	time.sleep(5)

	GPIO.output(magnetPin, GPIO.LOW)
	
	time.sleep(1)
	##Drop Piece

	# motorDown()

	# GPIO.output(magnetPin, GPIO.LOW)

	# time.sleep(2)

	# motorUp()
	
	
	return "Take Completed"
	
def extract_Text(input_string, text):
    fen_index = input_string.find(text)

    if fen_index != -1:
        # Add the length of "FEN: " to get the starting index of the desired text
        start_index = fen_index + len(text)
        # Extract the text after "FEN: "
        extracted_text = input_string[start_index:]
        return extracted_text.strip()  # Remove leading/trailing whitespaces
    else:
        return None  # "FEN: " not found in the string
        
def del_Text_After(input_string, character):
    index = input_string.find(character)

    if index != -1:
        # Slice the string up to the specified character (excluding the character itself)
        modified_string = input_string[:index]
        return modified_string
    else:
        return input_string  # Character not found, return the original string


def castle(location):
	# print(location)
	if location == "K":
		castle_p1 = "h1"
		castle_p2 = "f1"
	if location == "Q":
		castle_p1 = "a1"
		castle_p2 = "d1"
	if location == "k":
		castle_p1 = "h8"
		castle_p2 = "f8"
	if location == "q":
		castle_p1 = "a8"
		castle_p2 = "d8"
	
	print(castle_p1+castle_p2)
	movePiece(castle_p1+castle_p2)
	return "Castle Complete"


def promotion(square, piece):
	squareNumber = convertToNumber(square)
	col, row = squareNumber
	row = int(row)
	col = int(col)	
	
	row = 9-row
	
			
	servo1_return, servo2_return = servo_angles_matrix[row-1][col-1]
		
	if piece == "wq":
		p1 = servo1_angle_WQ
		p2 = servo2_angle_WQ
		pickUpPromote(p1,p2,servo1_return,servo2_return)
	elif piece == "wr":
		p1 = servo1_angle_WR
		p2 = servo2_angle_WR
		pickUpPromote(p1,p2,servo1_return,servo2_return)
	elif piece == "wn":
		p1 = servo1_angle_WN
		p2 = servo2_angle_WN
		pickUpPromote(p1,p2,servo1_return,servo2_return)
	elif piece == "wb":
		p1 = servo1_angle_WB
		p2 = servo2_angle_WB
		pickUpPromote(p1,p2,servo1_return,servo2_return)
	elif piece == "bq":
		p1 = servo1_angle_BQ
		p2 = servo2_angle_BQ
		pickUpPromote(p1,p2,servo1_return,servo2_return)
	elif piece == "br":
		p1 = servo1_angle_BR
		p2 = servo2_angle_BR
		pickUpPromote(p1,p2,servo1_return,servo2_return)
	elif piece == "bn":
		p1 = servo1_angle_BN
		p2 = servo2_angle_BN
		pickUpPromote(p1,p2,servo1_return,servo2_return)
	elif piece == "bb":
		p1 = servo1_angle_BB
		p2 = servo2_angle_BB
		pickUpPromote(p1,p2,servo1_return,servo2_return)
		
	
def pickUpPromote(p1,p2,servo1_togoback,servo2_togoback):
	
	motorDown()

	GPIO.output(magnetPin, GPIO.HIGH)

	time.sleep(2)

	motorUp()

	#Piece is picked up

	pg1 = servo1_angle_graveyard
	pg2 = servo2_angle_graveyard

	driver.setReg(1,P_GOAL_POSITION_L, [pg1%256,pg1>>8])
	driver.setReg(2,P_GOAL_POSITION_L, [pg2%256,pg2>>8])

	time.sleep(5)

	GPIO.output(magnetPin, GPIO.LOW)
	
	time.sleep(1)
	
	GPIO.output(motorDownPin, GPIO.LOW)
	GPIO.output(motorUpPin, GPIO.HIGH)

	time.sleep(2)

	
	driver.setReg(1,P_GOAL_POSITION_L, [p1%256,p1>>8])
	driver.setReg(2,P_GOAL_POSITION_L, [p2%256,p2>>8])
	
	time.sleep(5)

	# This move down and pick up

	motorDown()

	GPIO.output(magnetPin, GPIO.HIGH)

	time.sleep(2)

	motorUp()

	#Piece is picked up
	
	p1 = servo1_togoback
	p2 = servo2_togoback
	
	driver.setReg(1,P_GOAL_POSITION_L, [p1%256,p1>>8])
	driver.setReg(2,P_GOAL_POSITION_L, [p2%256,p2>>8])
	
	time.sleep(5)
	
	motorDown()
	
	GPIO.output(magnetPin, GPIO.LOW)

	time.sleep(2)

	motorUp()
	

def extract_eP(text):
	words = text.split()
	return words[-3]
		
def CastleCheckWhite(Castle_Fen):
	if Castle_Fen.find('K') != -1:
		Castle_K = True
	else:
		Castle_K = False
	if Castle_Fen.find('Q') != -1:
		Castle_Q = True
	else:
		Castle_Q = False
	return Castle_K, Castle_Q

def CastleCheckBlack(Castle_Fen):
	if Castle_Fen.find('k') != -1:
		Castle_K = True
	else:
		Castle_K = False
	if Castle_Fen.find('q') != -1:
		Castle_Q = True
	else:
		Castle_Q = False
	return Castle_K, Castle_Q


# Set initial position of the object
object_position = 500


# Function to move the object
def move_object(direction):
    global object_position
    

def calibrationModeS2(direction,object_position):
	object_position += direction * 1
	#print(f"Object position: {object_position}")]
	if object_position > 1022:
		object_position = 1022
	driver.setReg(2,P_GOAL_POSITION_L, [object_position%256,object_position>>8])
	return object_position

def calibrationModeS1(direction,object_position):
	object_position += direction * 1
	if object_position > 1022:
		object_position = 1022
	# print(f"Object position: {object_position}")
	driver.setReg(1,P_GOAL_POSITION_L, [object_position%256,object_position>>8])
	return object_position
