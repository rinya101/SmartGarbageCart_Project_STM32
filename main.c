/**
 * @file main.c
 * @brief 系统入口文件
 */

#include "stm32f4xx.h"
#include "SysClockConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app_message.h"
#include "app_battery.h"
#include "app_oled.h"
#include "task_app.h"
void led_init(void);

int main(void)
{
    /** 时钟配置 */
    SysClockConfig_HSE25MHZ();
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
    led_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /** APP 层初始化 */
    App_Message.Init();
    App_Message.Send("Message Init Ok\r\n", strlen("Message Init Ok\r\n"));
    App_Battery.Init();
    App_Battery.Print();
    App_Oled.Init();
    App_Oled.Clear();
    /** 创建业务任务 */
    xTaskCreate(task_app, "task_app", 1024, NULL, 4, NULL);

    /** 启动调度器 */
    vTaskStartScheduler();

    while(1)
    {

    }
}

/**
 * @brief LED初始化
 */
void led_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}