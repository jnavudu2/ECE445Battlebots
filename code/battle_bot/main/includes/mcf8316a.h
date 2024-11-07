#ifndef MCF8316A_H
#define MCF8316A_H

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_types.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_check.h"

// ISD_CONFIG register
#define ISD_CONFIG_ADDR        0x00000080  // Address
#define ISD_CONFIG_MSG         0x64738C20  // Recommended Value

// REV_DRIVE_CONFIG register
#define REV_DRIVE_CONFIG_ADDR  0x00000082  // Address
#define REV_DRIVE_CONFIG_MSG   0x02B20000  // Recommended Value

// MOTOR_STARTUP1 register
#define MOTOR_STARTUP1_ADDR    0x00000084  // Address
#define MOTOR_STARTUP1_MSG     0x0B6807D0  // Recommended Value

// MOTOR_STARTUP2 register
#define MOTOR_STARTUP2_ADDR    0x00000086  // Address
#define MOTOR_STARTUP2_MSG     0x2306600C  // Recommended Value

// CLOSED_LOOP1 register
#define CLOSED_LOOP1_ADDR      0x00000088  // Address
#define CLOSED_LOOP1_MSG       0x0D3201B5  // Recommended Value

// CLOSED_LOOP2 register
#define CLOSED_LOOP2_ADDR      0x0000008A  // Address
#define CLOSED_LOOP2_MSG       0x1BAD0000  // Recommended Value

// CLOSED_LOOP3 register
#define CLOSED_LOOP3_ADDR      0x0000008C  // Address
#define CLOSED_LOOP3_MSG       0x00000000  // Recommended Value

// CLOSED_LOOP4 register
#define CLOSED_LOOP4_ADDR      0x0000008E  // Address
#define CLOSED_LOOP4_MSG       0x00000000  // Recommended Value

// SPEED_PROFILES1 register
#define SPEED_PROFILES1_ADDR   0x00000094  // Address
#define SPEED_PROFILES1_MSG    0x00000000  // Recommended Value

// SPEED_PROFILES2 register
#define SPEED_PROFILES2_ADDR   0x00000096  // Address
#define SPEED_PROFILES2_MSG    0x00000000  // Recommended Value

// SPEED_PROFILES3 register
#define SPEED_PROFILES3_ADDR   0x00000098  // Address
#define SPEED_PROFILES3_MSG    0x00000000  // Recommended Value

// SPEED_PROFILES4 register
#define SPEED_PROFILES4_ADDR   0x0000009A  // Address
#define SPEED_PROFILES4_MSG    0x000D000D  // Recommended Value

// SPEED_PROFILES5 register
#define SPEED_PROFILES5_ADDR   0x0000009C  // Address
#define SPEED_PROFILES5_MSG    0xEEC80106  // Recommended Value

// SPEED_PROFILES6 register
#define SPEED_PROFILES6_ADDR   0x0000009E  // Address
#define SPEED_PROFILES6_MSG    0xEEC80106  // Recommended Value

// FAULT_CONFIG1 register
#define FAULT_CONFIG1_ADDR     0x00000090  // Address
#define FAULT_CONFIG1_MSG      0x3EBC80106 // Recommended Value

// FAULT_CONFIG2 register
#define FAULT_CONFIG2_ADDR     0x00000092  // Address
#define FAULT_CONFIG2_MSG      0x07D00888   // Recommended Value

// PIN_CONFIG register 
#define PIN_CONFIG_ADDR        0x000000A4   // Address 
#define PIN_CONFIG_MSG         0x00000000   // Recommended Value 

// DEVICE_CONFIG1 register 
#define DEVICE_CONFIG1_ADDR    0x000000A6   // Address 
#define DEVICE_CONFIG1_MSG     0x00101462   // Recommended Value 

// DEVICE_CONFIG2 register 
#define DEVICE_CONFIG2_ADDR    0x000000A8   // Address 
#define DEVICE_CONFIG2_MSG     0x4000F00F   // Recommended Value 

// PERI_CONFIG1 register 
#define PERI_CONFIG1_ADDR      0x000000AA   // Address 
#define PERI_CONFIG1_MSG       0x41C01F00   // Recommended Value 

// GD_CONFIG1 register 
#define GD_CONFIG1_ADDR        0x000000AC   // Address 
#define GD_CONFIG1_MSG         0x1C450100   // Recommended Value 

// GD_CONFIG2 register 
#define GD_CONFIG2_ADDR        0x000000AE   // Address 
#define GD_CONFIG2_MSG         0x00200000   // Recommended Value 

// INT_ALGO_1 register 
#define INT_ALGO_1_ADDR        0x000000A0   // Address 
#define INT_ALGO_1_MSG         0x2433407D   // Recommended Value 

// INT_ALGO_2 register 
#define INT_ALGO_2_ADDR        0x000000A2   // Address 
#define INT_ALGO_2_MSG         0x00101A7    // Recommended Value 

#define I2C_SLAVE_ADDRESS           0x01


#endif /* MCF8316A_H */