from pypose import runRobot
import subprocess
import operations

mode = 0
mode = input("Type 1 for input game, 2 for automated game, or 3 for interactive game:")
if mode == "1":
    process = subprocess.Popen(['python', 'mode1.py'])
    process.wait()
elif mode == "2":
    process = subprocess.Popen(['python', 'mode2.py'])
    process.wait()
elif mode == "3":
    print("This hasn't been integrated yet")
    #process = subprocess.Popen(['python', 'mode3.py'])
    #process.wait()
