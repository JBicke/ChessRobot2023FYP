from driver import Driver
import time
import RPi.GPIO as GPIO

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
time.sleep(2)
GPIO.setup(4, GPIO.OUT)

GPIO.setup(21, GPIO.OUT)
GPIO.setup(20, GPIO.OUT)
GPIO.setup(26, GPIO.OUT)

driver = Driver(port='/dev/ttyUSB0')

from ax12 import P_MOVING, P_GOAL_SPEED_L, P_GOAL_POSITION_L, P_BAUD_RATE, P_ID

#record = driver.getReg(1,P_ID,1)
#print(record)

GPIO.output(21, GPIO.LOW)
GPIO.output(26, GPIO.HIGH)

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
GPIO.output(21, GPIO.HIGH)
GPIO.output(26, GPIO.LOW)

time.sleep(2)

GPIO.output(20, GPIO.HIGH)

time.sleep(2)

GPIO.output(21, GPIO.LOW)
GPIO.output(26, GPIO.HIGH)

time.sleep(3)

#Piece is picked up

p1 = 360
p2 = 575

driver.setReg(1,P_GOAL_POSITION_L, [p1%256,p1>>8])
driver.setReg(2,P_GOAL_POSITION_L, [p2%256,p2>>8])

time.sleep(5)

#Drop Piece

GPIO.output(21, GPIO.HIGH)
GPIO.output(26, GPIO.LOW)

time.sleep(3)

GPIO.output(20, GPIO.LOW)

time.sleep(3)

GPIO.output(21, GPIO.LOW)
GPIO.output(26, GPIO.HIGH)

time.sleep(5)
