#include "bsp_it.h"
#include "bsp_debug.h"
/*------------------------ 编码器 -----------------------------*/
extern void encoder_tim_irq_handler(void* param);
extern void encoder_btn_irq_handler(void* param);
extern void encoder_left_irq_handler(void* param);
/*------------------------ OLED DMA ---------------------------*/
extern void oled_dma_irq_handler(void* param);
/*------------------------ 循迹 -------------------------------*/
extern void tracker_a_exti_handler(void* param);
extern void tracker_b_exti_handler(void* param);
extern void tracker_c_exti_handler(void* param);
/*------------------------ 超声波 -----------------------------*/
extern void ult_eh_exti_handler(void* param);
extern void ult_tim_handler(void* param);
/*-------------------------- 串口 -----------------------------*/
extern void main_message_irq_handler(void *param);
extern void DMA_main_message_irq_handler(void* param);

/**
 * @brief EXTI0_IRQHandler
 * 
 */
void EXTI0_IRQHandler(void)
{
    //encoder_left_irq_handler((void*)"EXTI0_IRQHandler");
}

/**
 * @brief EXTI2_IRQHandler
 * 
 */
void EXTI2_IRQHandler(void)
{
    //encoder_btn_irq_handler((void*)"EXTI2_IRQHandler");
}
/**
 * @brief EXTI3_IRQHandler
 * 
 */
void EXTI3_IRQHandler(void)
{
    tracker_a_exti_handler((void*)"EXTI3_IRQHandler");
}
/**
 * @brief EXTI4_IRQHandler
 * 
 */
void EXTI4_IRQHandler(void)
{
    tracker_b_exti_handler((void*)"EXTI4_IRQHandler");
}
/**
 * @brief EXTI9_5_IRQHandler
 * @note 超声波中断服务
 * 
 */
void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
        tracker_c_exti_handler((void*)"EXTI9_5_IRQHandler_5");
    }
    if(EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
        ult_eh_exti_handler((void*)"EXTI9_5_IRQHandler_7");
    }
}
/**
 * @brief TIM3_IRQHandler
 * @note 超声波定时器中断服务
 */
void TIM3_IRQHandler(void)
{
    ult_tim_handler((void*)"TIM3_IRQHandler");
}

/**
 * @brief TIM5_IRQHandler
 * 
 */
void TIM5_IRQHandler(void)
{
    //encoder_tim_irq_handler((void*)"TIM5_IRQHandler");
}
/**
 * @brief USART1_IRQHandler
 * 
 */
void USART1_IRQHandler(void)
{
    main_message_irq_handler((void*)"USART1_IRQHandler");
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