#pragma once

// --- CONFIGURACIÓN DE PINES Y DIRECCIÓN ---
#define I2C_MASTER_NUM              0       
#define I2C_MASTER_FREQ_HZ          100000  
#define I2C_MASTER_TX_BUF_DISABLE   0       
#define I2C_MASTER_RX_BUF_DISABLE   0       
#define I2C_MASTER_TIMEOUT_MS       1000
// dirección
#define LIS2DH12_SENSOR_ADDR        0x19
#define REG_WHO_AM_I                0x0F
#define REG_CTRL_REG1               0x20
#define REG_CTRL_REG4               0x23
#define REG_OUT_X_L                 0x28
#define REG_STATUS                  0x27
#define AUTO_INCREMENT_BIT          0x80 

void i2c_init();
void getAccelValue();
