/*******************************************************************************
 *      文    件: bsp_servo.c
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 舵机 BSP驱动
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#ifndef _BSP_SERVO_H
#define _BSP_SERVO_H

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

typedef struct servo_handle servo_handle_t;

/**
 * @brief 舵机配置
 */
typedef struct
{
    /* RCC */
    uint32_t        gpio_clk;
    uint32_t        tim_clk;
    /* GPIO */
    GPIO_TypeDef*   gpio_port;
    uint32_t        gpio_pin;
    uint8_t         gpio_af_src;
    uint16_t        gpio_af;     
    /* TIM */
    TIM_TypeDef*    tim;
    uint16_t        tim_prescaler;
    uint32_t        tim_period;
    /* PWM */
    uint16_t        channel;
    uint32_t        duty;
} servo_cfg_t;

/**
 * @brief 舵机设备
 */
typedef struct
{
    TIM_TypeDef*    tim;
    uint16_t        channel;
} servo_dev_t;

/**
 * @brief 舵机句柄
 */
struct servo_handle
{
    float         angle;
    servo_dev_t   dev;
};

void bsp_servo_init(servo_handle_t *handle, servo_cfg_t *cfg);
void bsp_servo_set_angle(servo_handle_t *handle, float angle);
void bsp_servo_set_us(servo_handle_t *handle, uint16_t us);
void bsp_servo_get_angle(servo_handle_t *handle, float *angle);

#endif /* _BSP_SERVO_H */