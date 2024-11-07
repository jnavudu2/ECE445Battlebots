#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_types.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_check.h"
#include "mcf8316a.h"
#include "mcf8316a_config.h"
#include "i2c_esp.h"

// Pinout Drive Motor 1
#define DRV1_MOTOR_BRK_PIN                CONFIG_GPIO_DRV1_MOTOR_BRK
#define DRV1_MOTOR_DIR_PIN                CONFIG_GPIO_DRV1_MOTOR_DIR
#define DRV1_MOTOR_FG_PIN                 CONFIG_ADC_DRV1_MOTOR_FG
#define DRV1_MOTOR_NFLT_PIN               CONFIG_GPIO_DRV1_MOTOR_NFLT
#define DRV1_MOTOR_SPD_PIN                CONFIG_MCPWM_DRV1_MOTOR_SPD

// Pinout Drive Motor 2
#define DRV2_MOTOR_BRK_PIN                CONFIG_GPIO_DRV2_MOTOR_BRK
#define DRV2_MOTOR_DIR_PIN                CONFIG_GPIO_DRV2_MOTOR_DIR
#define DRV2_MOTOR_FG_PIN                 CONFIG_ADC_DRV2_MOTOR_FG
#define DRV2_MOTOR_NFLT_PIN               CONFIG_GPIO_DRV2_MOTOR_NFLT
#define DRV2_MOTOR_SPD_PIN                CONFIG_MCPWM_DRV2_MOTOR_SPD

// Pinout Weapon Motor
#define WPN_MOTOR_BRK_PIN                 CONFIG_GPIO_WPN_MOTOR_BRK
#define WPN_MOTOR_DIR_PIN                 CONFIG_GPIO_WPN_MOTOR_DIR
#define WPN_MOTOR_FG_PIN                  CONFIG_ADC_WPN_MOTOR_FG
#define WPN_MOTOR_NFLT_PIN                CONFIG_GPIO_WPN_MOTOR_NFLT
#define WPN_MOTOR_SPD_PIN                 CONFIG_MCPWM_WPN_MOTOR_SPD

mcf8316a_t motors[motor_count];

esp_err_t mcf8316a_init() {
    motors[drive_motor_1].mux_sel = (uint8_t) drive_motor_1;
    motors[drive_motor_1].brake_pin = DRV1_MOTOR_BRK_PIN;
    motors[drive_motor_1].direction_pin = DRV1_MOTOR_DIR_PIN;
    motors[drive_motor_1].feedback_pin = DRV1_MOTOR_FG_PIN;
    motors[drive_motor_1].nFault_pin = DRV1_MOTOR_NFLT_PIN;
    motors[drive_motor_1].speed_pin = (uint8_t) DRV1_MOTOR_SPD_PIN;

    motors[drive_motor_2].mux_sel = (uint8_t) drive_motor_2;
    motors[drive_motor_2].brake_pin = (uint8_t) DRV2_MOTOR_BRK_PIN;
    motors[drive_motor_2].direction_pin = (uint8_t) DRV2_MOTOR_DIR_PIN;
    motors[drive_motor_2].feedback_pin = (uint8_t) DRV2_MOTOR_FG_PIN;
    motors[drive_motor_2].nFault_pin = (uint8_t) DRV2_MOTOR_NFLT_PIN;
    motors[drive_motor_2].speed_pin = (uint8_t) DRV2_MOTOR_SPD_PIN;

    motors[weapon_motor].mux_sel = (uint8_t) weapon_motor;
    motors[weapon_motor].brake_pin = (uint8_t) WPN_MOTOR_BRK_PIN;
    motors[weapon_motor].direction_pin = (uint8_t) WPN_MOTOR_DIR_PIN;
    motors[weapon_motor].feedback_pin = (uint8_t) WPN_MOTOR_FG_PIN;
    motors[weapon_motor].nFault_pin = (uint8_t) WPN_MOTOR_NFLT_PIN;
    motors[weapon_motor].speed_pin = (uint8_t) WPN_MOTOR_SPD_PIN;
}

esp_err_t mcf8316a_configure(i2c_master_dev_handle_t* dev_handle, e_motor motor_index) {
    esp_err_t error = ESP_OK;
    e_config_register i;

    if (select_i2c_bus(motors[motor_index].mux_sel) != ESP_OK) {
        error = ESP_ERR_NOT_FINISHED;
    } 
    else 
    {

        for (i = (e_config_register) 0; i < config_count; i++) {
            const uint8_t buffer[CONFIG_BUFFER_LENGTH] = {
                0x00,
                0x00,
                config_addresses[i],
                (uint8_t) ((config_messages[i] >> 24) & 0xFF),
                (uint8_t) ((config_messages[i] >> 16) & 0xFF),
                (uint8_t) ((config_messages[i] >> 8) & 0xFF),
                (uint8_t) (config_messages[i] & 0xFF)
            };

            error = i2c_write(dev_handle, buffer, CONFIG_BUFFER_LENGTH);

            if (error != ESP_OK) {
                break;
            }
        }

    }

    return error;
}

esp_err_t mcf8316a_control(e_motor motor_index, uint32_t speed, uint32_t brake, uint32_t dir) {
    return ESP_OK;
}

esp_err_t mcf8316a_get_status(e_motor motor_index) {
    return ESP_OK;
}
