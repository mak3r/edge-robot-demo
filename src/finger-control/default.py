from pyftdi.ftdi import Ftdi
from pyftdi.i2c import I2cController
i2c = I2cController()
i2c.configure('ftdi://ftdi:232h:1/1')
agent = i2c.get_port(0x47)

# Load program from eeprom
agent.write_to(0x00, b'\x04')
