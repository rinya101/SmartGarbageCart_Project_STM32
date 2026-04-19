/*******************************************************************************
 *      文    件: bsp_motor.c
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 电机 BSP驱动
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#include "bsp_motor.h"
#include "math.h"
#ifndef NULL
#define NULL (void*)0
#endif
/**
 * @brief 初始化电机时钟
 * 
 * @param cfg 
 */
static void motor_clk_init(motor_cfg_t *cfg)
{
    RCC_AHB1PeriphClockCmd(cfg->gpio_m1a_rcc, ENABLE);
    RCC_AHB1PeriphClockCmd(cfg->gpio_m1b_rcc, ENABLE);
    RCC_AHB1PeriphClockCmd(cfg->gpio_m2a_rcc, ENABLE);
    RCC_AHB1PeriphClockCmd(cfg->gpio_m2b_rcc, ENABLE);
    RCC_APB1PeriphClockCmd(cfg->tim_rcc, ENABLE);
}
/**
 * @brief 电机GPIO初始化
 * 
 * @param cfg 
 */
static void motor_gpio_init(motor_cfg_t *cfg)
{
    GPIO_InitTypeDef gpio_init;

    // M1B 方向
    gpio_init.GPIO_Pin   = cfg->gpio_m1b_pin;
    gpio_init.GPIO_Mode  = cfg->gpio_m1b_mode;
    gpio_init.GPIO_OType = cfg->gpio_m1b_otype;
    gpio_init.GPIO_Speed = cfg->gpio_m1b_speed;
    gpio_init.GPIO_PuPd  = cfg->gpio_m1b_pu;
    GPIO_Init(cfg->gpio_m1b_port, &gpio_init);

    // M2B 方向
    gpio_init.GPIO_Pin   = cfg->gpio_m2b_pin;
    gpio_init.GPIO_Mode  = cfg->gpio_m2b_mode;
    gpio_init.GPIO_OType = cfg->gpio_m2b_otype;
    gpio_init.GPIO_Speed = cfg->gpio_m2b_speed;
    gpio_init.GPIO_PuPd  = cfg->gpio_m2b_pu;
    GPIO_Init(cfg->gpio_m2b_port, &gpio_init);

    // M1A PWM
    gpio_init.GPIO_Pin   = cfg->gpio_m1a_pin;
    gpio_init.GPIO_Mode  = cfg->gpio_m1a_mode;
    gpio_init.GPIO_OType = cfg->gpio_m1a_otype;
    gpio_init.GPIO_Speed = cfg->gpio_m1a_speed;
    gpio_init.GPIO_PuPd  = cfg->gpio_m1a_pu;
    GPIO_Init(cfg->gpio_m1a_port, &gpio_init);
    GPIO_PinAFConfig(cfg->gpio_m1a_port, cfg->gpio_m1a_pin_source, cfg->gpio_m1a_af);

    // M2A PWM
    gpio_init.GPIO_Pin   = cfg->gpio_m2a_pin;
    gpio_init.GPIO_Mode  = cfg->gpio_m2a_mode;
    gpio_init.GPIO_OType = cfg->gpio_m2a_otype;
    gpio_init.GPIO_Speed = cfg->gpio_m2a_speed;
    gpio_init.GPIO_PuPd  = cfg->gpio_m2a_pu;
    GPIO_Init(cfg->gpio_m2a_port, &gpio_init);
    GPIO_PinAFConfig(cfg->gpio_m2a_port, cfg->gpio_m2a_pin_source, cfg->gpio_m2a_af);
}
/**
 * @brief 电机定时器初始化
 * 
 * @param cfg 
 */
static void motor_tim_init(motor_cfg_t *cfg)
{
    TIM_TimeBaseInitTypeDef tim_init;
    tim_init.TIM_Prescaler     = cfg->tim_pre;
    tim_init.TIM_Period        = cfg->tim_period;
    tim_init.TIM_ClockDivision = cfg->tim_div;
    tim_init.TIM_CounterMode   = cfg->tim_mode;
    tim_init.TIM_RepetitionCounter = cfg->tim_rep;
    TIM_TimeBaseInit(cfg->tim, &tim_init);

    TIM_OCInitTypeDef tim_oc_init;
    TIM_OCStructInit(&tim_oc_init);
    tim_oc_init.TIM_OCMode      = TIM_OCMode_PWM1;
    tim_oc_init.TIM_OutputState = TIM_OutputState_Enable;
    tim_oc_init.TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM_OC1Init(cfg->tim, &tim_oc_init);
    TIM_OC1PreloadConfig(cfg->tim, TIM_OCPreload_Enable);
    TIM_OC2Init(cfg->tim, &tim_oc_init);
    TIM_OC2PreloadConfig(cfg->tim, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(cfg->tim, ENABLE);
    TIM_Cmd(cfg->tim, ENABLE);
}
/**
 * @brief 电机初始化
 * 
 * @param handle 
 * @param cfg 
 */
void bsp_motor_init(motor_handle_t *handle, motor_cfg_t *cfg)
{
    if (handle == NULL || cfg == NULL) return;
    handle->dev.tim            = cfg->tim;
    handle->dev.tim_channel1   = cfg->tim_channel1;
    handle->dev.tim_channel2   = cfg->tim_channel2;
    handle->dev.period         = cfg->tim_period;
    handle->dev.gpio_m1b_port       = cfg->gpio_m1b_port;
    handle->dev.gpio_m1b_pin        = cfg->gpio_m1b_pin;
    handle->dev.gpio_m2b_port       = cfg->gpio_m2b_port;
    handle->dev.gpio_m2b_pin        = cfg->gpio_m2b_pin;
    handle->dev.gpio_m1a_port       = cfg->gpio_m1a_port;
    handle->dev.gpio_m1a_pin        = cfg->gpio_m1a_pin;
    handle->dev.gpio_m2a_port       = cfg->gpio_m2a_port;
    handle->speed1 = 0;
    handle->speed2 = 0;
    motor_clk_init(cfg);
    motor_gpio_init(cfg);
    motor_tim_init(cfg);
    bsp_motor_stop(handle);
}

/**
 * @brief 速度调制（正弦）
 * @note 输入 0 ~ 1000 ，输出 0 ~ 1000
 * @param x 
 * @return int16_t 
 */
static int16_t calculate_smooth_speed(int16_t x)
{
    // 限制范围：-1000 ~ 1000
    if(x < -1000) x = -1000;
    if(x > 1000)  x = 1000;
    
    // 你的公式：y = 1000 * sin( x / 636.0f )
    float y = 1000.0f * sinf( x / 636.0f );
    
    // 转成整数速度值
    return (int16_t)y;
}
/**
 * @brief 设置电机速度
 * 
 * @param handle 
 * @param speed1 
 * @param speed2 
 */
void bsp_motor_set_speed(motor_handle_t *handle, int16_t speed1, int16_t speed2)
{
    if (handle == NULL) return;
    /* 数值更新 */
    handle->speed1 = speed1;
    handle->speed2 = speed2;
    speed1 = calculate_smooth_speed(speed1);
    speed2 = calculate_smooth_speed(speed2);
    uint32_t ccr1 = 0, ccr2 = 0;
    if (speed1 > 0) 
    {
        GPIO_SetBits(handle->dev.gpio_m1b_port, handle->dev.gpio_m1b_pin);
        ccr1 = speed1;
        ccr1 = handle->dev.period - ccr1;
    } else 
    {
        GPIO_ResetBits(handle->dev.gpio_m1b_port, handle->dev.gpio_m1b_pin);
        ccr1 = -speed1;
        
    }
    if (speed2 > 0) 
    {
        GPIO_SetBits(handle->dev.gpio_m2b_port, handle->dev.gpio_m2b_pin);
        ccr2 = speed2;
        ccr2 = handle->dev.period - ccr2;
    } else 
    {
        GPIO_ResetBits(handle->dev.gpio_m2b_port, handle->dev.gpio_m2b_pin);
        ccr2 = -speed2;
        
    }
    TIM_SetCompare1(handle->dev.tim, ccr1);
    TIM_SetCompare2(handle->dev.tim, ccr2); 
}
/**
 * @brief 电机停止
 * 
 * @param handle 
 */
void bsp_motor_stop(motor_handle_t *handle)
{
    bsp_motor_set_speed(handle, 0, 0);
}