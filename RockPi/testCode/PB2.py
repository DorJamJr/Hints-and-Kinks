# Code to do some testing of OPi interrupt handling capability

# Results: The input

import radxa.CM3
from OPi import GPIO
import time

GPIO.setmode(radxa.CM3.BOARD)

# all IO can now be referenced by the pin# to which that IO is located on the
#  40 pin IO connector

# In our case: Amber LED is on pin 31
#              PB1 on pin 5
#              PB2 on pin 7

PB1 = 5
PB2 = 7
LED = 31

GPIO.setup(PB1, GPIO.IN)
GPIO.setup(PB2, GPIO.IN)

GPIO.setup(LED, GPIO.OUT)

def int_service():
  print("PB pressed")

# this is setup for calling an interrupt service at instant event detected
#  NOT WORKING
#GPIO.add_event_detect(PB1, GPIO.FALLING,callback = int_service)
#GPIO.add_event_detect(PB2, GPIO.FALLING,callback = int_service)

# this is setup for detecting an event to be handled when
# function event_detected is called
#  WORKING
GPIO.add_event_detect(PB1, GPIO.FALLING,callback = None)
GPIO.add_event_detect(PB2, GPIO.FALLING,callback = None)


# see if we can get an result from pressing a button during the while loop...

loop_count = 0

while (loop_count < 5):
  print ("Loop count = "+str(loop_count))
  GPIO.output(LED, GPIO.HIGH)
  time.sleep(2)

# [try pushing button while LED is off]

  GPIO.output(LED, GPIO.LOW)
  time.sleep(1)

# see if a button was pushed while led off
  if GPIO.event_detected(PB1):
    print("#### PB1 pushed")
  if GPIO.event_detected(PB2):
    print("#### PB2 pushed")

# Now see if we can turn the event detection OFF and then read the current
#  state of the PB's

# Turning off the event detect for a line before reading its state ISte REQUIRED
  GPIO.remove_event_detect(PB1)
  GPIO.remove_event_detect(PB2)
  time.sleep(0.1)
  print("Read the state of PB1 and PB2")
  print("  PB1 = "+str(GPIO.input(PB1)))
  print("  PB2 = "+str(GPIO.input(PB2)))

# Now reenable the event detect
  GPIO.add_event_detect(PB1, GPIO.FALLING, callback=None)
  GPIO.add_event_detect(PB2, GPIO.FALLING, callback=None)
  
  loop_count += 1

