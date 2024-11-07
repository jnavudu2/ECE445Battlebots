#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_types.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c_master.h"
#include "i2c_esp.h"
#include "mcf8316a.h"
#include "esp_check.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void) {
    i2c_master_bus_handle_t bus_handle;    
    i2c_master_dev_handle_t dev_handle;

    i2c_init_master(&bus_handle);
    i2c_init_device(&bus_handle, &dev_handle, I2C_SLAVE_ADDRESS);

    while(1) {
        uint8_t write_buffer[2] = {0x11, 0x22};
        i2c_write(&dev_handle, write_buffer, 2);
        i2c_wait_idle();

        uint8_t read_buffer[2] = {0xFF, 0xFF};
        printf("Read Buffer: %x, %x\n", read_buffer[0], read_buffer[1]);
        i2c_read(&dev_handle, write_buffer, 2, read_buffer, 2);

        printf("Read Buffer: %x, %x\n", read_buffer[0], read_buffer[1]);
    }
}