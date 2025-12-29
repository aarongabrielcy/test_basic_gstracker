#include "output.h"
#include "driver/gpio.h"
#include "config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_check.h"
#include "esp_log.h"

uint8_t ou = 0;



void configure_gpio_ou(void)
{
    gpio_config_t ou_conf = {};
    ou_conf.intr_type = GPIO_INTR_DISABLE; 
    ou_conf.mode = GPIO_MODE_OUTPUT;      
    ou_conf.pin_bit_mask = (1ULL << OUTPUT1_PIN); 
    ou_conf.pull_down_en = 0;
    ou_conf.pull_up_en = 0;
    gpio_config(&ou_conf);

}

void ou_init(){
    configure_gpio_ou();
}

void OU_on(void){
    if(ou == 0){
        ou ^= 1;
        gpio_set_level(OUTPUT1_PIN, 0); 
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(OUTPUT1_PIN, 1);
    }
}

void OU_off(void){
    if(ou == 1){
        ou ^= 1;
        gpio_set_level(OUTPUT1_PIN, 0); 
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(OUTPUT1_PIN, 1);
    }
}

uint8_t getOUValue(void){
    return ou;
}