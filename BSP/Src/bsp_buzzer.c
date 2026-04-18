/*******************************************************************************
 *      文    件: bsp_buzzer.c
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 蜂鸣器 BSP驱动
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#include "bsp_buzzer.h"
#include "FreeRTOS.h"
#include "task.h"
#ifndef NULL
#define NULL (void*)0
#endif
// ================== C 调 完整音阶 ==================
// 低音组 (_1 ~ _7)
#define L1  68182   // 低音1
#define L2  60606   // 低音2
#define L3  54054   // 低音3
#define L4  51020   // 低音4
#define L5  45455   // 低音5
#define L6  40404   // 低音6
#define L7  36364   // 低音7

// 中音组 (1 ~ 7)
#define M1  34091   // 中音1
#define M2  30303   // 中音2
#define M3  27027   // 中音3
#define M4  25510   // 中音4
#define M5  22727   // 中音5
#define M6  20243   // 中音6
#define M7  18050   // 中音7

// 高音组 (1_ ~ 7_)
#define H1  17065   // 高音1
#define H2  15175   // 高音2
#define H3  13514   // 高音3
#define H4  12755   // 高音4
#define H5  11364   // 高音5
#define H6  10121   // 高音6
#define H7  9091    // 高音7


static void buzzer_gpio_init(buzzer_cfg_t *cfg)
{
    RCC_AHB1PeriphClockCmd(cfg->gpio_rcc, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = cfg->gpio_pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_Init(cfg->gpio_port, &GPIO_InitStructure);
    GPIO_PinAFConfig(cfg->gpio_port, GPIO_PinSource9, GPIO_AF_TIM4);
}

static void buzzer_tim_pwm_init(buzzer_cfg_t *cfg)
{
    RCC_APB1PeriphClockCmd(cfg->tim_rcc, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    TIM_InitStructure.TIM_Prescaler     = cfg->prescaler;
    TIM_InitStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_InitStructure.TIM_Period        = cfg->period;
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(cfg->timx, &TIM_InitStructure);

    TIM_OCInitTypeDef TIM_OC_InitStructure;
    TIM_OCStructInit(&TIM_OC_InitStructure);

    TIM_OC_InitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
    TIM_OC_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC_InitStructure.TIM_Pulse       = cfg->duty;

    if (cfg->tim_channel == TIM_Channel_4) 
    {
        TIM_OC4Init(cfg->timx, &TIM_OC_InitStructure);
        TIM_OC4PreloadConfig(cfg->timx, TIM_OCPreload_Enable);
    }

    TIM_ARRPreloadConfig(cfg->timx, ENABLE);
    TIM_Cmd(cfg->timx, ENABLE);
}
/**
 * @brief 蜂鸣器初始化
 * 
 * @param buzzer 
 * @param cfg 
 */
void bsp_buzzer_init(buzzer_handle_t *buzzer, buzzer_cfg_t *cfg)
{
    if (buzzer == NULL || cfg == NULL) return;

    buzzer->dev.timx    = cfg->timx;
    buzzer->dev.period  = cfg->period;
    buzzer->dev.duty    = cfg->duty;
    buzzer->period      = cfg->period;
    buzzer->duty        = cfg->duty;
    buzzer->is_open     = 0;

    buzzer_gpio_init(cfg);
    buzzer_tim_pwm_init(cfg);

    bsp_buzzer_off(buzzer);
}
/**
 * @brief 蜂鸣器打开
 * 
 * @param buzzer 
 */
void bsp_buzzer_on(buzzer_handle_t *buzzer)
{
    if (!buzzer) return;

    buzzer->is_open = 1;
    TIM_SetCompare4(buzzer->dev.timx, buzzer->duty);
}
/**
 * @brief 蜂鸣器关闭
 * 
 * @param buzzer 
 */
void bsp_buzzer_off(buzzer_handle_t *buzzer)
{
    if (!buzzer) return;

    buzzer->is_open = 0;
    TIM_SetCompare4(buzzer->dev.timx, buzzer->dev.period);
}

void bsp_buzzer_trigger(buzzer_handle_t *buzzer)
{
    if (buzzer->is_open)
    {
        bsp_buzzer_off(buzzer);
    }
    else
    {
        bsp_buzzer_on(buzzer);
    }
}

/**
 * @brief 设置蜂鸣器周期
 * @note uint16_t -> (0, 65535)
 * @param buzzer 
 * @param period 
 */
void bsp_buzzer_set_period(buzzer_handle_t *buzzer, uint16_t period)
{
    if (buzzer == NULL) return;

    buzzer->period = period;
    buzzer->dev.period = period;

    TIM_SetAutoreload(buzzer->dev.timx, period);
}
/**
 * @brief 设置占空比
 * @note uint16_t -> (0, 65535)
 * @param buzzer 
 * @param duty 
 */
void bsp_buzzer_set_duty(buzzer_handle_t *buzzer, uint16_t duty)
{
    if (buzzer == NULL) return;

    buzzer->duty = duty;
    buzzer->dev.duty = duty;

    if(buzzer->is_open){
        TIM_SetCompare4(buzzer->dev.timx, duty);
    }
}
/**
 * @brief 蒲公英的约定
 * 
 * @param buzzer 
 */
void buzzer_play_dandelion_interlude(buzzer_handle_t *buzzer)
{
    /* 第一小节 */
    bsp_buzzer_set_period(buzzer, M3);bsp_buzzer_on(buzzer);vTaskDelay(400);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 3 */
    bsp_buzzer_set_period(buzzer, M5);bsp_buzzer_on(buzzer);vTaskDelay(400);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 5 */
    bsp_buzzer_set_period(buzzer, H1);bsp_buzzer_on(buzzer);vTaskDelay(400);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 1(H) */
    bsp_buzzer_set_period(buzzer, H3);bsp_buzzer_on(buzzer);vTaskDelay(400);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 3(H) */
    /* 第二小节 */
    bsp_buzzer_set_period(buzzer, H3);bsp_buzzer_on(buzzer);vTaskDelay(440+440+440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 3(H) */
    bsp_buzzer_set_period(buzzer, H4);bsp_buzzer_on(buzzer);vTaskDelay(440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);
    bsp_buzzer_set_period(buzzer, H2);bsp_buzzer_on(buzzer);vTaskDelay(440+440+440+440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 2(H) */
    /* 第三小节 */
    bsp_buzzer_set_period(buzzer, H2);bsp_buzzer_on(buzzer);vTaskDelay(440+440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 2(H) */
    bsp_buzzer_set_period(buzzer, H5);bsp_buzzer_on(buzzer);vTaskDelay(440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 5(H) */
    bsp_buzzer_set_period(buzzer, M7);bsp_buzzer_on(buzzer);vTaskDelay(440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 7*/
    bsp_buzzer_set_period(buzzer, H1);bsp_buzzer_on(buzzer);vTaskDelay(440+440+440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 2(H) */
    /* 第四小节 */
    bsp_buzzer_set_period(buzzer, H3);bsp_buzzer_on(buzzer);vTaskDelay(440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 3(H) */
    bsp_buzzer_set_period(buzzer, H4);bsp_buzzer_on(buzzer);vTaskDelay(440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 4(H) */
    bsp_buzzer_set_period(buzzer, H5);bsp_buzzer_on(buzzer);vTaskDelay(440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 5(H) */
    bsp_buzzer_set_period(buzzer, H1);bsp_buzzer_on(buzzer);vTaskDelay(440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 1(H) */
    bsp_buzzer_set_period(buzzer, H1);bsp_buzzer_on(buzzer);vTaskDelay(440+440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 1(H) */
    bsp_buzzer_set_period(buzzer, H2);bsp_buzzer_on(buzzer);vTaskDelay(440+440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 2(H) */
    /* 第五小节 */
    bsp_buzzer_set_period(buzzer, H3);bsp_buzzer_on(buzzer);vTaskDelay(440+440+440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 3(H) */
    bsp_buzzer_set_period(buzzer, M3);bsp_buzzer_on(buzzer);vTaskDelay(400);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 3 */
    bsp_buzzer_set_period(buzzer, M5);bsp_buzzer_on(buzzer);vTaskDelay(400);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 5 */
    bsp_buzzer_set_period(buzzer, H1);bsp_buzzer_on(buzzer);vTaskDelay(400);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 1(H) */
    bsp_buzzer_set_period(buzzer, H3);bsp_buzzer_on(buzzer);vTaskDelay(400);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 3(H) */
    /* 第六小节 */
    bsp_buzzer_set_period(buzzer, H3);bsp_buzzer_on(buzzer);vTaskDelay(440+440+440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 3(H) */
    bsp_buzzer_set_period(buzzer, H4);bsp_buzzer_on(buzzer);vTaskDelay(440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);
    bsp_buzzer_set_period(buzzer, H2);bsp_buzzer_on(buzzer);vTaskDelay(440+440+440+440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 2(H) */
    /* 第七小节 */
    bsp_buzzer_set_period(buzzer, H2);bsp_buzzer_on(buzzer);vTaskDelay(440+440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 2(H) */
    bsp_buzzer_set_period(buzzer, H5);bsp_buzzer_on(buzzer);vTaskDelay(440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 5(H) */
    bsp_buzzer_set_period(buzzer, M7);bsp_buzzer_on(buzzer);vTaskDelay(440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 7*/
    bsp_buzzer_set_period(buzzer, H1);bsp_buzzer_on(buzzer);vTaskDelay(440+440+440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 2(H) */
    /* 第八小节 */
    bsp_buzzer_set_period(buzzer, H3);bsp_buzzer_on(buzzer);vTaskDelay(440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 3(H) */
    bsp_buzzer_set_period(buzzer, H4);bsp_buzzer_on(buzzer);vTaskDelay(440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 4(H) */
    bsp_buzzer_set_period(buzzer, H5);bsp_buzzer_on(buzzer);vTaskDelay(440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 5(H) */
    bsp_buzzer_set_period(buzzer, H1);bsp_buzzer_on(buzzer);vTaskDelay(440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 1(H) */
    bsp_buzzer_set_period(buzzer, H1);bsp_buzzer_on(buzzer);vTaskDelay(440+440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 1(H) */
    bsp_buzzer_set_period(buzzer, H2);bsp_buzzer_on(buzzer);vTaskDelay(440+440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 2(H) */
    /* 第九小节 */
    bsp_buzzer_set_period(buzzer, H1);bsp_buzzer_on(buzzer);vTaskDelay(440+440+440-40);bsp_buzzer_off(buzzer);vTaskDelay(40);/* 1(H) */
}