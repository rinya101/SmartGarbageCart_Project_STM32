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