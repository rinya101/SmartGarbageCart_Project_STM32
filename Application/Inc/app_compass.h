#ifndef _APP_COMPASS_H
#define _APP_COMPASS_H
#include "app_ctype.h"

typedef struct 
{
    /* 方法 */
    AppStatusTypeDef (*Init)(void);
    uint16_t (*Read)(void);
    AppStatusTypeDef (*Calibrate)(void);
    AppStatusTypeDef (*Print)(void);
} AppCompassObjectTypeDef; 

extern AppCompassObjectTypeDef App_Compass;

#endif