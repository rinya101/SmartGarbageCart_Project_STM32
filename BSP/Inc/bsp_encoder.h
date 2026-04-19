/*******************************************************************************
 *      文    件: bsp_encoder.h
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 编码器 驱动
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#ifndef _BSP_ENCODER_H_
#define _BSP_ENCODER_H_
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

typedef struct encoder_handle encoder_handle_t;
/**
 * @brief 编码器按键状态
 * 
 */
typedef enum 
{
    PRESS = 1,
    RELEASE = 2,
    LONGPRESS = 3,
} btn_state_t;
/**
 * @brief 编码器配置
 * 
 */
typedef struct
{
    /* RCC 时钟 */
    uint32_t            gpio_a_clk;
    uint32_t            gpio_b_clk;
    uint32_t            gpio_btn_clk;
    uint32_t            tim_clk;

    /* GPIO */
    GPIO_TypeDef        *gpio_a_port;
    GPIO_TypeDef        *gpio_b_port;
    GPIO_TypeDef        *gpio_btn_port;

    uint32_t            gpio_a_pin;
    uint32_t            gpio_b_pin;
    uint32_t            gpio_btn_pin;

    /* SYSCFG 映射 (EXTI 必须) */
    uint8_t             exti_a_port_src;
    uint8_t             exti_a_pin_src;

    uint8_t             exti_btn_port_src;
    uint8_t             exti_btn_pin_src;

    /* EXTI */
    uint32_t            exti_a_line;
    uint32_t            exti_btn_line;

    EXTITrigger_TypeDef exti_a_trigger;
    EXTITrigger_TypeDef exti_btn_trigger;

    /* NVIC */
    uint8_t             nvic_a_priority;
    uint8_t             nvic_a_subpriority;
    uint8_t             nvic_a_irq;

    uint8_t             nvic_btn_priority;
    uint8_t             nvic_btn_subpriority;
    uint8_t             nvic_btn_irq;

    uint8_t             nvic_tim_priority;
    uint8_t             nvic_tim_subpriority;
    uint8_t             nvic_tim_irq;

    /* TIM */
    TIM_TypeDef         *tim;
    uint16_t            tim_psc;
    uint32_t            tim_arr;

} encoder_cfg_t;
/**
 * @brief 编码器句柄
 * 
 */
struct encoder_handle
{
    int32_t count;              /* 计数器 */
    btn_state_t btn_state;      /* 按键状态 */
    struct
    {
        GPIO_TypeDef* a_port;
        GPIO_TypeDef* b_port;
        GPIO_TypeDef* btn_port;
        uint32_t      a_pin;
        uint32_t      b_pin;
        uint32_t      btn_pin;
        TIM_TypeDef*  tim;
        uint16_t      tim_psc;
        uint32_t      tim_arr;
        uint32_t      exti_a_line;
        uint32_t      exti_btn_line;
        uint8_t       pre_debouncing_flag;
        uint8_t       post_debouncing_flag;
        uint8_t       long_press_flag;
        uint8_t       rotate_detection_flag;
    } dev;                      /* 设备，用于存储操作对象，用户不需要访问修改 */
    struct
    {
        void (*press)(const void *param);
        void (*longpress)(const void *param);
        void (*release)(const void *param);
        void (*rotate_clockwise)(const void *param);
        void (*rotate_counterclockwise)(const void *param);
    } event_callback;          /* 事件处理，如果需要在按键按下或者编码器旋转是触发事件，可以定义一个函数作为事件函数，让指针指向这个函数 */
};

void bsp_encoder_init(encoder_handle_t *handle, const encoder_cfg_t *cfg);

#endif