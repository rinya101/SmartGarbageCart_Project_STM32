#ifndef _APP_ESP32_MSG_H_
#define _APP_ESP32_MSG_H_
#include "app_ctype.h"
#include "Encode.h"
typedef struct AppESP32MsgObjectTypedef AppESP32MsgObjectTypedef;

/**
 * @brief ESP32消息对象指针
 * 
 */
struct AppESP32MsgObjectTypedef 
{
    AppESP32MsgObjectTypedef* (*Init)(void);
    AppESP32MsgObjectTypedef* (*Send)(uint8_t *buf);
    AppESP32MsgObjectTypedef* (*Scan)(void);
    int (*SendCmd)(Decode_Cmd_t *cmd);
};

extern AppESP32MsgObjectTypedef App_ESP32Msg;
#endif