import time
from pyftdi.ftdi import Ftdi
from pyftdi.i2c import I2cController

CONNECTED=False
def connect():
	try:
		print("Connecting to ftdi device")
		i2c = I2cController()
		i2c.configure('ftdi://ftdi:232h:1/1')
		agent = i2c.get_port(0x47)
		CONNECTED=True
	except:
		print("Connection failed")
		CONNECTED=False

cur_gesture = "default.py"
while True:
	# This should really implement a sighup/sigkill handler
	time.sleep(30)
	with open('/config/gesture.conf') as f:
		gesture = f.readlines()
		next_gesture = gesture[0].split("=")[1].rstrip()
		print("next_gesture: {}".format(next_gesture))
	
	if (cur_gesture != next_gesture):
		if(not CONNECTED):
			try:
				connect()
				print("Gesture changed next_gesture: {}, cur_gesture: {}".format(next_gesture, cur_gesture))
				exec(open(next_gesture).read())
				cur_gesture = next_gesture
			except:
				print("Gesture changed but device not connected")

