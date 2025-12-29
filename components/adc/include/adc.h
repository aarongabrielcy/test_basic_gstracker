#pragma once
#include "esp_adc/adc_oneshot.h"


#define ADC_UNIT       ADC_UNIT_1 
#define ADC_CHANNEL_EXTERNO    ADC_CHANNEL_4  // GPIO 5 Bateria externa
#define ADC_CHANNEL_INTERNO    ADC_CHANNEL_3  // GPIO 4 Bateria interna
#define ADC_ATTEN      ADC_ATTEN_DB_12 // mayor rango
#define ADC_VREF_VOLTAGE  3.55
#define VOLTAGE_DIVIDER_FACTOR  11.0

void adc_init(void);
float getBatteryIntern(void);
float getBatteryExtern(void);