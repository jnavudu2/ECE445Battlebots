#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_types.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_check.h"
#include "driver/ledc.h"
#include "pwm.h"

esp_err_t pwm_init(ledc_timer_t timer, ledc_channel_t channel, uint32_t gpio_pin) {
    esp_err_t error = ESP_OK;

    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_DUTY_RES,
        .timer_num        = timer,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 4 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    error = ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = channel,
        .timer_sel      = timer,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = gpio_pin,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };

    error = ledc_channel_config(&ledc_channel);

    return error;
}