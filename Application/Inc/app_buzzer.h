#ifndef _APP_BUZZER_H_
#define _APP_BUZZER_H_
#include "app_ctype.h"


typedef struct 
{
    /* 方法 */
    AppStatusTypeDef (*Init)(void);
    AppStatusTypeDef (*Beep)(uint16_t time);
    AppStatusTypeDef (*On)(void);
    AppStatusTypeDef (*Off)(void);
    AppStatusTypeDef (*Trigger)(void);
    AppStatusTypeDef (*SetPeriod)(uint16_t period);
    AppStatusTypeDef (*SetDuty)(uint16_t duty);
    AppStatusTypeDef (*PlayMusic)(void);
} AppBuzzerObjectTypeDef;

extern AppBuzzerObjectTypeDef App_Buzzer;

#endif