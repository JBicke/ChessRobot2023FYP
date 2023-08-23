from pypose.driver import Driver

driver = Driver(port='/dev/ttyUSB0')

from pypose.ax12 import P_ID, P_MOVING, P_GOAL_SPEED_L

driver.setReg(1,P_ID,(0x01))

# detect_ID = driver.getReg(0,2, 1)
#speed = 888
#driver.setReg(0,P_GOAL_SPEED_L, [speed % 256, speed >>8])
