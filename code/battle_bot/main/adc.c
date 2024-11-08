#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_types.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_adc/adc_oneshot.h"


esp_err_t adc_init(adc_oneshot_unit_handle_t* adc_handle, uint32_t adc_unit, uint32_t adc_channel) {
    esp_err_t error = ESP_OK;

    adc_oneshot_unit_init_cfg_t unit_config = {
        .unit_id = adc_unit,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };

    adc_oneshot_chan_cfg_t channel_config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };

    error = adc_oneshot_new_unit(&unit_config, adc_handle);

    if (error == ESP_OK) {
        error = adc_oneshot_config_channel(*adc_handle, adc_channel, &channel_config);
    }
    return error;
}