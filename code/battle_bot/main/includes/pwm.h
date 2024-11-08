#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_types.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_check.h"
#include "driver/ledc.h"


#define LEDC_MODE               LEDC_HIGH_SPEED_MODE
#define LEDC_FREQUENCY          (4000) // Frequency in Hertz. Set frequency at 4 kHz
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits

esp_err_t pwm_init(ledc_timer_t timer, ledc_channel_t channel, uint32_t gpio_pin);
