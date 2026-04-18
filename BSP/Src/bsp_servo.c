#include "bsp_servo.h"
#include "bsp_debug.h"
#ifndef NULL
#define NULL 0
#endif

/**
 * @brief 初始化舵机
 */
void bsp_servo_init(servo_handle_t *handle, servo_cfg_t *cfg)
{
    if (handle == NULL || cfg == NULL)
    {
        return;
    }

    handle->dev.tim = cfg->tim;
    handle->dev.channel = cfg->channel;

    /* 时钟初始化 */
    RCC_AHB1PeriphClockCmd(cfg->gpio_clk, ENABLE);
    RCC_APB2PeriphClockCmd(cfg->tim_clk, ENABLE);

    /* GPIO 初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = cfg->gpio_pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(cfg->gpio_port, &GPIO_InitStructure);

    /* GPIO 复用配置 */
    GPIO_PinAFConfig(cfg->gpio_port, cfg->gpio_af_src, cfg->gpio_af);

    /* 定时器时基初始化 */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler         = cfg->tim_prescaler;
    TIM_TimeBaseStructure.TIM_Period            = cfg->tim_period;
    TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(cfg->tim, &TIM_TimeBaseStructure);

    /* PWM 输出通道初始化 */
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_OCMode        = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputNState  = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse         = cfg->duty;
    TIM_OCInitStructure.TIM_OCNPolarity   = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCNIdleState  = TIM_OCNIdleState_Reset;

    switch (cfg->channel)
    {
        case TIM_Channel_1:
            TIM_OC1Init(cfg->tim, &TIM_OCInitStructure);
            TIM_OC1PreloadConfig(cfg->tim, TIM_OCPreload_Enable);
            break;
        case TIM_Channel_2:
            TIM_OC2Init(cfg->tim, &TIM_OCInitStructure);
            TIM_OC2PreloadConfig(cfg->tim, TIM_OCPreload_Enable);
            break;
        case TIM_Channel_3:
            TIM_OC3Init(cfg->tim, &TIM_OCInitStructure);
            TIM_OC3PreloadConfig(cfg->tim, TIM_OCPreload_Enable);
            break;
        case TIM_Channel_4:
            TIM_OC4Init(cfg->tim, &TIM_OCInitStructure);
            TIM_OC4PreloadConfig(cfg->tim, TIM_OCPreload_Enable);
            break;
        default:
            SGCS_ERROR("invalid channel %d", cfg->channel);
            return;
    }

    TIM_ARRPreloadConfig(cfg->tim, ENABLE);
    TIM_Cmd(cfg->tim, ENABLE);

    /* 高级定时器 TIM1/TIM8 必须开启主输出 MOE */
    TIM_CtrlPWMOutputs(cfg->tim, ENABLE);

    /* 初始设置 90° 中位 */
    bsp_servo_set_angle(handle, 90.0f);
}

/**
 * @brief 角度转 PWM 高电平时间（us）
 * 0°→500us  180°→2500us
 */
static uint16_t angle_to_duty_us(float angle)
{
    if (angle < 0.0f) angle = 0.0f;
    if (angle > 180.0f) angle = 180.0f;

    return (uint16_t)( (angle / 180.0f) * 2000.0f + 500.0f );
}

/**
 * @brief 设置舵机角度（0~180°）
 * @brief 0 ~ 180
 */
void bsp_servo_set_angle(servo_handle_t *handle, float angle)
{
    uint16_t duty;

    if (handle == NULL)
    {
        return;
    }

    handle->angle = angle;
    duty = angle_to_duty_us(angle);

    switch (handle->dev.channel)
    {
        case TIM_Channel_1:
            TIM_SetCompare1(handle->dev.tim, duty);
            break;
        case TIM_Channel_2:
            TIM_SetCompare2(handle->dev.tim, duty);
            break;
        case TIM_Channel_3:
            TIM_SetCompare3(handle->dev.tim, duty);
            break;
        case TIM_Channel_4:
            TIM_SetCompare4(handle->dev.tim, duty);
            break;
        default:
            SGCS_ERROR("invalid channel %d", handle->dev.channel);
            break;
    }
}
/**
 * @brief  将电平时间转换为角度
 * 
 * @param us 
 * @return float 
 */
static float duty_us_to_angle(uint16_t us)
{
    if (us < 500)  us = 500;
    if (us > 2500) us = 2500;

    return (us - 500.0f) / 2000.0f * 180.0f;
}
/**
 * @brief 通过微妙数设置舵机角度
 * @note 500~2500
 * @param handle 
 * @param us 
 */
void bsp_servo_set_us(servo_handle_t *handle, uint16_t us)
{
    if (handle == NULL) return;
    if (us < 500)  us = 500;
    if (us > 2500) us = 2500;

    handle->angle = duty_us_to_angle(us);
    switch (handle->dev.channel)
    {
        case TIM_Channel_1:
            TIM_SetCompare1(handle->dev.tim, us);
            break;
        case TIM_Channel_2:
            TIM_SetCompare2(handle->dev.tim, us);
            break;
        case TIM_Channel_3:
            TIM_SetCompare3(handle->dev.tim, us);
            break;
        case TIM_Channel_4:
            TIM_SetCompare4(handle->dev.tim, us);
            break;
        default:
            break;
    }
}
/**
 * @brief 获取当前舵机角度
 */
void bsp_servo_get_angle(servo_handle_t *handle, float *angle)
{
    if (handle == NULL || angle == NULL)
    {
        return;
    }
    *angle = handle->angle;
}