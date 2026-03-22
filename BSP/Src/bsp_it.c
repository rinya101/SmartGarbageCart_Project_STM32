#include "bsp_it.h"
/*------------------------ 编码器 -----------------------------*/
extern void encoder_tim_irq_handler(void* param);
extern void encoder_btn_irq_handler(void* param);
extern void encoder_left_irq_handler(void* param);
extern void oled_dma_irq_handler(void* param);
/*------------------------ 循迹 -------------------------------*/
extern void tracker_a_exti_handler(void* param);
extern void tracker_b_exti_handler(void* param);
/*------------------------ 超声波 -----------------------------*/
extern void ult_first_eh_exti_handler(void* param);
extern void ult_second_eh_exti_handler(void* param);
extern void ult_third_eh_exti_handler(void* param);
extern void ult_tim_handler(void* param);
/*-------------------------- 串口 -----------------------------*/
extern void main_message_irq_handler(void *param);
extern void DMA_main_message_irq_handler(void* param);
/**
 * @brief EXTI1 中断处理函数
 * 
 */
void EXTI1_IRQHandler(void)
{
    ult_first_eh_exti_handler((void*)"EXTI1_IRQHandler");
}
/**
 * @brief EXTI2 中断处理函数
 * 
 */
void EXTI2_IRQHandler(void)
{
    ult_second_eh_exti_handler((void*)"EXTI2_IRQHandler");
}
/**
 * @brief EXTI3 中断处理函数
 * 
 */
void EXTI3_IRQHandler(void)
{
    ult_third_eh_exti_handler((void*)"EXTI3_IRQHandler");
}
/**
 * @brief EXTI4 中断处理函数
 * 
 */
void EXTI4_IRQHandler(void)
{
    tracker_a_exti_handler((void*)"EXTI4_IRQHandler");
}
/**
 * @brief EXTI9_5 中断处理函数
 * 
 */
void EXTI9_5_IRQHandler(void)
{
    tracker_b_exti_handler((void*)"EXTI9_5_IRQHandler");
}
/**
 * @brief EXTI15-10 中断处理函数
 * @note 1. 编码器 btn 中断
 *       2. 编码器中断
 */
void EXTI15_10_IRQHandler(void)
{
    encoder_btn_irq_handler((void*)"EXTI15_10_IRQHandler");
    encoder_left_irq_handler((void*)"EXTI15_10_IRQHandler");
}
/**
 * @brief USART1 中断处理函数
 * 
 */
void USART1_IRQHandler(void)
{
    main_message_irq_handler((void*)"USART1_IRQHandler");
}
/**
 * @brief TIM3 中断处理函数
 * 
 */
void TIM3_IRQHandler(void)
{
    ult_tim_handler((void*)"TIM3_IRQHandler");
}
/**
 * @brief TIM5 中断处理函数
 * @note 1. 编码器定时器中断
 */
void TIM5_IRQHandler(void)
{
    encoder_tim_irq_handler((void*)"TIM5_IRQHandler");
}
/**
 * @brief DAM1_Channel7 中断处理函数
 * 
 */
void DMA1_Channel7_IRQHandler(void)
{
    oled_dma_irq_handler((void*)"DMA1_Channel7_IRQHandler");
}
/**
 * @brief  DAM2_Stream7 中断处理函数
 * 
 */
void DMA2_Stream7_IRQHandler(void)
{
    DMA_main_message_irq_handler((void*)"DMA2_Stream7_IRQHandler");
}