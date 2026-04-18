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

// ===================== 电机默认配置宏 TIM2_CH1_CH2 =====================
#define MOTOR_DEFAULT_CONFIG() (motor_cfg_t){                          \
    /* RCC */                                              \
    .gpio_m1a_rcc        = RCC_AHB1Periph_GPIOA,           \
    .gpio_m1b_rcc        = RCC_AHB1Periph_GPIOB,           \
    .gpio_m2a_rcc        = RCC_AHB1Periph_GPIOA,           \
    .gpio_m2b_rcc        = RCC_AHB1Periph_GPIOB,           \
    .tim_rcc             = RCC_APB1Periph_TIM2,            \
    /* GPIO PORT */                                        \
    .gpio_m1a_port       = GPIOA,                          \
    .gpio_m1b_port       = GPIOB,                          \
    .gpio_m2a_port       = GPIOA,                          \
    .gpio_m2b_port       = GPIOB,                          \
    /* GPIO PIN */                                         \
    .gpio_m1a_pin        = GPIO_Pin_0,                     \
    .gpio_m1b_pin        = GPIO_Pin_10,                    \
    .gpio_m2a_pin        = GPIO_Pin_1,                     \
    .gpio_m2b_pin        = GPIO_Pin_12,                    \
    /* MODE */                                             \
    .gpio_m1a_mode       = GPIO_Mode_AF,                   \
    .gpio_m1b_mode       = GPIO_Mode_OUT,                  \
    .gpio_m2a_mode       = GPIO_Mode_AF,                   \
    .gpio_m2b_mode       = GPIO_Mode_OUT,                  \
    /* OTYPER */                                           \
    .gpio_m1a_otype      = GPIO_OType_PP,                  \
    .gpio_m1b_otype      = GPIO_OType_PP,                  \
    .gpio_m2a_otype      = GPIO_OType_PP,                  \
    .gpio_m2b_otype      = GPIO_OType_PP,                  \
    /* SPEED */                                            \
    .gpio_m1a_speed      = GPIO_Speed_100MHz,              \
    .gpio_m1b_speed      = GPIO_Speed_100MHz,              \
    .gpio_m2a_speed      = GPIO_Speed_100MHz,              \
    .gpio_m2b_speed      = GPIO_Speed_100MHz,              \
    /* PUPD */                                             \
    .gpio_m1a_pu         = GPIO_PuPd_NOPULL,               \
    .gpio_m1b_pu         = GPIO_PuPd_NOPULL,               \
    .gpio_m2a_pu         = GPIO_PuPd_NOPULL,               \
    .gpio_m2b_pu         = GPIO_PuPd_NOPULL,               \
    /* AF */                                               \
    .gpio_m1a_pin_source = GPIO_PinSource0,                \
    .gpio_m2a_pin_source = GPIO_PinSource1,                \
    .gpio_m1a_af         = GPIO_AF_TIM2,                   \
    .gpio_m2a_af         = GPIO_AF_TIM2,                   \
    /* TIM */                                              \
    .tim                 = TIM2,                           \
    .tim_channel1        = TIM_Channel_1,                   \
    .tim_channel2        = TIM_Channel_2,                   \
    .tim_pre             = 100 - 1,                         \
    .tim_period          = 1000 - 1,                      \
    .tim_div             = TIM_CKD_DIV1,                   \
    .tim_mode            = TIM_CounterMode_Up,             \
    .tim_rep             = 0,                              \
}


void bsp_motor_init(motor_handle_t *handle, motor_cfg_t *cfg);
void bsp_motor_set_speed(motor_handle_t *handle, int16_t speed1, int16_t speed2);
void bsp_motor_stop(motor_handle_t *handle);

#endif