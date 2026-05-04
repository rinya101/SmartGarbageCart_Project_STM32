#include "app_servo.h"
#include "bsp_servo.h"
#include "PeripheralParamConfig.h"
#include "app_message.h"
#include <stdio.h>
/**
 * @brief 舵机方法
 * 
 * @return AppServoObjectTypeDef* 
 */
static AppServoObjectTypeDef* App_ServoInit(void);
static AppServoObjectTypeDef* App_ServoSetAngle(uint8_t id, float angle);
static AppServoObjectTypeDef* App_ServoGetAngle(uint8_t id, float* angle);
static AppServoObjectTypeDef* App_ServoSetUs(uint8_t id, uint16_t us);
static AppServoObjectTypeDef* App_ServoPrint(void);
static servo_handle_t* GetHandle(uint8_t id);
/**
 * @brief 舵机句柄
 * 
 */
servo_handle_t servo1;
servo_handle_t servo2;
servo_handle_t servo3;

/**
 * @brief 舵机实例
 * 
 */
AppServoObjectTypeDef App_Servo = {
    .Init      = App_ServoInit,
    .SetAngle  = App_ServoSetAngle,
    .GetAngle  = App_ServoGetAngle,
    .SetUs     = App_ServoSetUs,
    .Print     = App_ServoPrint
};
/**
 * @brief 通过id获取句柄
 * 
 * @param id 
 * @return servo_handle_t* 
 */
static servo_handle_t* GetHandle(uint8_t id)
{
    switch(id)
    {
        case 1:
            return &servo1;
        case 2:
            return &servo2;
        case 3:
            return &servo3;
        default:
            return (servo_handle_t*)0;
    }
}
/**
 * @brief 舵机初始化
 * 
 * @return AppServoObjectTypeDef* 
 */
static AppServoObjectTypeDef* App_ServoInit(void)
{
    bsp_servo_init(&servo1, &SERVO1_DEFAULT_CONFIG());
    bsp_servo_init(&servo2, &SERVO2_DEFAULT_CONFIG());
    bsp_servo_init(&servo3, &SERVO3_DEFAULT_CONFIG());
    return &App_Servo;
}
/**
 * @brief 设定舵机角度
 * 
 * @param id 
 * @param angle 
 * @return AppServoObjectTypeDef* 
 */
static AppServoObjectTypeDef* App_ServoSetAngle(uint8_t id, float angle)
{
    if(id <1 || id>3) return &App_Servo;
    
    bsp_servo_set_angle(GetHandle(id), angle);
    return &App_Servo;
}
/**
 * @brief 获取舵机角度
 * 
 * @param id 
 * @param angle 
 * @return AppServoObjectTypeDef* 
 */
static AppServoObjectTypeDef* App_ServoGetAngle(uint8_t id, float* angle)
{
    if(id <1 || id>3) return &App_Servo;
    servo_handle_t* servo_handle = GetHandle(id);
    bsp_servo_get_angle(servo_handle, angle);
    return &App_Servo;
}
/**
 * @brief 通过微秒数设定舵机角度
 * 
 * @param id 
 * @param us 
 * @return AppServoObjectTypeDef* 
 */
static AppServoObjectTypeDef* App_ServoSetUs(uint8_t id, uint16_t us)
{
    if(id <1 || id>3) return &App_Servo;
    bsp_servo_set_us(GetHandle(id), us);
    return &App_Servo;
}
/**
 * @brief 输出舵机当前状态角度
 * 
 * @return AppServoObjectTypeDef* 
 */
static AppServoObjectTypeDef* App_ServoPrint(void)
{
    uint8_t buf[32];
    sprintf((char*)buf, "[Servo] Servo1: %.2f, Servo2: %.2f, Servo3: %.2f", 
    GetHandle(1)->angle, 
    GetHandle(2)->angle,
    GetHandle(3)->angle);
    App_Message.Send(buf);
    return &App_Servo;
}