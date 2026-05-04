#ifndef _APP_MOTOR_H
#define _APP_MOTOR_H
#include "app_ctype.h"
/**
 * @brief 实现链式调用关键
 * 
 */
typedef struct AppMotorObjectTypeDef AppMotorObjectTypeDef;

/**
 * @brief 电机对象指针
 * 
 */
struct AppMotorObjectTypeDef {

    uint16_t speed;
    uint16_t CarAngle;    /* 车体角度 */

    AppMotorObjectTypeDef* (*Init)(void);
    AppMotorObjectTypeDef* (*Forward)(void);
    AppMotorObjectTypeDef* (*Backward)(void);
    AppMotorObjectTypeDef* (*Stop)(void);
    AppMotorObjectTypeDef* (*TurnLeft)(void);
    AppMotorObjectTypeDef* (*TurnRight)(void);
    AppMotorObjectTypeDef* (*Orientation)(uint16_t angle);
    AppStatusTypeDef (*SetSpeed)(uint16_t speed);
    AppStatusTypeDef (*Low)(void);
    AppStatusTypeDef (*Middle)(void);
    AppStatusTypeDef (*High)(void);
};
/**
 * @brief 电机单例
 * 
 */
extern AppMotorObjectTypeDef App_Motor;

#endif