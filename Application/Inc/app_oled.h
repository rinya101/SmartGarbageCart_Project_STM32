#ifndef _APP_OLED_H
#define _APP_OLED_H
#include "app_ctype.h"
#include "bsp_oled.h"



typedef struct
{
    /* 方法 */
    AppStatusTypeDef (*Init)(void);
    AppStatusTypeDef (*Refresh)(void);
    AppStatusTypeDef (*Clear)(void);
    AppStatusTypeDef (*DrawPoint)(uint8_t x, uint8_t y, uint8_t color);
    AppStatusTypeDef (*DrawLine)(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
    AppStatusTypeDef (*DrawRect)(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
    AppStatusTypeDef (*DrawFillRect)(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
    AppStatusTypeDef (*Welcome)(void);

} AppOledObjectTypedef;

extern AppOledObjectTypedef App_Oled;
#endif