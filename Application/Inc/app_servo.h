#ifndef _APP_SERVO_H
#define _APP_SERVO_H
#include "app_ctype.h"
/**
 * @brief 舵机对象
 * 
 */
typedef struct AppServoObjectTypeDef AppServoObjectTypeDef;

/**
 * @brief 舵机对象 方法
 * 
 */
struct AppServoObjectTypeDef {
    AppServoObjectTypeDef* (*Init)(void);
    AppServoObjectTypeDef* (*SetAngle)(uint8_t id, float angle);
    AppServoObjectTypeDef* (*GetAngle)(uint8_t id, float* angle);
    AppServoObjectTypeDef* (*SetUs)(uint8_t id, uint16_t us);
    AppServoObjectTypeDef* (*Print)(void);
};

/**
 * @brief 舵机对象
 * 
 */
extern AppServoObjectTypeDef App_Servo;

#endif