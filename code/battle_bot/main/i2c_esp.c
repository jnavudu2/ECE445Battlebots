#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_types.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c_master.h"
#include "i2c_esp.h"
#include "esp_check.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define I2C_MASTER_SCL_IO                       2
#define I2C_MASTER_SDA_IO                       3
#define MASTER_FREQUENCY                    40000
#define PORT_NUMBER         I2C_NUM_1

esp_err_t i2c_init_master(i2c_master_bus_handle_t* bus_handle) {
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,      // clock rate
        .i2c_port = PORT_NUMBER,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,                 // default: 7
        .flags.enable_internal_pullup = true,   // default: true
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, bus_handle));

    return ESP_OK;
}

esp_err_t i2c_init_device(i2c_master_bus_handle_t* bus_handle, i2c_master_dev_handle_t* dev_handle, uint8_t address) {
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = address,
        .scl_speed_hz = MASTER_FREQUENCY,
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(*bus_handle, &dev_cfg, dev_handle));

    return ESP_OK;
}

esp_err_t i2c_write(i2c_master_dev_handle_t* dev_handle, const uint8_t* write_buffer, size_t data_length) {
    ESP_ERROR_CHECK(i2c_master_transmit(*dev_handle, write_buffer, data_length, -1));

    return ESP_OK;
}

esp_err_t i2c_read(i2c_master_dev_handle_t* dev_handle, const uint8_t* control_buffer,
    size_t control_length, uint8_t* read_buffer, size_t data_length) {
    ESP_ERROR_CHECK(i2c_master_transmit_receive(*dev_handle, control_buffer, control_length ,read_buffer, data_length, -1));

    return ESP_OK;
}

esp_err_t i2c_check(i2c_master_bus_handle_t* bus_handle, uint8_t address) {
    ESP_ERROR_CHECK(i2c_master_probe(*bus_handle, address, -1));

    return ESP_OK;
}


void i2c_wait_idle()
{
    vTaskDelay(pdMS_TO_TICKS(100));
}
