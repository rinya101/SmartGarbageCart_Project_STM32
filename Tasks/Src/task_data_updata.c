#include "task_data_updata.h"
#include "app_message.h"
#include "app_esp32_msg.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app_compass.h"
#include "app_ultrasonic.h"
#include "app_tracking.h"
#include "app_motor.h"
/**
 * @brief 任务函数，用于更新数据
 * 
 * @param pvParameters 
 */
void task_data_updata(void *pvParameters)
{
    while(1)
    {
        /* 指南针数据更新 */
        App_Compass.Read();
        /* 超声波数据更新 */
        App_Ult.Trigger();
        printf("Dis :%d\t Angle:%d\n",App_Ult.GetDistance(), App_Compass.Read());
        vTaskDelay(500);
    }
}