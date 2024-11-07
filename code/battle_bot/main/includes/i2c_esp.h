#ifndef I2C_ESP_H
#define I2C_ESP_H


#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_types.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c_master.h"
#include "esp_check.h"


// change I2C pins
#define I2C_MASTER_SCL_GPIO                     CONFIG_I2C_SCL
#define I2C_MASTER_SDA_GPIO                     CONFIG_I2C_SDA
#define I2C_GPIO_MUX_SEL_0                      CONFIG_GPIO_MUX_SEL_0
#define I2C_GPIO_MUX_SEL_1                      CONFIG_GPIO_MUX_SEL_1
#define MASTER_FREQUENCY                        CONFIG_I2C_MASTER_FREQ
#define PORT_NUMBER                             I2C_NUM_1

// initialize i2c bus and ESP32 to be master on I2C port 0
esp_err_t i2c_init_master(i2c_master_bus_handle_t* bus_handle);

// initialize slave device on master bus with slave address
esp_err_t i2c_init_device(i2c_master_bus_handle_t* bus_handle, i2c_master_dev_handle_t* dev_handle, uint8_t address);

// write to slave device
esp_err_t i2c_write(i2c_master_dev_handle_t *dev_handle, const uint8_t *write_buffer, size_t data_length);

// check if slave device is connected
esp_err_t i2c_check(i2c_master_bus_handle_t *bus_handle, uint8_t address);

// stall the CPU
void i2c_wait_idle(uint8_t ms);

// select correct i2c bus
esp_err_t select_i2c_bus(uint8_t mux_select);

#endif /* I2C_ESP_H */