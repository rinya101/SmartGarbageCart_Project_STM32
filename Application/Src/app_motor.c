#include "app_motor.h"
#include "bsp_motor.h"
#include "PeripheralParamConfig.h"
#include <math.h>
#include "app_compass.h"
#include "app_esp32_msg.h"
/**
 * @brief 电机方法声明
 * 
 * @return AppMotorObjectTypeDef* 
 */
static AppMotorObjectTypeDef* App_Motor_Init(void);
static AppStatusTypeDef App_MotorSetSpeed(uint16_t speed);
static AppMotorObjectTypeDef* App_Motor_Forward(void);
static AppMotorObjectTypeDef* App_Motor_Backward(void);
static AppMotorObjectTypeDef* App_Motor_Stop(void);
static AppMotorObjectTypeDef* App_Motor_TurnLeft(void);
static AppMotorObjectTypeDef* App_Motor_TurnRight(void);
static AppMotorObjectTypeDef* App_Motor_Orientation(uint16_t angle);

static AppStatusTypeDef App_Motor_Speed_Low(void);
static AppStatusTypeDef App_Motor_Speed_Middle(void);
static AppStatusTypeDef App_Motor_Speed_High(void);

/**
 * @brief 电机全局单例
 * 
 */
motor_handle_t motor;

/**
 * @brief 电机方法实现
 * 
 */
AppMotorObjectTypeDef App_Motor = {
    .Init      = App_Motor_Init,
    .Forward   = App_Motor_Forward,
    .Backward  = App_Motor_Backward,
    .Stop      = App_Motor_Stop,
    .TurnLeft  = App_Motor_TurnLeft,
    .TurnRight = App_Motor_TurnRight,
    .Low       = App_Motor_Speed_Low,
    .Middle    = App_Motor_Speed_Middle,
    .High      = App_Motor_Speed_High,
    .SetSpeed  = App_MotorSetSpeed,
    .Orientation = App_Motor_Orientation,
};

/**
 * @brief 电机初始化
 * 
 * @return AppMotorObjectTypeDef* 
 */
static AppMotorObjectTypeDef* App_Motor_Init(void)
{
    bsp_motor_init(&motor, &MOTOR_DEFAULT_CONFIG());
    App_Motor.speed = 500;
    return &App_Motor;
}

static AppStatusTypeDef App_MotorSetSpeed(uint16_t speed)
{
    App_Motor.speed = speed;
    if (motor.speed1 > 0) motor.speed1 = App_Motor.speed; else  motor.speed1 = -App_Motor.speed;
    if (motor.speed2 > 0) motor.speed2 = App_Motor.speed; else  motor.speed2 = -App_Motor.speed;
    bsp_motor_set_speed(&motor, motor.speed1, motor.speed2);
    return APP_OK;
}
/**
 * @brief 电机（小车）前进
 * 
 * @return AppMotorObjectTypeDef* 
 */
static AppMotorObjectTypeDef* App_Motor_Forward(void)
{
    motor.speed1 = App_Motor.speed;
    motor.speed2 = App_Motor.speed;
    bsp_motor_set_speed(&motor, motor.speed1, motor.speed2);
    return &App_Motor;
}
/**
 * @brief 电机（小车）后退
 * 
 * @return AppMotorObjectTypeDef* 
 */
static AppMotorObjectTypeDef* App_Motor_Backward(void)
{
    motor.speed1 = -App_Motor.speed;
    motor.speed2 = -App_Motor.speed;
    bsp_motor_set_speed(&motor, motor.speed1, motor.speed2);
    return &App_Motor;
}
/**
 * @brief 电机（小车）右转
 * 
 * @return AppMotorObjectTypeDef* 
 */
static AppMotorObjectTypeDef* App_Motor_Stop(void)
{
    bsp_motor_set_speed(&motor, 0, 0);
    return &App_Motor;
}
/**
 * @brief 电机（小车）左转
 * 
 * @return AppMotorObjectTypeDef* 
 */
static AppMotorObjectTypeDef* App_Motor_TurnLeft(void)
{
    motor.speed1 = -App_Motor.speed;
    motor.speed2 = App_Motor.speed;
    bsp_motor_set_speed(&motor, motor.speed1, motor.speed2);
    return &App_Motor;
}
/**
 * @brief 电机（小车）右转
 * 
 * @return AppMotorObjectTypeDef* 
 */
static AppMotorObjectTypeDef* App_Motor_TurnRight(void)
{
    motor.speed1 = App_Motor.speed;
    motor.speed2 = -App_Motor.speed;
    bsp_motor_set_speed(&motor, motor.speed1, motor.speed2);
    return &App_Motor;
}
/**
 * @brief 小车朝向
 * @note 0-360 
 *      @arg 0      -> 北
 *      @arg 90     -> 东
 *      @arg 180    -> 南
 *      @arg 270    -> 西
 * @param angle 
 * @return AppMotorObjectTypeDef* 
 */
static AppMotorObjectTypeDef* App_Motor_Orientation(uint16_t angle)
{
    uint16_t current_angle = App_Compass.Read();
    int16_t diff = (angle - current_angle);
    while (diff > 5 || diff < -5) /* 差值小于 5 度 */
    {
        uint16_t current_angle = App_Compass.Read();
        int16_t diff = (angle - current_angle);
        if (diff > 0)
        {
            if (diff > 180)
            {
                App_Motor.TurnLeft()->Low();
            }
            else
            {
                App_Motor.TurnRight()->Low();
            }
        }
        else if (diff < 0)
        {
            if (diff < 180)
            {
                App_Motor.TurnRight()->Low();
            }
            else
            {
                App_Motor.TurnLeft()->Low();
            }
        }
    }

}

/**
 * @brief 电机（小车）低速
 * 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_Motor_Speed_Low(void)
{
    App_Motor.speed = 200;
    if (motor.speed1 > 0) motor.speed1 = App_Motor.speed; else  motor.speed1 = -App_Motor.speed;
    if (motor.speed2 > 0) motor.speed2 = App_Motor.speed; else  motor.speed2 = -App_Motor.speed;
    bsp_motor_set_speed(&motor, motor.speed1, motor.speed2);
    return APP_OK;
}
/**
 * @brief 电机（小车）中速
 * 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_Motor_Speed_Middle(void)
{
    App_Motor.speed = 600;
    if (motor.speed1 > 0) motor.speed1 = App_Motor.speed; else  motor.speed1 = -App_Motor.speed;
    if (motor.speed2 > 0) motor.speed2 = App_Motor.speed; else  motor.speed2 = -App_Motor.speed;
    bsp_motor_set_speed(&motor, motor.speed1, motor.speed2);
    return APP_OK;
}
/**
 * @brief 电机（小车）高速
 * 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_Motor_Speed_High(void)
{
    App_Motor.speed = 950;
    if (motor.speed1 > 0) motor.speed1 = App_Motor.speed; else  motor.speed1 = -App_Motor.speed;
    if (motor.speed2 > 0) motor.speed2 = App_Motor.speed; else  motor.speed2 = -App_Motor.speed;
    bsp_motor_set_speed(&motor, motor.speed1, motor.speed2);
    return APP_OK;
}