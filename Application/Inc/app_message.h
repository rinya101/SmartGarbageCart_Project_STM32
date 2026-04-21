/**
 * @brief 应用层消息接口（单例面向对象）
 * @note 全局唯一对象，无需创建
 */
#ifndef _APP_MESSAGE_H_
#define _APP_MESSAGE_H_

#include "app_ctype.h"

/**
 * @brief 消息对象结构体
 */
typedef struct
{
    /* 方法 */
    AppStatusTypeDef (*Init)(void);
    AppStatusTypeDef (*Send)(uint8_t *data, uint16_t len);
    AppStatusTypeDef (*Process)(void);
} AppMessageObjectTypeDef;

/**
 * @brief 全局单例对象
 */
extern AppMessageObjectTypeDef App_Message;

#endif