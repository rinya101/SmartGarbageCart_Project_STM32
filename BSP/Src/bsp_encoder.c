/*******************************************************************************
 *      文    件: bsp_encoder.h
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 编码器驱动
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#include "bsp_encoder.h"
#include "stm32f4xx_gpio.h"
#ifndef NULL
#define NULL ((void*)0)
#endif
#if (ENCODER_BTN_IRQ_HANDLE == ENCODER_LEFT_IRQ_HANDLE)
void ENCODER_BTN_IRQ_HANDLE(void)
{
    if (EXTI_GetITStatus(g_encoder->btn_line) != RESET)
    {
        g_encoder->btn_cb(g_encoder);
        EXTI_ClearITPendingBit(g_encoder->btn_line);
    }
    if (EXTI_GetITStatus(g_encoder->left_line) != RESET)
    {
        g_encoder->encoder_cb(g_encoder);
        EXTI_ClearITPendingBit(g_encoder->left_line);
    }
    
}
#else
void ENCODER_BTN_IRQ_HANDLE(void)
{
    if (EXTI_GetITStatus(g_encoder->btn_line) != RESET)
    {
        g_encoder->btn_cb(g_encoder);
        EXTI_ClearITPendingBit(g_encoder->btn_line);
    }
}
void ENCODER_LEFT_IRQ_HANDLE(void)
{
    if (EXTI_GetITStatus(g_encoder->left_line) != RESET)
    {
        g_encoder->encoder_cb(g_encoder);
        EXTI_ClearITPendingBit(g_encoder->left_line);
    }
}
#endif
/**
 * @brief 编码器实例
 * 
 */
encoder_handle_t* g_encoder = NULL;

/**
 * @brief 默认回调函数
 * @note 需要手动挂载
 * @param encoder 
 * @return void
 */
static __weak void encoder_default_cb(encoder_handle_t *encoder)
{
    if (encoder == NULL)
    {
        SGCS_ERROR("encoder is null");
        return;
    }
    uint8_t r = GPIO_ReadInputDataBit(encoder->right_port, encoder->right_pin);
    if(r == 1) 
    {
        encoder->count++;  // 顺时针
        encoder->turn_state = RIGHT;
        encoder->right_event(encoder);
    }
    else
    {
        encoder->count--;  // 逆时针
        encoder->turn_state = LEFT;
        encoder->left_event(encoder);
    }
}
/**
 * @brief 默认按键回调函数
 * @note 下降沿触发
 * @param encoder 
 * @return __weak 
 */
static __weak void encoder_btn_default_cb(encoder_handle_t *encoder)
{
    if (encoder == NULL)
    {
        SGCS_ERROR("encoder is null");
        return;
    }
    if (GPIO_ReadInputDataBit(encoder->btn_port, encoder->btn_pin) == 0)
    {
        encoder->btn_state = PRESS;
        encoder->btn_press(encoder);
        bsp_tim5_timing(60000, TIM5_USED_ENCODER_ID); /* 0 ：encoder */
    }
    else
    {
        encoder->btn_state = RELEASE;
        bsp_tim5_stop();
    }
}
/**
 * @brief 编码左转事件
 * 
 * @param encode 
 * @return __weak 
 */
static __weak void encoder_turn_left_event(encoder_handle_t* encode)
{
    printf("[info] encoder_left_event: You can add or modify code here. [File: %s | Func: %s | Line: %d]\r\n",
        __FILE__, __FUNCTION__, __LINE__);
}
/**
 * @brief 编码左转事件
 * 
 * @param encode 
 * @return __weak 
 */
static __weak void encoder_turn_right_event(encoder_handle_t* encode)
{
    printf("[info] encoder_right_event: You can add or modify code here. [File: %s | Func: %s | Line: %d]\r\n",
        __FILE__, __FUNCTION__, __LINE__);
}
/**
 * @brief 编码按键按下事件
 * 
 * @param encode 
 * @return __weak 
 */
static __weak void encoder_btn_press_event(encoder_handle_t* encode)
{
    printf("[info] encoder_press_event: You can add or modify code here. [File: %s | Func: %s | Line: %d]\r\n",
        __FILE__, __FUNCTION__, __LINE__);
}
/**
 * @brief 编码按键长按事件
 * 
 * @param encoder 
 * @return __weak 
 */
static __weak void encoder_btn_long_press_event(encoder_handle_t* encoder)
{
    if (GPIO_ReadInputDataBit(encoder->btn_port, encoder->btn_pin) == 0)
    {
        encoder->btn_state = LONG_PRESS;
        printf("[info] encoder_long_press_event: You can add or modify code here. [File: %s | Func: %s | Line: %d]\r\n",
            __FILE__, __FUNCTION__, __LINE__);
    }
}
/**
 * @brief 获取 GPIO 端口源 (给 SYSCFG 使用)
 */
static uint8_t GPIO_GetPortSource(GPIO_TypeDef* GPIOx)
{
    if (GPIOx == GPIOA) return EXTI_PortSourceGPIOA;
    if (GPIOx == GPIOB) return EXTI_PortSourceGPIOB;
    if (GPIOx == GPIOC) return EXTI_PortSourceGPIOC;
    return 0;
}

/**
 * @brief 获取 GPIO 引脚源 (给 SYSCFG 使用)
 */
static uint8_t GPIO_GetPinSource(uint32_t pin)
{
    if (pin == GPIO_Pin_0)  return GPIO_PinSource0;
    if (pin == GPIO_Pin_1)  return GPIO_PinSource1;
    if (pin == GPIO_Pin_2)  return GPIO_PinSource2;
    if (pin == GPIO_Pin_3)  return GPIO_PinSource3;
    if (pin == GPIO_Pin_4)  return GPIO_PinSource4;
    if (pin == GPIO_Pin_5)  return GPIO_PinSource5;
    if (pin == GPIO_Pin_6)  return GPIO_PinSource6;
    if (pin == GPIO_Pin_7)  return GPIO_PinSource7;
    if (pin == GPIO_Pin_8)  return GPIO_PinSource8;
    if (pin == GPIO_Pin_9)  return GPIO_PinSource9;
    if (pin == GPIO_Pin_10) return GPIO_PinSource10;
    if (pin == GPIO_Pin_11) return GPIO_PinSource11;
    if (pin == GPIO_Pin_12) return GPIO_PinSource12;
    if (pin == GPIO_Pin_13) return GPIO_PinSource13;
    if (pin == GPIO_Pin_14) return GPIO_PinSource14;
    if (pin == GPIO_Pin_15) return GPIO_PinSource15;
    return 0;
}
/**
 * @brief 编码器初始化
 * 
 * @param handle 
 * @param cfg 
 */
void encoder_init(encoder_handle_t* handle , encoder_cfg_t cfg)
{
    if (handle == NULL)
    {
        SGCS_ERROR("handle is NULL");
        return;
    }
    /* 句柄获取 参数 */
    handle->btn_line    = cfg.btn_exti_line;
    handle->left_line   = cfg.left_exti_line;
    handle->encoder_cb  = cfg.event_cb;
    handle->btn_cb      = cfg.btn_cb;
    handle->left_event      = cfg.left_event;
    handle->right_event     = cfg.right_event;
    handle->btn_press       = cfg.btn_press;
    handle->btn_long_press  = cfg.btn_long_press;
    handle->btn_port    = cfg.btn_port;
    handle->btn_pin     = cfg.btn_pin;
    handle->left_port   = cfg.left_port;
    handle->left_pin    = cfg.left_pin;
    handle->right_port  = cfg.right_port;
    handle->right_pin   = cfg.right_pin;
    /* 时钟使能 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    /* GPIO 配置 */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode       = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Speed      = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd       = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Pin        = cfg.btn_pin;
    GPIO_Init(cfg.btn_port, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin        = cfg.left_pin;
    GPIO_Init(cfg.left_port, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin        = cfg.right_pin;
    GPIO_Init(cfg.right_port, &GPIO_InitStruct);
    /* EXIT 映射 */
    SYSCFG_EXTILineConfig(GPIO_GetPortSource(cfg.btn_port), GPIO_GetPinSource(cfg.btn_pin));
    SYSCFG_EXTILineConfig(GPIO_GetPortSource(cfg.left_port), GPIO_GetPinSource(cfg.left_pin));
    /* 中断配置 */
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line       = cfg.btn_exti_line;
    EXTI_InitStruct.EXTI_Mode       = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger    = cfg.btn_trigger;
    EXTI_InitStruct.EXTI_LineCmd    = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
    EXTI_InitStruct.EXTI_Line       = cfg.left_exti_line;
    EXTI_InitStruct.EXTI_Trigger    = cfg.left_trigger;
    EXTI_Init(&EXTI_InitStruct);
    /* NVIC 配置 */
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel                     = cfg.btn_irq_channel;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = cfg.preemption_priority;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority           = cfg.sub_priority;
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    NVIC_InitStruct.NVIC_IRQChannel                     = cfg.left_irq_channel;
    NVIC_Init(&NVIC_InitStruct);
    /* 定时器5 初始化 */
    tim5_cfg_t tim5_cfg;
    tim5_cfg = TIM5_DEFAULT_CONFIG();
    bsp_tim5_attach_callback(&g_tim5_handle);
    bsp_tim5_init(&g_tim5_handle, tim5_cfg);
    /* DUBUG */
    SGCS_INFO("encoder init succeed");
}
/**
 * @brief 挂载回调函数
 * 
 * @param handle 
 * @param cb 
 * @param btn_cb 
 */
void encoder_attach_callback(encoder_cfg_t *cfg)
{
    if (cfg == NULL)
    {
        SGCS_ERROR("cfg is null");
        return;
    }
    cfg->event_cb = encoder_default_cb;
    cfg->btn_cb = encoder_btn_default_cb;
}
/**
 * @brief 挂载事件
 * 
 * @param cfg 
 */
void encoder_attach_event(encoder_cfg_t *cfg)
{
    if (cfg == NULL)
    {
        SGCS_ERROR("cfg is null");
        return;
    }
    cfg->left_event = encoder_turn_left_event;
    cfg->right_event = encoder_turn_right_event;
    cfg->btn_press = encoder_btn_press_event;
    cfg->btn_long_press = encoder_btn_long_press_event;
}