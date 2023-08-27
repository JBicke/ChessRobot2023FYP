import RPi.GPIO as GPIO
import time

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
time.sleep(2)
GPIO.setup(4, GPIO.OUT)

GPIO.setup(21, GPIO.OUT)
GPIO.setup(20, GPIO.OUT)
GPIO.setup(26, GPIO.OUT)

# GPIO.output(4, GPIO.HIGH)
# time.sleep(2)
# GPIO.output(21, GPIO.HIGH)
# time.sleep(2)
# GPIO.output(20, GPIO.HIGH)
# time.sleep(2)
# GPIO.output(26, GPIO.HIGH)
# time.sleep(2)


# GPIO.output(4, GPIO.LOW)
# time.sleep(2)
# GPIO.output(21, GPIO.LOW)
# time.sleep(2)
# GPIO.output(20, GPIO.LOW)
# time.sleep(2)
# GPIO.output(26, GPIO.LOW)
# time.sleep(2)

GPIO.output(21, GPIO.HIGH)
GPIO.output(26, GPIO.LOW)
time.sleep(2)

GPIO.output(20, GPIO.HIGH)
time.sleep(2)


GPIO.output(21, GPIO.LOW)
GPIO.output(26, GPIO.HIGH)
time.sleep(2)



GPIO.output(21, GPIO.HIGH)
GPIO.output(26, GPIO.LOW)
time.sleep(2)

GPIO.output(20, GPIO.LOW)
time.sleep(2)

GPIO.output(21, GPIO.LOW)
GPIO.output(26, GPIO.HIGH)
time.sleep(2)

GPIO.cleanup()

print("ran")
