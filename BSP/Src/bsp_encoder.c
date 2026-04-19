/*******************************************************************************
 *      文    件: bsp_encoder.c
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 编码器 驱动
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#include "bsp_encoder.h"
#include "bsp_message.h"
#ifndef NULL
#define NULL (void*)0
#endif
/**
 * @brief 编码器锚点服务于中断函数
 * 
 */
static encoder_handle_t* s_encoder;
/**
 * @brief 消抖定时器
 * 
 * @param handle    编码器句柄
 * @param delay_ms  消抖时间
 * @warning 这个函数于定时器配置密切相关，使用默认配置（ENCODER_DEFAULT_CONFIG()）最好
 */
static void encoder_tim_start(encoder_handle_t *handle, uint16_t delay_ms)
{
    uint16_t counter = delay_ms * 100;
    TIM_SetCounter(handle->dev.tim, counter);
    TIM_Cmd(handle->dev.tim, ENABLE);
}
/**
 * @brief 消抖定时器停止
 * 
 * @param handle     编码器句柄
 */
static void encoder_tim_stop(encoder_handle_t *handle)
{
    TIM_Cmd(handle->dev.tim, DISABLE);
}
/**
 * @brief 编码器中断处理函数
 * @note 被 bsp_it.c 内的STM32外部中断函数调用
 * 
 * @param param 
 */
void encoder_a_IRQhandler(void* param)
{
    if (EXTI_GetITStatus(s_encoder->dev.exti_a_line) != RESET)
    {
        s_encoder->dev.rotate_detection_flag = 1;
        if (GPIO_ReadInputDataBit(s_encoder->dev.b_port, s_encoder->dev.b_pin) == Bit_RESET)
        {
            s_encoder->count++;
            if (s_encoder->event_callback.rotate_clockwise != NULL)
            {
                s_encoder->event_callback.rotate_clockwise((const void*)s_encoder);
            }
        }
        else
        {
            s_encoder->count--;
            if (s_encoder->event_callback.rotate_counterclockwise != NULL)
            {
                s_encoder->event_callback.rotate_counterclockwise((const void*)s_encoder);
            }
        }
        EXTI_ClearITPendingBit(s_encoder->dev.exti_a_line);
    }
}
/**
 * @brief 编码器按键中断处理函数
 * @note 被 bsp_it.c 内的STM32外部中断函数调用
 * 
 * @param param 
 */
void encoder_btn_IRQhandler(void* param)
{
    if (EXTI_GetITStatus(s_encoder->dev.exti_btn_line) != RESET)
    {
        if (GPIO_ReadInputDataBit(s_encoder->dev.btn_port, s_encoder->dev.btn_pin) == Bit_RESET)
        {
            /* 开始按下延时消抖 */
            s_encoder->dev.pre_debouncing_flag = 1;
            encoder_tim_start(s_encoder, 20);
        }
        else
        {
            /* 开始释放延时消抖 */
            s_encoder->dev.post_debouncing_flag = 1;
            encoder_tim_start(s_encoder, 20);
        }

        EXTI_ClearITPendingBit(s_encoder->dev.exti_btn_line);
    }
}
/**
 * @brief 编码器按键消抖定时器中断处理函数
 * @note 被 bsp_it.c 内的STM32定时器中断函数调用
 * @param param 
 */
void encoder_tim_IRQHandler(void* param)
{
    if (TIM_GetITStatus(s_encoder->dev.tim, TIM_IT_Update) != RESET)
    {
        if (s_encoder->dev.long_press_flag)
        {
            s_encoder->dev.long_press_flag = 0;
            /* 再次判断 */
            if (GPIO_ReadInputDataBit(s_encoder->dev.btn_port, s_encoder->dev.btn_pin) == Bit_RESET)
            {
                s_encoder->btn_state = LONGPRESS;
                if (s_encoder->event_callback.longpress != NULL)
                {
                    s_encoder->event_callback.longpress((const void*)s_encoder);
                }
            }
        }
        /* 按下消抖结束 */
        if (s_encoder->dev.pre_debouncing_flag)
        {
            s_encoder->dev.pre_debouncing_flag = 0;
            /* 再次判断 */
            if (GPIO_ReadInputDataBit(s_encoder->dev.btn_port, s_encoder->dev.btn_pin) == Bit_RESET)
            {
                s_encoder->btn_state = PRESS;
                s_encoder->dev.rotate_detection_flag = 0;
                if (s_encoder->event_callback.press != NULL)
                {
                    s_encoder->event_callback.press((const void*)s_encoder);
                }
                /* 长按计时开始 */
                s_encoder->dev.long_press_flag = 1;
                encoder_tim_start(s_encoder, 500);
            }
        }
        if (s_encoder->dev.post_debouncing_flag)
        {
            s_encoder->dev.post_debouncing_flag = 0;
            /* 再次判断 */
            if (GPIO_ReadInputDataBit(s_encoder->dev.btn_port, s_encoder->dev.btn_pin) == Bit_SET && s_encoder->dev.rotate_detection_flag == 0)
            {
                s_encoder->btn_state = RELEASE;
                if (s_encoder->event_callback.release != NULL)
                {
                    s_encoder->event_callback.release((const void*)s_encoder);
                }
                encoder_tim_stop(s_encoder);
            }
        }
        TIM_ClearITPendingBit(s_encoder->dev.tim, TIM_IT_Update);
    }
}
/**
 * @brief 编码器时钟初始化
 * 
 * @param cfg 
 */
static void encoder_clk_init(const encoder_cfg_t *cfg)
{
    RCC_AHB1PeriphClockCmd(cfg->gpio_a_clk, ENABLE);
    RCC_AHB1PeriphClockCmd(cfg->gpio_b_clk, ENABLE);
    RCC_AHB1PeriphClockCmd(cfg->gpio_btn_clk, ENABLE);
    RCC_APB1PeriphClockCmd(cfg->tim_clk, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
}
/**
 * @brief 编码器 GPIO 初始化
 * 
 * @param cfg 
 */
static void encoder_gpio_init(const encoder_cfg_t *cfg)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = cfg->gpio_a_pin | cfg->gpio_b_pin | cfg->gpio_btn_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/**
 * @brief 编码器 外部中断 初始化
 * 
 * @param cfg 
 */
static void encoder_exti_init(const encoder_cfg_t *cfg)
{
    SYSCFG_EXTILineConfig(cfg->exti_a_port_src, cfg->exti_a_pin_src);
    SYSCFG_EXTILineConfig(cfg->exti_btn_port_src, cfg->exti_btn_pin_src);

    EXTI_InitTypeDef EXTI_InitStructure;

    EXTI_InitStructure.EXTI_Line = cfg->exti_a_line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = cfg->exti_a_trigger;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    EXTI_InitStructure.EXTI_Line = cfg->exti_btn_line;
    EXTI_InitStructure.EXTI_Trigger = cfg->exti_btn_trigger;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = cfg->nvic_a_irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = cfg->nvic_a_priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = cfg->nvic_a_subpriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = cfg->nvic_btn_irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = cfg->nvic_btn_priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = cfg->nvic_btn_subpriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
 * @brief 编码器 定时器 初始化
 * 
 * @param cfg 
 */
static void encoder_tim_init(const encoder_cfg_t *cfg)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    TIM_TimeBaseStructure.TIM_Prescaler = cfg->tim_psc;
    TIM_TimeBaseStructure.TIM_Period = cfg->tim_arr;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseInit(cfg->tim, &TIM_TimeBaseStructure);

    TIM_ITConfig(cfg->tim, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = cfg->nvic_tim_irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = cfg->nvic_tim_priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = cfg->nvic_tim_subpriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(cfg->tim, DISABLE);
}

/**
 * @brief 编码器初始化
 * 
 * @param handle  编码器句柄
 * @param cfg     编码器配置
 *     @note 一般情况下可以直接调用 ENCODER_DEFAULT_CONFIG() 默认初始化即可
 */
void bsp_encoder_init(encoder_handle_t *handle, const encoder_cfg_t *cfg)
{
    if (handle == NULL || cfg == NULL)
        return;
    handle->dev.a_port      = cfg->gpio_a_port;
    handle->dev.a_pin       = cfg->gpio_a_pin;
    handle->dev.b_port      = cfg->gpio_b_port;
    handle->dev.b_pin       = cfg->gpio_b_pin;
    handle->dev.btn_port    = cfg->gpio_btn_port;
    handle->dev.btn_pin     = cfg->gpio_btn_pin;
    handle->dev.exti_a_line = cfg->exti_a_line;
    handle->dev.exti_btn_line = cfg->exti_btn_line;
    handle->dev.tim = cfg->tim;
    handle->dev.tim_arr     = cfg->tim_arr;
    handle->dev.tim_psc     = cfg->tim_psc;

    handle->dev.pre_debouncing_flag     = 0;
    handle->dev.post_debouncing_flag    = 0;
    handle->dev.long_press_flag         = 0;
    handle->dev.rotate_detection_flag   = 0;
    s_encoder = handle;

    encoder_clk_init(cfg);
    encoder_gpio_init(cfg);
    encoder_exti_init(cfg);
    encoder_tim_init(cfg);
}
