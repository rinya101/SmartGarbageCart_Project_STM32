#ifndef _APP_CTYPE_H
#define _APP_CTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#ifndef NULL
#define NULL    ((void *)0)
#endif

#define APP_UNUSED(x)           ((void)(x))
#define APP_ARRAY_SIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))
#define APP_MIN(a, b)           ((a) < (b) ? (a) : (b))
#define APP_MAX(a, b)           ((a) > (b) ? (a) : (b))

typedef enum {
    APP_OK         = 0U,
    APP_ERROR      = 1U,
    APP_BUSY       = 2U,
    APP_TIMEOUT    = 3U,
    APP_NO_INIT    = 4U,
    APP_INVALID    = 5U,
    APP_EMPTY      = 6U,
    APP_FULL       = 7U,
    APP_UNDER_VOLT = 8U,
    APP_OVER_VOLT  = 9U,
} AppStatusTypeDef;

// 修复冲突版本 ✅
typedef enum {
    APP_SWITCH_OFF = 0U,
    APP_SWITCH_ON  = 1U,
} AppSwitchTypeDef;

typedef enum {
    APP_PIN_LOW  = 0U,
    APP_PIN_HIGH = 1U,
} AppPinStateTypeDef;

#ifdef __cplusplus
}
#endif

#endif