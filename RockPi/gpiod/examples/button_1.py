#From gpiod/tests/button.py -  edited 7/30/23 - JRA for notes and clarity

# This code demo's how to wait on an edge event - the code is blocking (truly waits until it happens)

# pylint: disable=missing-docstring
import sys
from datetime import timedelta

from gpiod import chip, line_event, line_request

try:
    if len(sys.argv) > 2:
        BUTTON_CHIP = sys.argv[1]
        BUTTON_LINE_OFFSET = int(sys.argv[2])
        if len(sys.argv) > 3:
            edge = sys.argv[3]
            if edge[0] == "r":
                BUTTON_EDGE = line_request.EVENT_RISING_EDGE
            elif edge[0] == "f":
                BUTTON_EDGE = line_request.EVENT_FALLING_EDGE
            else:
                BUTTON_EDGE = line_request.EVENT_BOTH_EDGES
        else:
            BUTTON_EDGE = line_request.EVENT_BOTH_EDGES

## we use class line_request to return the number associated with the edge type (this is likely
##    a lookup)            

    else:
        raise Exception()
# pylint: disable=broad-except
except Exception:
    print(
        """Usage:
    python3 button-1.py <chip> <line offset> [rising|falling|both]"""
    )
    sys.exit()


## now we instantiate an instance of chip class ... "c" is a chip object
c = chip(BUTTON_CHIP)

## now we get an object for our particular line (button)
button = c.get_line(BUTTON_LINE_OFFSET)

## config is a line_request object - set some variables related to the upcoming line action
config = line_request()
## set a name to the particular line request (just setting a text variable)
config.consumer = "Button"
## set the variable "request_type"  (a number related to edge selection)
config.request_type = BUTTON_EDGE

## button is a chip instance thing... so now we pass the config parameters to the button instance (chip.get_line)
button.request(config)

## for a falling edge event, the "event_get_fd" is "4"
print("event fd: ", button.event_get_fd())

while True:
## the actual stall is in the button.event_wait() statement... I think if it gets an appropriate event it will 
##  fall through to test what event happened with the event_read()    
    if button.event_wait(timedelta(seconds=10)):
        # event_read() is blocking function.  (I think the blocking is done in the previous statement)
        event = button.event_read()
        if event.event_type == line_event.RISING_EDGE:
            print("rising: ", event.timestamp)
        else:
            print("falling: ", event.timestamp)
    else:
        print("timeout(10s)")
