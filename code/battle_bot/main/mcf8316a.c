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
#include "esp_adc/adc_oneshot.h"
#include "adc.h"
#include "pwm.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "math.h"


// Pinout Drive Motor 1
#define DRVL_MOTOR_BRK_PIN                CONFIG_GPIO_DRVL_MOTOR_BRK
#define DRVL_MOTOR_DIR_PIN                CONFIG_GPIO_DRVL_MOTOR_DIR
#define DRVL_MOTOR_SPD_PIN                CONFIG_MCPWM_DRVL_MOTOR_SPD

#define DRVL_MOTOR_ADC_UNIT               ADC_UNIT_1
#define DRVL_MOTOR_ADC_CHAN               ADC_CHANNEL_2

#define DRVL_PWM_TIMER                    LEDC_TIMER_0
#define DRVL_PWM_GPIO                     CONFIG_GPIO_DRVL_PWM
#define DRVL_PWM_CHANNEL                  LEDC_CHANNEL_0

// Pinout Drive Motor 2
#define DRVR_MOTOR_BRK_PIN                CONFIG_GPIO_DRVR_MOTOR_BRK
#define DRVR_MOTOR_DIR_PIN                CONFIG_GPIO_DRVR_MOTOR_DIR
#define DRVR_MOTOR_SPD_PIN                CONFIG_MCPWM_DRVR_MOTOR_SPD

#define DRVR_MOTOR_ADC_UNIT               ADC_UNIT_1
#define DRVR_MOTOR_ADC_CHAN               ADC_CHANNEL_3

#define DRVR_PWM_TIMER                    LEDC_TIMER_1
#define DRVR_PWM_GPIO                     CONFIG_GPIO_DRVR_PWM
#define DRVR_PWM_CHANNEL                  LEDC_CHANNEL_1

// Pinout Weapon Motor
#define WPN_MOTOR_BRK_PIN                 CONFIG_GPIO_WPN_MOTOR_BRK
#define WPN_MOTOR_DIR_PIN                 CONFIG_GPIO_WPN_MOTOR_DIR
#define WPN_MOTOR_SPD_PIN                 CONFIG_MCPWM_WPN_MOTOR_SPD

#define WPN_MOTOR_ADC_UNIT                ADC_UNIT_1
#define WPN_MOTOR_ADC_CHAN                ADC_CHANNEL_4

#define WPN_PWM_TIMER                     LEDC_TIMER_2
#define WPN_PWM_GPIO                      CONFIG_GPIO_WPN_PWM
#define WPN_PWM_CHANNEL                   LEDC_CHANNEL_2


mcf8316a_pinout_t motor_pinouts[motor_count];
mcf8316a_t motor_status[motor_count];

const uint32_t brake_pins[motor_count] = {DRVL_MOTOR_BRK_PIN, DRVR_MOTOR_BRK_PIN, WPN_MOTOR_BRK_PIN};
const uint32_t dir_pins[motor_count] = {DRVL_MOTOR_DIR_PIN, DRVR_MOTOR_DIR_PIN, WPN_MOTOR_DIR_PIN};
const adc_channel_t adc_channels[motor_count] = {DRVL_MOTOR_ADC_CHAN, DRVR_MOTOR_ADC_CHAN, WPN_MOTOR_ADC_CHAN};
const adc_unit_t adc_units[motor_count] = {DRVL_MOTOR_ADC_UNIT, DRVR_MOTOR_ADC_UNIT, WPN_MOTOR_ADC_UNIT};
const ledc_timer_t pwm_timers[motor_count] = {DRVL_PWM_TIMER, DRVR_PWM_TIMER, WPN_PWM_TIMER};
const ledc_channel_t pwm_channels[motor_count] = {DRVL_PWM_CHANNEL, DRVR_PWM_CHANNEL, WPN_PWM_CHANNEL};
const uint32_t pwm_pins[motor_count] = {DRVL_PWM_GPIO, DRVR_PWM_GPIO, WPN_PWM_GPIO};

esp_err_t mcf8316a_init() {
    esp_err_t error = ESP_OK;
    e_motor i;

    for (i = (e_motor) 0; i < motor_count; i++) {
        motor_pinouts[i].mux_sel = (uint8_t) i;
        motor_pinouts[i].brake_pin = brake_pins[i];
        motor_pinouts[i].direction_pin = dir_pins[i];
        error = adc_init(&motor_pinouts[i].adc_feedback, adc_units[i], adc_channels[i]);

        error = pwm_init(pwm_timers[i], pwm_channels[i], pwm_pins[i]);

        motor_status[i].brake_status = 0;
        motor_status[i].direction = 1;
        motor_status[i].feedback_speed = 0;
        motor_status[i].speed = 0;
    }

    return error;
}

esp_err_t mcf8316a_configure(i2c_master_dev_handle_t* dev_handle, e_motor motor_index) {
    esp_err_t error = ESP_OK;
    e_config_register i;

    if (select_i2c_bus(motor_pinouts[motor_index].mux_sel) != ESP_OK) {
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

esp_err_t mcf8316a_control(e_motor motor_index) {
    esp_err_t error = ESP_OK;

    gpio_set_level(motor_pinouts[motor_index].direction_pin, motor_status[motor_index].direction);
    gpio_set_level(motor_pinouts[motor_index].brake_pin, motor_status[motor_index].brake_status);

    uint32_t duty = (motor_status[motor_index].speed / 100) * pow(2, LEDC_DUTY_RES);
    error = ledc_set_duty(LEDC_MODE, pwm_channels[motor_index], duty);
    error = ledc_update_duty(LEDC_MODE, pwm_channels[motor_index]);

    return error;
}

esp_err_t mcf8316a_get_status(e_motor motor_index) {
    esp_err_t error = ESP_OK;

    uint32_t raw_adc = 0;
    error = adc_oneshot_read(motor_pinouts[motor_index].adc_feedback, adc_channels[motor_index], &raw_adc);
    motor_status[motor_index].feedback_speed = raw_adc;

    return error;
}
