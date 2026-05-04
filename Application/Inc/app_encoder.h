#ifndef _APP_ENCODER_H_
#define _APP_ENCODER_H_
#include "app_ctype.h"

typedef struct 
{
    AppStatusTypeDef (*Init)(void);
    int32_t (*GetVlaue)(void);
} AppEncoderObjectTypeDef;

extern AppEncoderObjectTypeDef App_Encoder;

#endif