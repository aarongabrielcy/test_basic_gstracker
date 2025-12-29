#include "input.h"
#include "config.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_check.h"
#include "esp_log.h"

#define ESP_INTR_FLAG_DEFAULT 0
#define TAG "INPUT"
TaskHandle_t input_task = NULL;


static void IRAM_ATTR setValue(void *args){
    //hacer algo
}

static void IRAM_ATTR setValue_ign(void *args){
    //hacer algo
}

void configure_gpio_input(void *pvParameters){
    gpio_config_t in_conf = {};
    in_conf.intr_type = GPIO_INTR_NEGEDGE; 
    in_conf.mode = GPIO_MODE_INPUT;      
    in_conf.pin_bit_mask = (1ULL << INPUT2_PIN); 
    in_conf.pull_down_en = 0;
    in_conf.pull_up_en = 0;
    gpio_config(&in_conf);
    
    gpio_config_t ign_conf = {};
    ign_conf.intr_type = GPIO_INTR_POSEDGE; 
    ign_conf.mode = GPIO_MODE_INPUT;      
    ign_conf.pin_bit_mask = (1ULL << IGNITION_PIN); 
    ign_conf.pull_down_en = 0;
    ign_conf.pull_up_en = 0;
    gpio_config(&ign_conf);
    
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(IGNITION_PIN, setValue_ign, NULL);
    gpio_isr_handler_add(INPUT2_PIN, setValue, NULL);
    while(1){
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void input_init(){
    xTaskCreate(configure_gpio_input, "input_task", 4096, NULL, 10, &input_task);
}

uint8_t getInValue(){
    return gpio_get_level(INPUT2_PIN);
}

uint8_t getIGNValue(){
    return gpio_get_level(IGNITION_PIN);
}