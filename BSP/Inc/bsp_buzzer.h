/*******************************************************************************
 *      文    件: bsp_buzzer.c
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 蜂鸣器 BSP驱动
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#ifndef _BSP_BUZZER_H
#define _BSP_BUZZER_H
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
typedef struct buzzer_handle buzzer_handle_t;
typedef struct 
{
    /* RCC */
    uint32_t            gpio_rcc;
    uint32_t            tim_rcc;

    /* GPIO */
    GPIO_TypeDef*       gpio_port;
    uint32_t            gpio_pin;

    /* TIM */
    TIM_TypeDef*        timx;
    uint16_t            tim_channel;
    uint16_t            prescaler;
    uint16_t            period;
    uint16_t            duty;

} buzzer_cfg_t;

typedef struct 
{
    TIM_TypeDef*        timx;
    uint16_t            period;
    uint16_t            duty;
} buzzer_dev_t;

struct buzzer_handle 
{
    uint16_t           duty;
    uint16_t           period;
    uint8_t            is_open;
    buzzer_dev_t       dev;
};

void bsp_buzzer_init(buzzer_handle_t *buzzer, buzzer_cfg_t *cfg);
void bsp_buzzer_on(buzzer_handle_t *buzzer);
void bsp_buzzer_off(buzzer_handle_t *buzzer);
void bsp_buzzer_trigger(buzzer_handle_t *buzzer);
void bsp_buzzer_set_period(buzzer_handle_t *buzzer, uint16_t period);
void bsp_buzzer_set_duty(buzzer_handle_t *buzzer, uint16_t duty);
void buzzer_play_song(buzzer_handle_t *buzzer);
void buzzer_play_dandelion_interlude(buzzer_handle_t *buzzer);
#endif