#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_types.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_adc/adc_oneshot.h"

// initialize the unit and channel for an ADC
esp_err_t adc_init(adc_oneshot_unit_handle_t* adc_handle, uint32_t adc_unit, uint32_t adc_channel);