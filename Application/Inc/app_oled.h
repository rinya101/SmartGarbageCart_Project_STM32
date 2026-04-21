#ifndef _APP_OLED_H
#define _APP_OLED_H
#include "app_ctype.h"
typedef struct 
{
    /* 属性 */

    /* 方法 */
    AppStatusTypeDef (*Init)(void);
    AppStatusTypeDef (*Clear)(void);

} AppOledObjectTypedef;

extern AppOledObjectTypedef App_Oled;

#endif // !_APP_OLED_H
