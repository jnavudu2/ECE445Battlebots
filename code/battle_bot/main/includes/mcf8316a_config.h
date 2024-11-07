#ifndef MCF8316A_CONFIG_H
#define MCF8316A_CONFIG_H

#include <string.h>
#include <stdio.h>
#include "esp_types.h"

// ISD_CONFIG register
#define ISD_CONFIG_ADDR        0x80  // Address
#define ISD_CONFIG_MSG         0x64738C20  // Recommended Value

// REV_DRIVE_CONFIG register
#define REV_DRIVE_CONFIG_ADDR  0x82  // Address
#define REV_DRIVE_CONFIG_MSG   0x02B20000  // Recommended Value

// MOTOR_STARTUP1 register
#define MOTOR_STARTUP1_ADDR    0x84  // Address
#define MOTOR_STARTUP1_MSG     0x0B6807D0  // Recommended Value

// MOTOR_STARTUP2 register
#define MOTOR_STARTUP2_ADDR    0x86  // Address
#define MOTOR_STARTUP2_MSG     0x2306600C  // Recommended Value

// CLOSED_LOOP1 register
#define CLOSED_LOOP1_ADDR      0x88  // Address
#define CLOSED_LOOP1_MSG       0x0D3201B5  // Recommended Value

// CLOSED_LOOP2 register
#define CLOSED_LOOP2_ADDR      0x8A  // Address
#define CLOSED_LOOP2_MSG       0x1BAD0000  // Recommended Value

// CLOSED_LOOP3 register
#define CLOSED_LOOP3_ADDR      0x8C  // Address
#define CLOSED_LOOP3_MSG       0x00000000  // Recommended Value

// CLOSED_LOOP4 register
#define CLOSED_LOOP4_ADDR      0x8E  // Address
#define CLOSED_LOOP4_MSG       0x00000000  // Recommended Value

// SPEED_PROFILES1 register
#define SPEED_PROFILES1_ADDR   0x94  // Address
#define SPEED_PROFILES1_MSG    0x00000000  // Recommended Value

// SPEED_PROFILES2 register
#define SPEED_PROFILES2_ADDR   0x96  // Address
#define SPEED_PROFILES2_MSG    0x00000000  // Recommended Value

// SPEED_PROFILES3 register
#define SPEED_PROFILES3_ADDR   0x98  // Address
#define SPEED_PROFILES3_MSG    0x00000000  // Recommended Value

// SPEED_PROFILES4 register
#define SPEED_PROFILES4_ADDR   0x9A  // Address
#define SPEED_PROFILES4_MSG    0x000D000D  // Recommended Value

// SPEED_PROFILES5 register
#define SPEED_PROFILES5_ADDR   0x9C  // Address
#define SPEED_PROFILES5_MSG    0xEEC80106  // Recommended Value

// SPEED_PROFILES6 register
#define SPEED_PROFILES6_ADDR   0x9E  // Address
#define SPEED_PROFILES6_MSG    0xEEC80106  // Recommended Value

// FAULT_CONFIG1 register
#define FAULT_CONFIG1_ADDR     0x90  // Address
#define FAULT_CONFIG1_MSG      0x3EBC80106 // Recommended Value

// FAULT_CONFIG2 register
#define FAULT_CONFIG2_ADDR     0x92  // Address
#define FAULT_CONFIG2_MSG      0x07D00888   // Recommended Value

// PIN_CONFIG register 
#define PIN_CONFIG_ADDR        0xA4   // Address 
#define PIN_CONFIG_MSG         0x00000000   // Recommended Value 

// DEVICE_CONFIG1 register 
#define DEVICE_CONFIG1_ADDR    0xA6   // Address 
#define DEVICE_CONFIG1_MSG     0x00101462   // Recommended Value 

// DEVICE_CONFIG2 register 
#define DEVICE_CONFIG2_ADDR    0xA8   // Address 
#define DEVICE_CONFIG2_MSG     0x4000F00F   // Recommended Value 

// PERI_CONFIG1 register 
#define PERI_CONFIG1_ADDR      0xAA   // Address 
#define PERI_CONFIG1_MSG       0x41C01F00   // Recommended Value 

// GD_CONFIG1 register 
#define GD_CONFIG1_ADDR        0xAC   // Address 
#define GD_CONFIG1_MSG         0x1C450100   // Recommended Value 

// GD_CONFIG2 register 
#define GD_CONFIG2_ADDR        0xAE   // Address 
#define GD_CONFIG2_MSG         0x00200000   // Recommended Value 

// INT_ALGO_1 register 
#define INT_ALGO_1_ADDR        0xA0   // Address 
#define INT_ALGO_1_MSG         0x2433407D   // Recommended Value 

// INT_ALGO_2 register 
#define INT_ALGO_2_ADDR        0xA2   // Address 
#define INT_ALGO_2_MSG         0x00101A7    // Recommended Value

#define CONFIG_BUFFER_LENGTH              7

const uint8_t config_addresses[config_count] = {
    ISD_CONFIG_ADDR,     
    REV_DRIVE_CONFIG_ADDR,
    MOTOR_STARTUP1_ADDR, 
    MOTOR_STARTUP2_ADDR, 
    CLOSED_LOOP1_ADDR,   
    CLOSED_LOOP2_ADDR,   
    CLOSED_LOOP3_ADDR,   
    CLOSED_LOOP4_ADDR,   
    SPEED_PROFILES1_ADDR,
    SPEED_PROFILES2_ADDR,
    SPEED_PROFILES3_ADDR,
    SPEED_PROFILES4_ADDR,
    SPEED_PROFILES5_ADDR,
    SPEED_PROFILES6_ADDR,
    FAULT_CONFIG1_ADDR,  
    FAULT_CONFIG2_ADDR,  
    PIN_CONFIG_ADDR,     
    DEVICE_CONFIG1_ADDR, 
    DEVICE_CONFIG2_ADDR, 
    PERI_CONFIG1_ADDR,   
    GD_CONFIG1_ADDR,     
    GD_CONFIG2_ADDR,     
    INT_ALGO_1_ADDR,     
    INT_ALGO_2_ADDR      
};

const uint32_t config_messages[config_count] = {
    ISD_CONFIG_MSG,      
    REV_DRIVE_CONFIG_MSG,
    MOTOR_STARTUP1_MSG,  
    MOTOR_STARTUP2_MSG,  
    CLOSED_LOOP1_MSG,    
    CLOSED_LOOP2_MSG,    
    CLOSED_LOOP3_MSG,    
    CLOSED_LOOP4_MSG,    
    SPEED_PROFILES1_MSG, 
    SPEED_PROFILES2_MSG, 
    SPEED_PROFILES3_MSG, 
    SPEED_PROFILES4_MSG, 
    SPEED_PROFILES5_MSG, 
    SPEED_PROFILES6_MSG, 
    FAULT_CONFIG1_MSG,   
    FAULT_CONFIG2_MSG,   
    PIN_CONFIG_MSG,      
    DEVICE_CONFIG1_MSG,  
    DEVICE_CONFIG2_MSG,  
    PERI_CONFIG1_MSG,    
    GD_CONFIG1_MSG,      
    GD_CONFIG2_MSG,      
    INT_ALGO_1_MSG,      
    INT_ALGO_2_MSG       
};

typedef enum {
    isd_config,
    rev_drive_config,
    motor_startup1,
    motor_startup2,
    closed_loop1,
    closed_loop2,
    closed_loop3,
    closed_loop4,
    speed_profiles1,
    speed_profiles2,
    speed_profiles3,
    speed_profiles4,
    speed_profiles5,
    speed_profiles6,
    fault_config1,
    fault_config2,
    pin_config,
    device_config1,
    device_config2,
    peri_config1,
    gd_config1,
    gd_config2,
    int_algo_1,
    int_algo_2,
    config_count // Number of configurations
} e_config_register;


#endif /* MCF8316A_CONFIG_H */