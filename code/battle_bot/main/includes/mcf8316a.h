#ifndef MCF8316A_H
#define MCF8316A_H

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_types.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_check.h"

#define MCF8316A_I2C_SLAVE_ADDR           0x01

typedef struct {
    uint8_t mux_sel;
    uint8_t brake_pin;
    uint8_t direction_pin;
    uint8_t feedback_pin;
    uint8_t nFault_pin;
    uint8_t speed_pin;
}mcf8316a_t;

typedef enum {
    drive_motor_1,
    drive_motor_2,
    weapon_motor,
    motor_count
} e_motor;

// initialize array of structs with motor configs
esp_err_t mcf8316a_init();

// configure MCF8316A over I2C (run at initialization)
esp_err_t mcf8316a_configure(i2c_master_dev_handle_t* dev_handle, mcf8316a_t* motor_driver);

// control the speed, braking, and direction of the motor
esp_err_t mcf8316a_control(e_motor motor_index, uint32_t speed, uint32_t brake, uint32_t dir) {
    return ESP_OK;
}

// read the feedback speed and fault status of the motor
esp_err_t mcf8316a_get_status(e_motor motor_index) {
    return ESP_OK;
}

#endif /* MCF8316A_H */