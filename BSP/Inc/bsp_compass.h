/*******************************************************************************
 *      文    件: bsp_compass.h
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 指南针 传感器驱动
 *      特别说明： 指南针数据变量已经定义，具体请参考 bsp_compass.c
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
 * 私有说明：
 *      1. 不说了，嘻嘻嘻嘻！
*******************************************************************************/
#ifndef _BSP_COMPASS_H
#define _BSP_COMPASS_H
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_gpio.h"
/* 寄存器定义 */
#define QMC5883P_CHIPID_REG        0x00
#define QMC5883P_XOUT_L_REG        0x01
#define QMC5883P_XOUT_H_REG        0x02
#define QMC5883P_YOUT_L_REG        0x03
#define QMC5883P_YOUT_H_REG        0x04
#define QMC5883P_ZOUT_L_REG        0x05
#define QMC5883P_ZOUT_H_REG        0x06
#define QMC5883P_STATUS_REG        0x09
#define QMC5883P_CTRL1_REG         0x0A
#define QMC5883P_CTRL2_REG         0x0B

#define COMPASS_I2C_TIMEOUT    1000
typedef struct compass_handle compass_handle_t;
/**
 * @brief 指南针模块状态枚举
 * 
 */
typedef enum 
{
    COMPASS_OK      = 0,
    COMPASS_ERROR   = 1,
} compass_status_t;
/**
 * @brief 指南针数据结构体
 * 
 */
typedef struct 
{
    int16_t x;
    int16_t y;
    int16_t z;
    float angle;
} copmass_data_t;
/**
 * @brief 指南针模块配置结构体
 * 
 */
typedef struct 
{
    /* RCC */
    uint32_t            gpio_sda_rcc;
    uint32_t            gpio_scl_rcc;
    uint32_t            i2c_rcc;
    /* GPIO */
    GPIO_TypeDef*       gpio_sda_port;
    GPIO_TypeDef*       gpio_scl_port;
    uint16_t            gpio_sda_source;
    uint16_t            gpio_scl_source;
    uint8_t             gpio_sda_af;
    uint8_t             gpio_scl_af;
    uint32_t            gpio_sda_pin;
    uint32_t            gpio_scl_pin;
    GPIOMode_TypeDef    gpio_sda_mode;
    GPIOMode_TypeDef    gpio_scl_mode;
    GPIOOType_TypeDef   gpio_sda_otype;
    GPIOOType_TypeDef   gpio_scl_otype;
    GPIOPuPd_TypeDef    gpio_sda_pu;
    GPIOPuPd_TypeDef    gpio_scl_pu;
    GPIOSpeed_TypeDef   gpio_sda_speed;
    GPIOSpeed_TypeDef   gpio_scl_speed;
    /* I2C */
    I2C_TypeDef*        i2c;
    uint16_t            i2c_mode;
    uint16_t            i2c_duty;
    uint16_t            i2c_own_addr;
    uint16_t            i2c_ack;
    uint16_t            i2c_ack_addr;
    uint32_t            i2c_speed;
    /* Base */
    uint8_t             addr;
} compass_cfg_t;
typedef struct
{
    I2C_TypeDef*        i2c;
    uint8_t             addr;
} compass_dev_t;
/**
 * @brief 指南针模块句柄
 * 
 */
struct compass_handle
{
    /* Base */
    copmass_data_t *data;
    uint8_t         id; // 模块ID 0x80 
    /* Dev */
    compass_dev_t dev;
};
compass_status_t bsp_compass_init(compass_handle_t *compass_handle, compass_cfg_t *cfg);
compass_status_t bsp_compass_read(compass_handle_t *compass_handle);
#endif