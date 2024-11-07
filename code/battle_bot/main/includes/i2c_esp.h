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



esp_err_t i2c_init_master(i2c_master_bus_handle_t* bus_handle);
esp_err_t i2c_init_device(i2c_master_bus_handle_t* bus_handle, i2c_master_dev_handle_t* dev_handle, uint8_t address);
esp_err_t i2c_write(i2c_master_dev_handle_t *dev_handle, const uint8_t *write_buffer, size_t data_length);
esp_err_t i2c_read(i2c_master_dev_handle_t *dev_handle, const uint8_t *control_buffer,
    size_t control_length, uint8_t *read_buffer, size_t data_length);
esp_err_t i2c_check(i2c_master_bus_handle_t *bus_handle, uint8_t address);
void i2c_wait_idle();

#endif /* I2C_ESP_H */