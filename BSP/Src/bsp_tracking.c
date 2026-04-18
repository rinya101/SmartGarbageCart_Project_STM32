/*******************************************************************************
 *      文    件: bsp_tracking.h
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 循迹 模块驱动
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#include "bsp_tracking.h"
#include "bsp_debug.h"
/**
 * @brief 五路循迹私有句柄指针
 * 
 */
track_handle_t* s_track_a_handle = NULL;
track_handle_t* s_track_b_handle = NULL;
track_handle_t* s_track_c_handle = NULL;
track_handle_t* s_track_d_handle = NULL;
track_handle_t* s_track_e_handle = NULL;
/*-------------------------------循迹中断服务函数--------------------------------------*/

/**
 * @brief A 循迹中断处理函数
 * 
 * @param param 
 */
void tracker_a_exti_handler(void* param)
{
    if (EXTI_GetITStatus(s_track_a_handle->dev.exti_line) != RESET)
    {
        EXTI_ClearITPendingBit(s_track_a_handle->dev.exti_line);
        uint8_t sta = GPIO_ReadInputDataBit(s_track_a_handle->dev.gpio_port, 
            s_track_a_handle->dev.gpio_pin);
        if (sta != 0)
        {
            s_track_a_handle->sta = TRACKER_STATE_BLACK;
        }
        else
        {
            s_track_a_handle->sta = TRACKER_STATE_WHITE;
        }
    }
}
/**
 * @brief B路循迹中断处理函数
 * 
 * @param param 
 */
void tracker_b_exti_handler(void* param)
{
    if (EXTI_GetITStatus(s_track_b_handle->dev.exti_line) != RESET)
    {
        EXTI_ClearITPendingBit(s_track_b_handle->dev.exti_line);
        uint8_t sta = GPIO_ReadInputDataBit(s_track_b_handle->dev.gpio_port, 
            s_track_b_handle->dev.gpio_pin);
        if (sta != 0)
        {
            s_track_b_handle->sta = TRACKER_STATE_BLACK;
        }
        else
        {
            s_track_b_handle->sta = TRACKER_STATE_WHITE;
        }
    }
}
/**
 * @brief C路循迹中断处理函数
 * 
 * @param param 
 */
void tracker_c_exti_handler(void* param)
{
    if (EXTI_GetITStatus(s_track_c_handle->dev.exti_line) != RESET)
    {
        EXTI_ClearITPendingBit(s_track_c_handle->dev.exti_line);
        uint8_t sta = GPIO_ReadInputDataBit(s_track_c_handle->dev.gpio_port, 
                                            s_track_c_handle->dev.gpio_pin);
        if (sta != 0)
        {
            s_track_c_handle->sta = TRACKER_STATE_BLACK;
        }
        else
        {
            s_track_c_handle->sta = TRACKER_STATE_WHITE;
        }
    }
}
/**
 * @brief 循迹初始化
 * 
 * @param handle 
 * @param cfg 
 */
void bsp_tracking_init(track_handle_t* handle, track_cfg_t* cfg)
{
    if (handle == NULL || cfg == NULL)
    {
        SGCS_ERROR("hanle or cfg is NULL\n");
        return;
    }
    if (cfg->id == TRACKER_A)
    {
        s_track_a_handle = handle;
        s_track_a_handle->id = cfg->id;
        s_track_a_handle->dev.exti_line = cfg->exti_line;
        s_track_a_handle->dev.gpio_port = cfg->gpio_port;
        s_track_a_handle->dev.gpio_pin  = cfg->gpio_pin;
    }
    else if (cfg->id == TRACKER_B)
    {
        s_track_b_handle = handle;
        s_track_b_handle->id = cfg->id;
        s_track_b_handle->dev.exti_line = cfg->exti_line;
        s_track_b_handle->dev.gpio_port = cfg->gpio_port;
        s_track_b_handle->dev.gpio_pin  = cfg->gpio_pin;
    }
    else if (cfg->id == TRACKER_C)
    {
        s_track_c_handle = handle;
        s_track_c_handle->id = cfg->id;
        s_track_c_handle->dev.exti_line = cfg->exti_line;
        s_track_c_handle->dev.gpio_port = cfg->gpio_port;
        s_track_c_handle->dev.gpio_pin  = cfg->gpio_pin;
    }
    /* 时钟使能 */
    RCC_AHB1PeriphClockCmd(cfg->rcc_gpio, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    /* GPIO 配置 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin     = cfg->gpio_pin;
    GPIO_InitStructure.GPIO_Mode    = cfg->gpio_mode;
    GPIO_InitStructure.GPIO_OType   = cfg->gpio_otype;
    GPIO_InitStructure.GPIO_Speed   = cfg->gpio_speed;
    GPIO_InitStructure.GPIO_PuPd    = cfg->gpio_pupd;
    GPIO_Init(cfg->gpio_port, &GPIO_InitStructure);
    SYSCFG_EXTILineConfig(cfg->gpio_port_source, cfg->gpio_pin_source);
    /* 外部中断配置 */
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line    = cfg->exti_line;
    EXTI_InitStructure.EXTI_Mode    = cfg->exti_mode;
    EXTI_InitStructure.EXTI_Trigger = cfg->exti_trigger;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    /* 中断优先级 */
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                      = cfg->nvic_irqn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = cfg->nvic_pri;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = cfg->nvic_subpri;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    SGCS_INFO("tracking init succeed");
}