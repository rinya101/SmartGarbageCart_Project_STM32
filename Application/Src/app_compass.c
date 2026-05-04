#include <stdio.h>
#include "app_compass.h"
#include "bsp_compass.h"
#include "app_message.h"
#include "PeripheralParamConfig.h"
#include "app_motor.h"
#include "app_buzzer.h"
#include "app_esp32_msg.h"
static AppStatusTypeDef App_CompassInit(void);
static uint16_t App_CompassRead(void);
static AppStatusTypeDef App_CompassCalibration(void);
static AppStatusTypeDef App_CompassPrint(void);
/**
 * @brief 罗盘实例
 * 
 */
AppCompassObjectTypeDef App_Compass = {
    .Init = App_CompassInit,
    .Read = App_CompassRead,
    .Calibrate = App_CompassCalibration,
    .Print = App_CompassPrint,
};
/**
 * @brief 罗盘句柄单例程
 * 
 */
static compass_handle_t compass = {0};

/**
 * @brief 初始化罗盘
 * 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_CompassInit(void)
{
    bsp_compass_init(&compass,&COMPASS_DEFAULT_CONFIG());
    return APP_OK;
}
/**
 * @brief 读取罗盘角度
 * 
 * @param angle 
 * @return uint16_t 
 */
static uint16_t App_CompassRead(void)
{
    uint16_t angle_temp = 0;
    bsp_compass_read(&compass);
    angle_temp = compass.data->angle;
    return angle_temp;
}
/**
 * @brief 校准罗盘
 * 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_CompassCalibration(void)
{
    App_Buzzer.Beep(60000);
    App_Motor.TurnRight()->Middle();
    bsp_compass_calibrate(&compass);
    App_Motor.Stop();
    App_Buzzer.Beep(60000);
    return APP_OK;
}
/**
 * @brief 打印罗盘角度
 * 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_CompassPrint(void)
{
    uint16_t angle = 0;
    uint8_t buf[24] = {0};
    bsp_compass_read(&compass);
    angle = compass.data->angle;
    sprintf((char *)buf,"[Compass] Angle:%d\r\n",angle);
    App_Message.Send(buf);
    return APP_OK;
}