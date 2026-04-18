/*******************************************************************************
 *      文    件: bsp_tracking.h
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 循迹 模块驱动
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
 * 私有说明：
 *      1. 代码内部仅仅提供 A B 两路循迹的中断服务函数 剩余的三路根据实际情况自行添加
 *      2. 必须注意，中断服务函数 5-9 为同一个服务函数
 *                 中断服务函数 10-15 为同一个服务函数
 *      3. 当然你也可以不使用中断，直接轮询，获取状态
 *      4. 不提供 状态变化事件 的回调函数，自行实现
*******************************************************************************/
#ifndef _BSP_TRACKING_H
#define _BSP_TRACKING_H
#include "stm32f4xx_gpio.h"
typedef struct track_handle track_handle_t;
/**
 * @brief 循迹模块ID枚举
 * 
 */
typedef enum
{
    TRACKER_A,
    TRACKER_B,
    TRACKER_C,  
    TRACKER_D,
    TRACKER_E,
} track_id_t;
/**
 * @brief 循迹模块状态枚举
 * 
 */
typedef enum
{
    TRACKER_STATE_NONE = -1,
    TRACKER_STATE_WHITE = 0,
    TRACKER_STATE_BLACK = 1,
}track_state_t;

/**
 * @brief 循迹初始化
 * 
 */
typedef struct 
{
    /* TRACKER */
    track_id_t          id;
    /* RCC */
    uint32_t            rcc_gpio;
    /* GPIO */
    GPIO_TypeDef*       gpio_port;
    uint32_t            gpio_pin;
    GPIOMode_TypeDef    gpio_mode;
    GPIOOType_TypeDef   gpio_otype;
    GPIOSpeed_TypeDef   gpio_speed;
    GPIOPuPd_TypeDef    gpio_pupd;
    uint8_t             gpio_port_source;
    uint8_t             gpio_pin_source;
    /* EXTI */
    uint32_t            exti_line;
    EXTIMode_TypeDef    exti_mode;
    EXTITrigger_TypeDef exti_trigger;
    /* NVIC */
    uint8_t             nvic_irqn;
    uint8_t             nvic_pri;
    uint8_t             nvic_subpri;
} track_cfg_t;

/**
 * @brief 循迹设备句柄
 * 
 */
typedef struct 
{
    /* EXTI */
    uint32_t            exti_line;
    /* GPIO */
    GPIO_TypeDef*       gpio_port;
    uint32_t            gpio_pin;
} track_dev_handle_t;

/**
 * @brief 循迹句柄
 * 
 */
struct track_handle
{
    /* BASE */
    track_id_t          id;
    track_state_t       sta;
    /* Dev */
    track_dev_handle_t  dev;
};

void bsp_tracking_init(track_handle_t* handle, track_cfg_t *cfg);

#endif