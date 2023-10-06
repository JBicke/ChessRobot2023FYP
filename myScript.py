from pynput import keyboard
from pypose import runRobot
import threading
import time

object_position = 20
movement_distance = 1

# Variable to track if the keys are being held down
key_o_held = False
key_p_held = False

def on_press(key):
    global key_o_held, key_p_held
    try:
        if key.char == 'o':
            key_o_held = True
            #print("Key 'o' pressed")
        elif key.char == 'p':
            key_p_held = True
            #print("Key 'p' pressed")
    except AttributeError:
        pass

def on_release(key):
    global key_o_held, key_p_held
    try:
        if key.char == 'o':
            key_o_held = False
            #print("Key 'o' released")
        elif key.char == 'p':
            key_p_held = False
            #print("Key 'p' released")
    except AttributeError:
        pass

def check_key_presses():
    global object_position
    while True:
        if key_o_held:
            object_position = runRobot.calibrationModeLeft(-1,object_position)
            time.sleep(0.01)  # Adjust the sleep duration as needed

        if key_p_held:
            object_position = runRobot.calibrationModeRight(1,object_position)
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