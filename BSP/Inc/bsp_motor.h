#ifndef _BSP_MOTOR_H
#define _BSP_MOTOR_H

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

typedef struct motor_handle motor_handle_t;

/**
 * @brief motor cfg
 */
typedef struct
{
    /* RCC */
    uint32_t            gpio_m1a_rcc;
    uint32_t            gpio_m1b_rcc;
    uint32_t            gpio_m2a_rcc;
    uint32_t            gpio_m2b_rcc;
    uint32_t            tim_rcc;

    /* GPIO */
    GPIO_TypeDef*       gpio_m1a_port;
    GPIO_TypeDef*       gpio_m1b_port;
    GPIO_TypeDef*       gpio_m2a_port;
    GPIO_TypeDef*       gpio_m2b_port;

    uint32_t            gpio_m1a_pin;
    uint32_t            gpio_m1b_pin;
    uint32_t            gpio_m2a_pin;
    uint32_t            gpio_m2b_pin;

    GPIOMode_TypeDef    gpio_m1a_mode;
    GPIOMode_TypeDef    gpio_m1b_mode;
    GPIOMode_TypeDef    gpio_m2a_mode;
    GPIOMode_TypeDef    gpio_m2b_mode;

    GPIOOType_TypeDef   gpio_m1a_otype;
    GPIOOType_TypeDef   gpio_m1b_otype;
    GPIOOType_TypeDef   gpio_m2a_otype;
    GPIOOType_TypeDef   gpio_m2b_otype;

    GPIOSpeed_TypeDef   gpio_m1a_speed;
    GPIOSpeed_TypeDef   gpio_m1b_speed;
    GPIOSpeed_TypeDef   gpio_m2a_speed;
    GPIOSpeed_TypeDef   gpio_m2b_speed;

    GPIOPuPd_TypeDef    gpio_m1a_pu;
    GPIOPuPd_TypeDef    gpio_m1b_pu;
    GPIOPuPd_TypeDef    gpio_m2a_pu;
    GPIOPuPd_TypeDef    gpio_m2b_pu;

    uint16_t            gpio_m1a_pin_source;
    uint16_t            gpio_m2a_pin_source;

    uint8_t             gpio_m1a_af;
    uint8_t             gpio_m2a_af;

    /* TIM */
    TIM_TypeDef*        tim;
    uint16_t            tim_channel1;
    uint16_t            tim_channel2;
    uint16_t            tim_pre;
    uint32_t            tim_period;
    uint16_t            tim_div;
    uint16_t            tim_mode;
    uint8_t             tim_rep;

} motor_cfg_t;

/**
 * @brief motor dev
 */
typedef struct
{
    TIM_TypeDef*        tim;
    uint16_t            tim_channel1;
    uint16_t            tim_channel2;
    uint32_t            period;
    GPIO_TypeDef*       gpio_m1a_port;
    GPIO_TypeDef*       gpio_m1b_port;
    GPIO_TypeDef*       gpio_m2a_port;
    GPIO_TypeDef*       gpio_m2b_port;
    uint32_t            gpio_m1a_pin;
    uint32_t            gpio_m1b_pin;
    uint32_t            gpio_m2a_pin;
    uint32_t            gpio_m2b_pin;

} motor_dev_t;

/**
 * @brief motor handle
 */
struct motor_handle
{
    motor_dev_t         dev;
    int16_t             speed1;
    int16_t             speed2;
};


void bsp_motor_init(motor_handle_t *handle, motor_cfg_t *cfg);
void bsp_motor_set_speed(motor_handle_t *handle, int16_t speed1, int16_t speed2);
void bsp_motor_stop(motor_handle_t *handle);

#endif