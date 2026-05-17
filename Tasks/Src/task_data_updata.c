#include "task_data_updata.h"
#include "app_message.h"
#include "app_esp32_msg.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app_compass.h"
#include "app_ultrasonic.h"
#include "app_tracking.h"
#include "app_battery.h"
#include "app_motor.h"
#include "Encode.h"
#include "app_car.h"
void Send_Updata(void);
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
        /* 串口打印 */
        printf("Dis :%d\t Angle:%d\t Bat:%f \n",App_Ult.GetDistance(), App_Compass.Read(), App_Battery.GetLevel());
        /* 指令发送 */
        Send_Updata();
        vTaskDelay(200);
    }
}
/**
 * @brief 发送状态数据
 * 
 */
void Send_Updata(void)
{
    Decode_Cmd_t cmd;
    /* 电子罗盘角度发送 */
    cmd.cmdCode = CMD_PC_COMPASS_ANGLEUPLOAD; 
    strcpy((char*)cmd.key, "ANGLE");
    sprintf((char*)cmd.value, "%d", App_Compass.Read());
    App_ESP32Msg.SendCmd(&cmd);
    memset(&cmd, 0, sizeof(Decode_Cmd_t));
    /* 超声波距离发送 */
    cmd.cmdCode = CMD_PC_ULTRASONIC_DISTANCEUPLOAD;
    strcpy((char*)cmd.key, "DISTANCE");
    sprintf((char*)cmd.value, "%d", App_Ult.GetDistance());
    App_ESP32Msg.SendCmd(&cmd);
    memset(&cmd, 0, sizeof(Decode_Cmd_t));
    /* 电池电量发送 */
    cmd.cmdCode = CMD_PC_BAT_PERCENTUPLOAD;
    strcpy((char*)cmd.key, "PERCENT");
    sprintf((char*)cmd.value, "%.2f", App_Battery.GetLevel());
    App_ESP32Msg.SendCmd(&cmd);
    memset(&cmd, 0, sizeof(Decode_Cmd_t));
    /* 发送小车状态 */
    cmd.cmdCode = CMD_PC_CAR_STA;
    strcpy((char*)cmd.key, "STA");
    sprintf((char*)cmd.value, "%s", App_Car.mode_str);
    App_ESP32Msg.SendCmd(&cmd);
    memset(&cmd, 0, sizeof(Decode_Cmd_t));
}