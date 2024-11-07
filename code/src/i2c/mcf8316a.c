#include "i2c.h"
#include "i2c_master.h"
#include "i2c_types.h"

#define I2C_SLAVE_ADDR          0x01
#define I2C_CLK_SRC_DEFAULT 0
#define TEST_I2C_PORT 0
#define I2C_MASTER_SCL_IO 0
#define I2C_MASTER_SDA_IO 0


i2c_master_bus_handle_t bus_handle;
i2c_master_dev_handle_t dev_handle;


void i2c_init() {
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = TEST_I2C_PORT,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x58,
        .scl_speed_hz = 100000,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));
}