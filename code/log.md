# Journal

## Meeting Notes

## Board Design Notes

### Rev A Notes

Looking at the 3 major designs we have 

### Rev A Test Notes

- Buck works as expected (3v3 output detected)

### Rev B Notes

Changes Needed:
- Smaller Buck: 
- new I2C mux: https://www.ti.com/product/TMUX131
- Need to add direction pin to MCF8316A
- may need UART bridge
- need to reverse direction of diode on buck
- can use 220uF ceramic capacitor on buck



## Code Notes

### MCF8316A Notes

#### Intro:

After looking at the chips (MCF8316A and MCF8329A), it looks like all 3 chips have the same slave address of 0x01, and they don't have configuration pins for the address. After consideration, it looks like we will have to implement an I2C mux to change between the 3 chips to prevent overwriting other devices and causing the bus to crash. 

To code the I2C bus, we're using the examples and the TRM to make a simple transmit/receive I2C driver. This I2C driver just needs to be able to configure the device on bootup and make sure that there are no faults before the robot starts moving. After the chips start moving the motors, the I2C bus can't read without stopping the motors, so the main structure will be:
```
mcf8316a\
|--mcf8316a.h\
|--mcf8316a.c\

i2c_esp\
|--i2c_esp.h\
|--i2c_esp.c\
```
Where `i2c_esp.h` will have `init`, `read`, `write`, and `read/write` functions. Then, the `mcf8316a` chip will call the necessary functions to read and write to the chips eeprom in its init function. The chip will also have a control function that allows us to control the PWM and a read function to read the feedback speed.

### Bluetooth Notes

