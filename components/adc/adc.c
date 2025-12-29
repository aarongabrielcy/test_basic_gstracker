#include "adc.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_check.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "ADC"
TaskHandle_t adc_task_handle = NULL;
float voltageBattery_extern;
float voltageBattery_intern;

void adcCallback(void *pvParameters)
{
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc1_handle));

    // 2. Configurar canal (GPIO 5)
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT, // 12 bits (0 - 4095)
        .atten = ADC_ATTEN,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_EXTERNO, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_INTERNO, &config));

    int adc_raw_externo;
    int adc_raw_interno;
    float voltage_pin_externo;
    float voltage_pin_interno;
    float voltage_final_externo;
    float voltage_final_interno;

    while (1) {
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_EXTERNO, &adc_raw_externo));
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_INTERNO, &adc_raw_interno));
        // Nota: Para precisión profesional se usa la librería 'adc_cali', 
        voltage_pin_externo = ((float)adc_raw_externo * ADC_VREF_VOLTAGE) / 4095.0;
        voltage_pin_interno = ((float)adc_raw_interno * ADC_VREF_VOLTAGE) / 4095.0;
        voltage_final_externo = voltage_pin_externo * VOLTAGE_DIVIDER_FACTOR;
        voltage_final_interno = voltage_pin_interno * VOLTAGE_DIVIDER_FACTOR;
        voltageBattery_extern = voltage_final_externo;
        voltageBattery_intern = voltage_final_interno;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

float getBatteryIntern(void){
    return voltageBattery_intern;
}

float getBatteryExtern(void){
    return voltageBattery_extern;
}


void adc_init(){
    xTaskCreate(adcCallback, "adcCallback", 4096, NULL, 10, &adc_task_handle);
}
