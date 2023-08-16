from pypose.driver import Driver

driver = Driver(port='/dev/ttyUSB0')

from pypose.ax12 import P_MOVING, P_GOAL_SPEED_L



s_moving = driver.getReg(0,P_MOVING, 1)
speed = 888
driver.setReg(0,P_GOAL_SPEED_L, [speed % 256, speed >>8])
