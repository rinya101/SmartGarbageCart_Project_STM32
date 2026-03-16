/*******************************************************************************
 *      文    件: bsp_tim5.h
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 TIM5 定时器配置
 *      特别说明： 本文件与 bsp_encoder.h 交叉引用，使用是注意引用 bsp_encoder.h
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
 * 私有说明：
 *      1. 本文件内部已经定义好定时器5句柄 g_tim5_handle
 *      2. 如果使用bsp_encoder.h 中的初始化函数，
 *          则不需要再初始化定时器5（内部已经初始化完成）
 *      3. 回调函数根据需要进行修改，注意内部使用ID与使用者对应（格外注意！！）
 *      4. 使用顺序：
 *          [1] void bsp_tim5_attach_callback(tim5_handle_t* handle) 
 *             或者 自定义：cfg-> callback 为自定义回调函数
 *          [2] void bsp_tim5_init(tim5_handle_t* handle, tim5_cfg_t cfg)
*******************************************************************************/
#ifndef _BSP_TIM5_H
#define _BSP_TIM5_H
#include "stm32f4xx_tim.h"
#include "bsp_encoder.h"
#include "PeripheralParamConfig.h"
typedef struct tim5_handle tim5_handle_t;
extern tim5_handle_t g_tim5_handle;
/**
 * @brief TIM5 回调函数
 * 
 */
typedef void (*tim5_callback_t)(tim5_handle_t* handle);

/**
 * @brief tim5 配置结构体
 * 
 */
typedef struct 
{
    /* TIM */
    TIM_TypeDef*    tim5;
    uint16_t        prescaler;
    uint16_t        period;
    uint16_t        counter_mode;
    uint16_t        division;
    /* NVIC */
    uint8_t         irq_channel;
    uint8_t         irq_prepriority;
    uint8_t         irq_subpriority;
    /* CallBack */
    tim5_callback_t callback; 
} tim5_cfg_t;
/**
 * @brief tim5 句柄
 * 
 */
struct  tim5_handle
{
    TIM_TypeDef*        tim5;
    tim5_callback_t     callback;
    uint8_t             user_id;
};
/* -------------------------- TIM5 配置（注意使用顺序） ---------------------- */
void bsp_tim5_attach_callback(tim5_handle_t* handle);
void bsp_tim5_init(tim5_handle_t* handle, tim5_cfg_t cfg);
/* ---------------------------------- TIM5 使用 ----------------------------- */
void bsp_tim5_timing(uint16_t period, uint8_t user_id);
void bsp_tim5_stop(void);
#endif