from pynput import keyboard
from pypose import runRobot
import threading
import time

object_position1 = 500
object_position2 = 500
movement_distance = 1

# Variable to track if the keys are being held down
key_o_held = False
key_p_held = False
key_l_held = False
key_k_held = False


def on_press(key):
    global key_o_held, key_p_held, key_l_held, key_k_held
    try:
        if key.char == 'o':
            key_o_held = True
            #print("Key 'o' pressed")
        elif key.char == 'p':
            key_p_held = True
            #print("Key 'p' pressed")
        elif key.char == 'l':
            key_l_held = True
            #print("Key 'l' pressed")
        elif key.char == 'k':
            key_k_held = True
            #print("Key 'k' pressed")
    except AttributeError:
        pass

def on_release(key):
    global key_o_held, key_p_held, key_l_held, key_k_held
    try:
        if key.char == 'o':
            key_o_held = False
            #print("Key 'o' released")
        elif key.char == 'p':
            key_p_held = False
            #print("Key 'p' released")
        elif key.char == 'l':
            key_l_held = False
            #print("Key 'l' released")
        elif key.char == 'k':
            key_k_held = False
            #print("Key 'k' released")
    except AttributeError:
        pass

def check_key_presses():
    global object_position1
    global object_position2
    while True:
        if key_o_held:
            object_position1 = runRobot.calibrationModeS1(-1,object_position1)
            time.sleep(0.01)  # Adjust the sleep duration as needed

        if key_p_held:
            object_position1 = runRobot.calibrationModeS1(1,object_position1)
            time.sleep(0.01)  # Adjust the sleep duration as needed

        if key_l_held:
            object_position2 = runRobot.calibrationModeS2(-1,object_position2)
            time.sleep(0.01)  # Adjust the sleep duration as needed

        if key_k_held:
            object_position2 = runRobot.calibrationModeS2(1,object_position2)
            time.sleep(0.01)  # Adjust the sleep duration as needed



# Start the keyboard event listener
with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
    # Start the thread to continuously check key presses
    key_thread = threading.Thread(target=check_key_presses)
    key_thread.start()

    try:
        while True:
            # Your main program logic here
            # ...
            time.sleep(1)  # Adjust the sleep duration as needed

    except KeyboardInterrupt:
        listener.stop()
        key_thread.join()