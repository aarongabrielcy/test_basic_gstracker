/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "pwrmanage.h"
#include "main_uart.h"
#include "monitor.h"
#include "gnss_uart.h"
#include "adc.h"
#include "input.h"
#include "output.h"
#include "accel.h"

void app_main(void)
{
    pwr_init();
    main_uart_init();
    monitor_uart_init();
    uart_init_2();
    adc_init();
    input_init();
    ou_init();
    i2c_init();
}
