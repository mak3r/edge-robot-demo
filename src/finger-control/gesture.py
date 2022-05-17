import time
from pyftdi.ftdi import Ftdi
from pyftdi.i2c import I2cController

i2c = I2cController()
i2c.configure('ftdi://ftdi:232h:1/1')
agent = i2c.get_port(0x47)

cur_gesture = "default.py"
while True:
	# This should really implement a sighup/sigkill handler
	time.sleep(60)
	with open('/config/gesture.conf') as f:
		gesture = f.readlines()
		next_gesture = gesture[0].split("=")[1].rstrip()
	
	if (cur_gesture != next_gesture):
		exec(open(next_gesture).read())
		cur_gesture = next_gesture

