/*******************************************************************************
 *      文    件: bsp_ultrasonic.h
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 超声波 模块
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#include "bsp_ultrasonic.h"
#include "PeripheralParamConfig.h"
#include "bsp_debug.h"
#ifndef NULL
#define NULL (void*)0
#endif

ult_handle_t* s_ult_handle = NULL;

/**
 * @brief us 转换距离
 * @param us        微秒数（ECHO高电平时间）
 * @param distance  输出距离，单位：厘米 cm
 */
static uint16_t us_transform_distance(uint16_t us)
{
    uint16_t distance = 0;
    // 声速 = 340 m/s
    // 距离 = 声速 * 时间 / 2   (往返)
    // 简化公式：cm = us / 58
    distance = us / 58;
    return distance;
}
/**
 * @brief 超声波数据更新事件
 * 
 * @param data 
 * @return __weak 
 */
static __weak void ult_data_updata_event(void* data)
{
//     printf("ult_:%d\r\n\
// [warning] You need to rewrite this function at the application layer! | File:%s | Line:%d | Fun:%s\r\n",
//         ((ult_handle_t*)data)->distance_cm,                       
//         __FILE__, __LINE__, __FUNCTION__
//     );

}
/**
 * @brief first 超声波外部中断处理函数
 * 
 * @param param 
 */
void ult_eh_exti_handler(void* param)
{
    if (EXTI_GetITStatus(s_ult_handle->dev_handle.eh_exti_line) != RESET)
    {
        EXTI_ClearITPendingBit(s_ult_handle->dev_handle.eh_exti_line);
        uint32_t counter = 0;
        uint8_t echo_val = GPIO_ReadInputDataBit(
            s_ult_handle->dev_handle.eh_gpio_port,
            s_ult_handle->dev_handle.eh_gpio_pin
        );
        if (echo_val == 1) 
        {
            TIM_SetCounter(s_ult_handle->dev_handle.timx, 0);
            TIM_Cmd(s_ult_handle->dev_handle.timx, ENABLE);
            s_ult_handle->state = ULT_BUSY;
        }
        else
        {
            //TIM_Cmd(s_ult_handle->dev_handle.timx, DISABLE);
            counter = TIM_GetCounter(s_ult_handle->dev_handle.timx);
            s_ult_handle->distance_cm = us_transform_distance((uint16_t)counter);
            s_ult_handle->state = ULT_READY;
        }
    }
}

/**
 * @brief 超声波定时器中断处理函数
 * 
 * @param param 
 */
void ult_tim_handler(void* param)
{
    if (TIM_GetITStatus(s_ult_handle->dev_handle.timx, TIM_IT_Update) != RESET)
    {
        /* 超时 */
        TIM_ClearITPendingBit(s_ult_handle->dev_handle.timx, TIM_IT_Update);
        TIM_Cmd(s_ult_handle->dev_handle.timx, DISABLE);
        s_ult_handle->state = ULT_READY;
        ult_data_updata_event(s_ult_handle);
    }
}
/**
 * @brief 超声波模块时钟初始化
 * 
 * @param handle 
 * @param cfg 
 */
static void ultrasonic_clk_init(ult_handle_t* handle, ult_cfg_t* cfg)
{
    /* GPIO Tr Ecoh 时钟使能 */
    RCC_AHB1PeriphClockCmd(cfg->eh_gpio_rcc, ENABLE);
    RCC_AHB1PeriphClockCmd(cfg->tr_gpio_rcc, ENABLE);
    /* TIM 时钟使能 */
    RCC_APB1PeriphClockCmd(cfg->tim_rcc, ENABLE);
}
/**
 * @brief 超声波模块GPIO初始化
 * 
 * @param handle 
 * @param cfg 
 */
static void ultrasonic_gpio_init(ult_handle_t* handle, ult_cfg_t* cfg)
{
    /* Trig */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin         = cfg->tr_gpio_pin;
    GPIO_InitStructure.GPIO_Mode        = cfg->tr_gpio_mode;
    GPIO_InitStructure.GPIO_OType       = cfg->tr_gpio_otype;
    GPIO_InitStructure.GPIO_Speed       = cfg->tr_gpio_speed;
    GPIO_InitStructure.GPIO_PuPd        = cfg->tr_gpio_pupd;
    GPIO_Init(cfg->tr_gpio_port, &GPIO_InitStructure);
    /* Echo */
    GPIO_InitStructure.GPIO_Pin         = cfg->eh_gpio_pin;
    GPIO_InitStructure.GPIO_Mode        = cfg->eh_gpio_mode;
    GPIO_InitStructure.GPIO_OType       = cfg->eh_gpio_otype;
    GPIO_InitStructure.GPIO_Speed       = cfg->eh_gpio_speed;
    GPIO_InitStructure.GPIO_PuPd        = cfg->eh_gpio_pupd;
    GPIO_Init(cfg->eh_gpio_port, &GPIO_InitStructure);
}
/**
 * @brief 超声波模块 定时器 初始化
 * 
 * @param handle 
 * @param cfg 
 */
static void ultrasonic_tim_init(ult_handle_t* handle, ult_cfg_t* cfg)
{
    TIM_DeInit(cfg->timx);
    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    TIM_InitStructure.TIM_Prescaler     = cfg->tim_prescaler;
    TIM_InitStructure.TIM_Period        = cfg->tim_period;
    TIM_InitStructure.TIM_CounterMode   = cfg->tim_counter_mode;
    TIM_InitStructure.TIM_ClockDivision = cfg->tim_division;
    TIM_TimeBaseInit(cfg->timx, &TIM_InitStructure);
    /* 开启中断 */
    TIM_ITConfig(cfg->timx, TIM_IT_Update, ENABLE);
    /* 开启 TIM */
    TIM_Cmd(cfg->timx, ENABLE);
}
/**
 * @brief 超声波模块 外部中断 中断优先级 初始化
 * 
 * @param handle 
 * @param cfg 
 */
static void ultrasonic_exti_init(ult_handle_t* handle, ult_cfg_t* cfg)
{
    /* 配置外部中断 */
    SYSCFG_EXTILineConfig(cfg->tr_port_source, cfg->tr_pin_source);
    SYSCFG_EXTILineConfig(cfg->eh_port_source, cfg->tr_pin_source);
    /* 外部中断 */
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line    = cfg->eh_exti_line;
    EXTI_InitStructure.EXTI_Mode    = cfg->eh_exti_mode;
    EXTI_InitStructure.EXTI_Trigger = cfg->eh_exti_trigger;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = cfg->eh_nvic_irqn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = cfg->eh_nvic_pri;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = cfg->eh_nvic_subpri;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel                     = cfg->tim_nvic_irqn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority   = cfg->tim_nvic_pri;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority          = cfg->tim_nvic_subpri;
    NVIC_InitStructure.NVIC_IRQChannelCmd                  = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
 * @brief 初始化超声波
 * 
 * @param handle 
 * @param cfg 
 */
void bsp_ultrasonic_init(ult_handle_t* handle, ult_cfg_t* cfg)
{
    if (handle == NULL || cfg == NULL)
    {
        SGCS_ERROR("handle or cfg is NULL");
        return;
    }
    s_ult_handle = handle;
    s_ult_handle->dev_handle.eh_exti_line = cfg->eh_exti_line;
    s_ult_handle->dev_handle.timx = cfg->timx;
    s_ult_handle->dev_handle.tr_gpio_port = cfg->tr_gpio_port;
    s_ult_handle->dev_handle.eh_gpio_port = cfg->eh_gpio_port;
    s_ult_handle->dev_handle.tr_gpio_pin = cfg->tr_gpio_pin;
    s_ult_handle->dev_handle.eh_gpio_pin = cfg->eh_gpio_pin;
    handle->state = ULT_READY;
    /* 时钟使能 */
    ultrasonic_clk_init(handle, cfg);
    /* GPIO 配置 */
    ultrasonic_gpio_init(handle, cfg);
    /* 外部中断 */
    ultrasonic_exti_init(handle, cfg);
    /* TIM 配置 */
    ultrasonic_tim_init(handle, cfg);
}

/**
 * @brief 获取超声波距离
 * 
 * @param handle 
 * @return uint16_t 
 */
uint16_t bsp_ultrasonic_get_distance(ult_handle_t* handle)
{
    if (handle == NULL)
    {
        SGCS_ERROR("handle is NULL");
        return 0;
    }
    return handle->distance_cm;
}
/**
 * @brief 触发超声波测距
 */
void bsp_ultrasonic_trigger(ult_handle_t *handle)
{
    GPIO_SetBits(handle->dev_handle.tr_gpio_port, handle->dev_handle.tr_gpio_pin);
    for (volatile int i = 0; i < 150; i++); // 标准 10us+
    GPIO_ResetBits(handle->dev_handle.tr_gpio_port, handle->dev_handle.tr_gpio_pin);
}