/**
 * @file main.c
 * @brief 系统入口文件
 */

#include "stm32f4xx.h"
#include "SysClockConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "app_bsp_install.h"
#include "app_message.h"
#include "app_esp32_msg.h"
#include "app_oled.h"
#include "app_battery.h"
#include "app_compass.h"
#include "app_buzzer.h"
#include "app_ultrasonic.h"
#include "app_encoder.h"
#include "app_motor.h"
#include "app_servo.h"
#include "app_tracking.h"
#include "task_app.h"
#include "PeripheralParamConfig.h"
#include "bsp_oled.h"
#include "task_data_updata.h"
#include "task_control.h"


void led_init(void);
int main(void)
{
    /** 时钟配置 */
    SysClockConfig_HSE25MHZ();
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
    led_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    /** APP 层初始化 */
    /* 消息初始化 */
    App_Message.Init();
    App_Message.Send("Message Init Ok\r\n");
    App_ESP32Msg.Init();
    App_ESP32Msg.Send("ESP32 Init Ok\r\n");
    /* 电池测量初始化 */
    App_Battery.Init();
    App_Battery.Print();
    /* 罗盘初始化 */
    App_Compass.Init();
    // App_Compass.Calibrate();
    /* 蜂鸣器初始化 */
    App_Buzzer.Init();
    App_Buzzer.SetPeriod(1000);
    App_Buzzer.SetDuty(500);
    /* 超声波初始化 */
    App_Ult.Init();
    App_Ult.Print();
    /* 旋转编码器初始化 */
    App_Encoder.Init();
    /* 马达初始化 */
    App_Motor.Init()->Stop();
    /* 舵机初始化 */
    App_Servo.Init()->SetAngle(1, 45)->SetAngle(2, 45)->SetAngle(3, 45);
    /* 循迹初始化 */
    App_Tracking.Init();
    /* OLED */
    OLED_DEFAULT_CONFIG(); /* 修复显示屏错行问题 */
    App_Oled.Init();
    App_Oled.Welcome();
    /* 小车控制任务（核心） */
    xTaskCreate(task_control, "task_control", 2048, NULL, 6, NULL);
    /* 杂项任务（OLED显示） */
    xTaskCreate(task_app, "task_app", 2028, NULL, 5, NULL);
    /* 数据更新任务 */
    xTaskCreate(task_data_updata, "task_data_updata", 1024, NULL, 4, NULL);
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