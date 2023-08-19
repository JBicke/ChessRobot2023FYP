from jamesTesting import Driver

driver = Driver(port='/dev/ttyUSB0')

from ax12 import P_MOVING, P_GOAL_SPEED_L


s_moving = driver.getReg(2,P_MOVING, 1)

