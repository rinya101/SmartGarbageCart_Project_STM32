#ifndef _APP_ULTRASONIC_H
#define _APP_ULTRASONIC_H
#include "app_ctype.h"
/**
 * @brief 超声波单例程
 * 
 */
typedef struct 
{
    AppStatusTypeDef (*Init)(void);
    uint16_t (*GetDistance)(void);
    AppStatusTypeDef (*Trigger)(void);
    AppStatusTypeDef (*Scan)(void);
    AppStatusTypeDef (*Print)(void);
} AppUltObjectTypeDef;

extern AppUltObjectTypeDef App_Ult;

#endif