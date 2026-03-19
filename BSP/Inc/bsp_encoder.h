/*******************************************************************************
 *      文    件: bsp_encoder.c
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 编码器驱动
 *      版    本: V1.1
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
 * 私有说明：1. 文件已经定义 g_encoder 句柄指针，使用时先分配内存，再初始化
 *          2. 关于 void encoder_attach_callback(encoder_cfg_t *cfg) 函数，
 *             可以自行查看修改回调函数，自定义操作（V1.1 开始不支持修改）
 *          3. 关于 void encoder_attach_event(encoder_cfg_t *cfg) 函数，
 *             主要包含编码器的事件（左转、右转、按下、长按）
 *             可以自行查看修改回调函数，自定义操作
 *          4. 使用顺序：
 *              [1] void encoder_attach_callback(encoder_cfg_t *cfg) 
 *              或者 配置 cfg-> event_cb、cfg -> btn_cb
 *              [2] void encoder_attach_event(encoder_cfg_t *cfg)
 *              或者 配置 cfg-> left_event、cfg-> right_event、cfg-> btn_press、
 *      cfg-> btn_long_press、
 *              [3] void encoder_init(encoder_handle_t* handle, encoder_cfg_t cfg)
 * （编者：❤️❤️保姆教程❤️❤️）
*******************************************************************************/
#ifndef _BSP_ENCODER_H
#define _BSP_ENCODER_H
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "bsp_pin_cfg.h"
#include "bsp_debug.h"
typedef struct encoder_handle encoder_handle_t;
/**
 * @brief 编码器回调函数
 * 
 */
typedef void (*encoder_cb_t)(encoder_handle_t *handle);
/**
 * @brief 编码器按键回调函数
 * 
 */
typedef void (*encoder_btn_cb_t)(encoder_handle_t *handle);
/**
 * @brief TIM5 回调函数
 * 
 */
typedef void (*tim5_callback_t)(encoder_handle_t* handle);
/**
 * @brief 编码器左转事件
 * 
 */
typedef void (*encoder_left_event)(void* param);
/**
 * @brief 编码器右转事件
 * 
 */
typedef void (*encoder_right_event)(void* param);
/**
 * @brief 编码器按键按下事件
 * 
 */
typedef void (*btn_press_event)(void* param);
/**
 * @brief 编码器按键长按事件
 * 
 */
typedef void (*btn_long_press_event)(void* param);
/**
 * @brief 按键状态枚举
 * 
 */
typedef enum 
{
    UNKNOW = 0,
    PRESS = 1,
    RELEASE = -1,
    LONG_PRESS = 2
} key_state_e;
/**
 * @brief 转向枚举
 * 
 */
typedef enum 
{
    STOP = 0,
    LEFT = -1,
    RIGHT = 1
} encoder_turn_t;

/**
 * @brief 编码器配置结构体
 * 
 */
typedef struct 
{
    /* RCC */
    uint32_t                btn_clk;
    uint32_t                left_clk;
    uint32_t                right_clk;
    uint32_t                syscfg_clk;
    uint32_t                tim_clk;
    /* GPIO */
    GPIO_TypeDef*           btn_port;
    uint32_t                btn_pin;
    GPIO_TypeDef*           left_port;
    uint32_t                left_pin;
    GPIO_TypeDef*           right_port;
    uint32_t                right_pin;
    /* TIM */
    TIM_TypeDef*            timx;
    uint16_t                tim_prescaler;
    uint16_t                tim_period;
    uint16_t                tim_counter_mode;
    uint16_t                tim_division;
    /* EXIT */
    uint32_t                btn_exti_line;    
    EXTITrigger_TypeDef     btn_trigger;
    uint32_t                left_exti_line;
    EXTITrigger_TypeDef     left_trigger;
    /* NVIC */
    uint8_t                 btn_irq_channel;
    uint8_t                 left_irq_channel;
    uint8_t                 preemption_priority;
    uint8_t                 sub_priority;
    uint8_t                 tim_irq_channel;
    uint8_t                 tim_irq_prepriority;
    uint8_t                 tim_irq_subpriority;
    /* ENCODER */
    uint16_t                press_long_time;
    /* CallBack */
    encoder_cb_t            event_cb;
    encoder_btn_cb_t        btn_cb;
    tim5_callback_t         tim_cb;
    /* EVENT */
    encoder_left_event      left_event;
    encoder_right_event     right_event;
    btn_press_event         btn_press;
    btn_long_press_event    btn_long_press;

} encoder_cfg_t;


/**
 * @brief 编码器句柄
 * 
 */
struct encoder_handle
{
    /* GPIO */
    GPIO_TypeDef*           btn_port;
    uint32_t                btn_pin;
    GPIO_TypeDef*           left_port;
    uint32_t                left_pin;
    GPIO_TypeDef*           right_port;
    uint32_t                right_pin;
    /* ENCODER */
    uint8_t                 dir;
    int16_t                 count;
    key_state_e             btn_state;
    encoder_turn_t          turn_state;
    uint16_t                press_long_time;
    /* EXIT line */
    uint32_t                btn_exti_line;
    uint32_t                left_exti_line;
    /* CallBack */
    encoder_cb_t            encoder_cb;
    encoder_btn_cb_t        btn_cb;
    tim5_callback_t         tim_cb;
    /* TIM */
    TIM_TypeDef*            timx;
    /* EVENT */
    encoder_left_event      left_event;
    encoder_right_event     right_event;
    btn_press_event         btn_press_event;
    btn_long_press_event    btn_long_press_event;
};
/* 注意使用顺序 如下 */
void encoder_attach_callback(encoder_cfg_t *cfg);
void encoder_attach_event(encoder_cfg_t *cfg);
void encoder_init(encoder_handle_t* handle, encoder_cfg_t* cfg);
#endif