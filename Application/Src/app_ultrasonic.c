#include <stdio.h>
#include "app_ultrasonic.h"
#include "bsp_ultrasonic.h"
#include "PeripheralParamConfig.h"
#include "app_message.h"

static AppStatusTypeDef App_UltInit(void);
static uint16_t App_GetDistance(void);
static AppStatusTypeDef App_Trigger(void);
static AppStatusTypeDef App_Print(void);
static AppStatusTypeDef App_Scan(void);

AppUltObjectTypeDef App_Ult = {
    .Init = App_UltInit,
    .GetDistance = App_GetDistance,
    .Trigger = App_Trigger,
    .Scan = App_Scan,
    .Print = App_Print,
};
/**
 * @brief 超声波句柄
 * 
 */
ult_handle_t ult;

/**
 * @brief 超声波初始化
 * 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_UltInit(void)
{
    bsp_ultrasonic_init(&ult, &ULT_DEFAULT_CONDFIG());
    return APP_OK;
}
/**
 * @brief 获取距离
 * 
 * @return uint16_t 
 */
static uint16_t App_GetDistance(void)
{
    uint16_t distance = 0;
    distance = ult.distance_cm;
    return distance;
}
/**
 * @brief 触发
 * 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_Trigger(void)
{
    bsp_ultrasonic_trigger(&ult);
    return APP_OK;
}
/**
 * @brief 扫描
 * 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_Scan(void)
{
    uint8_t buf[24] = {0};
    if (ult.state) /* 准备完成 */
    {
        App_Ult.distance = App_GetDistance();
        sprintf((char *)buf,"[Ultrasonic] Distance:%d\r\n",App_GetDistance());
        App_Message.Send(buf);
    }
    else
    {
        App_Trigger();
    }
}
/**
 * @brief 打印
 * 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_Print(void)
{
    uint8_t buf[24] = {0};
    bsp_ultrasonic_trigger(&ult);
    sprintf((char *)buf,"[Ultrasonic] Distance:%d\r\n",bsp_ultrasonic_get_distance(&ult));
    App_Message.Send(buf);
    return APP_OK;
}