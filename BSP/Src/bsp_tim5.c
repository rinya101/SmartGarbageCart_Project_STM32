/*******************************************************************************
 *      文    件: bsp_tim5.h
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 TIM5 定时器配置
 *      特别说明： 本文件与 bsp_encoder.h 交叉引用，使用是注意引用 bsp_encoder.h
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#include "bsp_tim5.h"
#include "bsp_debug.h"
/**
 * @brief TIM5 句柄
 * 
 */
tim5_handle_t g_tim5_handle;

/**
 * @brief TIM5 默认回调函数
 * 
 * @param handle 
 */
static __weak void tim5_default_callback(tim5_handle_t* handle)
{
    TIM_Cmd(handle->tim5, ENABLE);
    if (handle->user_id == TIM5_USED_ENCODER_ID)
    {
        g_encoder->btn_long_press(g_encoder);
    }
}
/**
 * @brief TIM5 中断服务函数
 * 
 */
void TIM5_IRQHandler(void)
{
    if (TIM_GetITStatus(g_tim5_handle.tim5, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(g_tim5_handle.tim5, TIM_IT_Update);
        g_tim5_handle.callback(&g_tim5_handle);
    }
}
/**
 * @brief TIM5 初始化
 * 
 * @param handle 
 * @param cfg 
 */
void bsp_tim5_init(tim5_handle_t* handle, tim5_cfg_t cfg)
{
    if (handle == NULL)
    {
        SGCS_ERROR("handle is null");
        return;
    }
    /* 参数拷贝 */
    handle->tim5 = cfg.tim5;
    /* 时钟使能 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    /* 配置定时器 */
    TIM_DeInit(cfg.tim5);
    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    TIM_InitStructure.TIM_Prescaler     = cfg.prescaler;
    TIM_InitStructure.TIM_Period        = cfg.period;
    TIM_InitStructure.TIM_CounterMode   = cfg.counter_mode;
    TIM_InitStructure.TIM_ClockDivision = cfg.division;
    TIM_TimeBaseInit(cfg.tim5, &TIM_InitStructure);
    /* 开启中断 */
    TIM_ITConfig(cfg.tim5, TIM_IT_Update, ENABLE);
    /* 使能定时器 - 暂时不开启 */
    //TIM_Cmd(cfg.tim5, ENABLE);
    /* NVIC */
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel                   = cfg.irq_channel;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = cfg.irq_prepriority;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority        = cfg.irq_subpriority;
    NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    SGCS_INFO("tim5 init success");
}
/**
 * @brief TIM5 挂载回调函数
 * 
 * @param handle 
 * @param callback 
 */
void bsp_tim5_attach_callback(tim5_handle_t* handle)
{
    if (handle == NULL)
    {
        SGCS_ERROR("handle is NULL");
        return;
    }
    handle->callback = tim5_default_callback;
}

/**
 * @brief 启动定时器5定时，定时时间到会触发中断回调
 * @note  时钟配置：100MHz / 1000 = 100kHz → 每计数1次 = 10us
 * @note  计算公式：定时时间(ms) = period / 100
 * @note  示例：
 *          period = 1000   → 10ms
 *          period = 3000   → 30ms
 *          period = 30000  → 300ms
 *          period = 8000   → 80ms
 *          perido = 65535  → 655.35ms (最大值)
 *          period = 80000  → 超出范围！最大 65535
 * @param handle 定时器句柄
 * @param user_id 用户id，用于区分定时器被谁使用
 *     @arg TIM5_USED_XXX_ID 用户自定义 XXX作为区分
 */
void bsp_tim5_timing(uint16_t period, uint8_t user_id)
{
    if(period == 0 || period > 65535)
    {
        SGCS_ERROR("period is invalid preiod = %d", period);
        return;
    }
    tim5_handle_t* handle = &g_tim5_handle;
    handle->user_id = user_id;
    TIM_Cmd(handle->tim5, DISABLE);      
    TIM_SetCounter(handle->tim5, 0);    
    TIM_SetAutoreload(handle->tim5, period);  
    TIM_Cmd(handle->tim5, ENABLE);
}
/**
 * @brief TIM5 停止
 * 
 */
void bsp_tim5_stop(void)
{
    TIM_Cmd(g_tim5_handle.tim5, DISABLE);
    TIM_SetCounter(g_tim5_handle.tim5, 0);
}