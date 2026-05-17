#include "task_control.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "Decode.h"
#include "app_car.h"
#include "app_buzzer.h"

/**
 * @brief 小车控制任务（核心）
 * 
 * @param pvParameters 
 */
void task_control(void *pvParameters)
{
    /* 初始化为 PC 控制 */
    App_Car.SetMode(CAR_MODE_PC_CONTROL);
    App_Buzzer.Beep(60000);
    while(1)
    {
        App_Car.Run();
        vTaskDelay(2);/* 延时2ms 释放 */
    }
}