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
agent = i2c.get_port(0x08)

# Write then read
agent.exchange([0x04], 1)

# Read
agent.read_from(0x00, 1)

# Write
agent.write_to(0x04, b'\xAB')


##### Step 01 #####
## Setup 0x47 as the i2c address in register 0x01
agent.write_to(0x01, b'\x47')

##### Step 02 #####
# Write a program
agent.write_to(0x08, b'\xFE\xFD\xFC\xFB\xFA\xF9\xF8\xF7\xF6\xF5\xF4\xF3\xF2\xF1\xF0')

##### Step 03 #####
## Setup the configuration register
## a) right now, load memory to eeprom (0x40) (including the program stored at 0x04 and above)
## b) store the command to read eeprom into memory when the device restarts (0x04)
## c) on reboot, use the value in register 0x01 as the i2c address for this device (0x02)
## (0x40 + 0x04 + 0x02) = 0x46
agent.write_to(0x00, b'\x46')


##### Step 04 #####
## Physically reboot or use the reset command with the reset pin wired to ground
## agent.write_to(0x00, b'\x80')

##### Step 05 #####
## After reset or reboot
## Use the modified i2c address from step 1 above
agent = i2c.get_port(0x47)


# An example program to reset eeprom to use 0x47 
# as the i2c address for this device and load eeprom into the 
# local registers at reboot/reset
from pyftdi.ftdi import Ftdi
from pyftdi.i2c import I2cController
i2c = I2cController()
i2c.configure('ftdi://ftdi:232h:1/1')
agent = i2c.get_port(0x47)
# Set the i2c address to 0x47
agent.write_to(0x01, b'\x47')
# Save the configuration to eeprom
agent.write_to(0x00, b'\x46')

# Close, then open - repeat
agent.write_to(0x10, b'\x28\x48\x68\x88\xA8\xC8\xE8\xFF\xE8\xC8\xA8\x88\x68\x48\x28\x28')
agent.write_to(0x20, b'\x28\x48\x68\x88\xA8\xC8\xE8\xFF\xE8\xC8\xA8\x88\x68\x58\x48\x38')
agent.write_to(0x30, b'\x28\x48\x68\x88\xA8\xC8\xE8\xFF\xE8\xC8\xA8\x88\x68\x58\x48\x38')
agent.write_to(0x40, b'\x28\x48\x68\x88\xA8\xC8\xE8\xFF\xE8\xC8\xA8\x88\x68\x58\x48\x38')
agent.write_to(0x50, b'\x28\x48\x68\x88\xA8\xC8\xE8\xFF\xE8\xC8\xA8\x88\x68\x58\x48\x38')

# Peace
agent.write_to(0x10, b'\x08\x18\x28\x38\x48\x58\x68\x78\x88\x98\xA8\xB8\xC8\xD8\xE8\xF8')
agent.write_to(0x20, b'\x08\x18\x28\x38\x48\x58\x68\x78\x88\x98\xA8\xB8\xC8\xD8\xE8\xF8')
agent.write_to(0x30, b'\x28\x28\x28\x28\x28\x28\x28\x28\x28\x28\x28\x28\x28\x28\x28\x28')
agent.write_to(0x40, b'\x28\x28\x28\x28\x28\x28\x28\x28\x28\x28\x28\x28\x28\x28\x28\x28')
agent.write_to(0x50, b'\x08\x18\x28\x38\x48\x58\x68\x78\x88\x98\xA8\xB8\xC8\xD8\xE8\xF8')

# Delay in seconds between gesture and relax operation
agent.write_to(0x04, b'\x0A')

# Count
agent.write_to(0x10, b'\xFF\xFF\x18\x18\x18\x18\x18\x18\x18\x18\x18\x18\x18\x18\x18\x18')
agent.write_to(0x20, b'\xFF\xFF\xFF\xFF\x18\x18\x18\x18\x18\x18\x18\x18\x18\x18\x18\x18')
agent.write_to(0x30, b'\xFF\xFF\xFF\xFF\xFF\xFF\x18\x18\x18\x18\x18\x18\x18\x18\x18\x18')
agent.write_to(0x40, b'\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x18\x18\x18\x18\x18\x18\x18\x18')
agent.write_to(0x50, b'\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x18\x18\x18\x18\x18\x18')

agent.read_from(0xFF, 1).hex()