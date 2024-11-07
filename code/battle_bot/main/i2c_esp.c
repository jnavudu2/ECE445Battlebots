#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_types.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c_master.h"
#include "i2c_esp.h"
#include "esp_check.h"
#include "driver/gpio.h"
#include "freertos/task.h"

#define GLITCH_IGNORE           7


esp_err_t i2c_init_master(i2c_master_bus_handle_t* bus_handle) {
    esp_err_t error = ESP_OK;

    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,      // clock rate
        .i2c_port = PORT_NUMBER,
        .scl_io_num = I2C_MASTER_SCL_GPIO,
        .sda_io_num = I2C_MASTER_SDA_GPIO,
        .glitch_ignore_cnt = GLITCH_IGNORE,     // default: 7
        .flags.enable_internal_pullup = true,   // default: true
    };

    error = i2c_new_master_bus(&i2c_mst_config, bus_handle);

    return error;
}

esp_err_t i2c_init_device(i2c_master_bus_handle_t* bus_handle, i2c_master_dev_handle_t* dev_handle, uint8_t address) {
    esp_err_t error = ESP_OK;
    
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,  // Addr length of 7
        .device_address = address,
        .scl_speed_hz = MASTER_FREQUENCY,       // 40kHz
    };

    error = i2c_master_bus_add_device(*bus_handle, &dev_cfg, dev_handle);

    return error;
}

esp_err_t i2c_write(i2c_master_dev_handle_t* dev_handle, const uint8_t* write_buffer, size_t data_length) {
    return i2c_master_transmit(*dev_handle, write_buffer, data_length, -1);
}

esp_err_t i2c_check(i2c_master_bus_handle_t* bus_handle, uint8_t address) {
    return i2c_master_probe(*bus_handle, address, -1);
}


void i2c_wait_idle(uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

esp_err_t select_i2c_bus(uint8_t mux_select) {
    esp_err_t error = ESP_OK;

    /*
        Datasheet Reference:
        (SEL_1, SEL_0)
        00 - S1B/S2B
        01 - S1C/S2C
        10 - S1A/S2A
    */

    // 00
    if (mux_select == 0) {
        gpio_set_level(I2C_GPIO_MUX_SEL_1, 0);
        gpio_set_level(I2C_GPIO_MUX_SEL_0, 0);
    } 
    // 01 
    else if (mux_select == 1) {
        gpio_set_level(I2C_GPIO_MUX_SEL_1, 0);
        gpio_set_level(I2C_GPIO_MUX_SEL_0, 1);
    }
    // 10
    else if (mux_select == 2) {
        gpio_set_level(I2C_GPIO_MUX_SEL_1, 1);
        gpio_set_level(I2C_GPIO_MUX_SEL_0, 0);
    } else {
        error = ESP_ERR_NOT_SUPPORTED;
    }

    return error;
}
