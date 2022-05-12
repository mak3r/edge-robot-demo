print("ft232h-samples.py is not a runnable program.")
print("It is a reference document for using the pyftdi commands")
exit()

from pyftdi.ftdi import Ftdi
from pyftdi.i2c import I2cController


# dump the list of devices
# interface expected is [ftdi://ftdi:232h:1/1]
Ftdi.show_devices()

# Create and configure controller
i2c = I2cController()
i2c.configure('ftdi://ftdi:232h:1/1')

# The device address must be known (is there no way to query adresses available?)
slave = i2c.get_port(0x08)

# Write then read
slave.exchange([0x04], 1)

# Read
slave.read_from(0x00, 1)

# Write
slave.write_to(0x04, b'\xAB')


##### Step 01 #####
## Setup 0x47 as the i2c address in register 0x01
slave.write_to(0x01, b'\x47')

##### Step 02 #####
# Write a program
slave.write_to(0x08, b'\xFE\xFD\xFC\xFB\xFA\xF9\xF8\xF7\xF6\xF5\xF4\xF3\xF2\xF1\xF0')

##### Step 03 #####
## Setup the configuration register
## a) right now, load memory to eeprom (0x40) (including the program stored at 0x04 and above)
## b) store the command to read eeprom into memory when the device restarts (0x04)
## c) on reboot, use the value in register 0x01 as the i2c address for this device (0x02)
## (0x40 + 0x04 + 0x02) = 0x46
slave.write_to(0x00, b'\x46')


##### Step 04 #####
## Physically reboot or use the reset command with the reset pin wired to ground
## slave.write_to(0x00, b'\x80')

##### Step 05 #####
## After reset or reboot
## Use the modified i2c address from step 1 above
slave = i2c.get_port(0x47)


# An example program to reset eeprom to use 0x47 
# as the i2c address for this device and load eeprom into the 
# local registers at reboot/reset
from pyftdi.ftdi import Ftdi
from pyftdi.i2c import I2cController
i2c = I2cController()
i2c.configure('ftdi://ftdi:232h:1/1')
slave = i2c.get_port(0x47)
slave.write_to(0x01, b'\x47')
slave.write_to(0x00, b'\x46')
