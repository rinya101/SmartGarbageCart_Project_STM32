#include "app_encoder.h"
#include "bsp_encoder.h"
#include "PeripheralParamConfig.h"
#include "app_message.h"
#include "app_buzzer.h"
#include "app_servo.h"
static AppStatusTypeDef App_EncoderInit(void);
static int32_t App_EncoderGetValue(void);
AppStatusTypeDef App_EncoderPressEvent(void);


AppEncoderObjectTypeDef App_Encoder = {
    .Init = App_EncoderInit,
    .GetVlaue = App_EncoderGetValue,
};

encoder_handle_t encoder;
/**
 * @brief 按下处理函数
 * 
 * @param param 
 */
void OnPressEvent(const void* param)
{
    App_Buzzer.Beep(5000);
    App_Message.Send("[Encoder] On Press\r\n");
}
/**
 * @brief 释放处理函数
 * 
 * @param param 
 */
void OnReleaseEvent(const void* param)
{
    //App_Buzzer.Beep(5000);
    App_Message.Send("[Encoder] On Release\r\n");
}
/**
 * @brief 长按处理函数
 * 
 * @param param 
 */
void OnLongPressEvent(const void* param)
{
    App_Buzzer.Beep(30000);
    App_Message.Send("[Encoder] On LongPress\r\n");


}
/**
 * @brief 编码器顺时针旋转
 * 
 * @param param 
 */
void OnRotateClockwiseEvent(const void* param)
{
    App_Buzzer.Beep(2000);
    App_Message.Send("[Encoder] On Rotate Clockwise\r\n");
    float angle = 0;
    App_Servo.GetAngle(1, &angle);
    angle -= 10;
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    App_Servo.SetAngle(1, angle);
}
/**
 * @brief 编码器逆时针旋转
 * 
 * @param param 
 */
void OnRotateAntiClockwiseEvent(const void* param)
{
    App_Buzzer.Beep(2000);
    App_Message.Send("[Encoder] On Rotate AntiClockwise\r\n");
    float angle = 0;
    App_Servo.GetAngle(1, &angle);
    angle += 10;
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    App_Servo.SetAngle(1, angle);
}
/**
 * @brief 旋转编码器初始化
 * 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_EncoderInit(void)
{
    encoder.event_callback.press = OnPressEvent;
    encoder.event_callback.release = OnReleaseEvent;
    encoder.event_callback.longpress = OnLongPressEvent;
    encoder.event_callback.rotate_clockwise = OnRotateClockwiseEvent;
    encoder.event_callback.rotate_counterclockwise = OnRotateAntiClockwiseEvent;
    bsp_encoder_init(&encoder, &ENCODER_DEFAULT_CONFIG());
    return APP_OK;
}
/**
 * @brief 读取编码器值
 * 
 * @return int32_t 
 */
static int32_t App_EncoderGetValue(void)
{
    return encoder.count;
}
