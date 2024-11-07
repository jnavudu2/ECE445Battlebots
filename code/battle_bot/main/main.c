#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_types.h"
#include "i2c_esp.h"
#include "mcf8316a.h"

void app_main(void) {
    i2c_master_bus_handle_t bus_handle;    
    i2c_master_dev_handle_t dev_handle;

    i2c_init_master(&bus_handle);
    i2c_init_device(&bus_handle, &dev_handle, MCF8316A_I2C_SLAVE_ADDR);

    while(1) {
        uint8_t write_buffer[2] = {0x11, 0x22};
        i2c_write(&dev_handle, write_buffer, 2); // should read in the following order: 0x02, 0x11, 0x22
        i2c_wait_idle(1000);

        // uint8_t read_buffer[2] = {0xFF, 0xFF};
        // printf("Read Buffer: %x, %x\n", read_buffer[0], read_buffer[1]);
        // i2c_read(&dev_handle, write_buffer, 2, read_buffer, 2);

        // printf("Read Buffer: %x, %x\n", read_buffer[0], read_buffer[1]);
    }
}