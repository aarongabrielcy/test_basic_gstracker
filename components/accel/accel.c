#include "accel.h"
#include "config.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/gpio.h"

static const char *TAG = "LIS2DH12";

int16_t x, y, z;

static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE, // Habilitamos pull-ups internos por seguridad (además de tus externos)
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    esp_err_t err = i2c_param_config(i2c_master_port, &conf);
    if (err != ESP_OK) {
        return err;
    }
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

/**
 * @brief Escribe un byte en un registro específico
 */
static esp_err_t lis2dh12_write_byte(uint8_t reg_addr, uint8_t data)
{
    uint8_t write_buf[2] = {reg_addr, data};
    return i2c_master_write_to_device(I2C_MASTER_NUM, LIS2DH12_SENSOR_ADDR, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

/**
 * @brief Lee bytes desde un registro
 */
static esp_err_t lis2dh12_read_bytes(uint8_t reg_addr, uint8_t *data, size_t len)
{
    return i2c_master_write_read_device(I2C_MASTER_NUM, LIS2DH12_SENSOR_ADDR, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

void i2c_init(void){
    ESP_ERROR_CHECK(i2c_master_init());

    // 1. Verificar conexión leyendo WHO_AM_I
    uint8_t who_am_i = 0;
    ESP_ERROR_CHECK(lis2dh12_read_bytes(REG_WHO_AM_I, &who_am_i, 1));

    ESP_LOGI(TAG, "WHO_AM_I = 0x%02X", who_am_i);
    if (who_am_i != 0x33) { //
        return;
    }

    // 2. Configurar CTRL_REG1 (0x20)
    // 0x57 -> 0101 0111: ODR 100Hz, Low Power OFF, Ejes Z Y X habilitados
    ESP_ERROR_CHECK(lis2dh12_write_byte(REG_CTRL_REG1, 0x57));

    // 3. Configurar CTRL_REG4 (0x23)
    // 0x88 -> 1000 1000: BDU activado, High Resolution (12-bit) activado
    ESP_ERROR_CHECK(lis2dh12_write_byte(REG_CTRL_REG4, 0x88));
    uint8_t raw_data[6];
    

    while (1) {
        // IMPORTANTE: Usamos (REG_OUT_X_L | 0x80) para activar el auto-incremento de dirección
        // y poder leer los 6 bytes de golpe.
        esp_err_t ret = lis2dh12_read_bytes(REG_OUT_X_L | AUTO_INCREMENT_BIT, raw_data, 6);

        if (ret == ESP_OK) {
            // Convertir bytes a int16_t
            // LIS2DH12 envía parte baja primero, luego parte alta.
            x = (int16_t)((raw_data[1] << 8) | raw_data[0]);
            y = (int16_t)((raw_data[3] << 8) | raw_data[2]);
            z = (int16_t)((raw_data[5] << 8) | raw_data[4]);

            // Ajuste por Alta Resolución (12 bits, justificado a la izquierda)
            // Desplazamos 4 bits a la derecha para obtener el valor real
            x >>= 4;
            y >>= 4;
            z >>= 4;
            // En rango +-2g High Res, 1 unidad ≈ 1 mg
        } else {
            ESP_LOGE(TAG, "Error leyendo datos I2C");
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Esperar 100ms
    }
}

void getAccelValue(int16_t *x_a,int16_t *y_a,int16_t *z_a ){
    *x_a = x;
    *y_a = y;
    *z_a = z;
}
