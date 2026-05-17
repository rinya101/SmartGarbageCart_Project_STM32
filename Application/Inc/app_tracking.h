#ifndef _APP_TRACKING_H_
#define _APP_TRACKING_H_
#include "app_ctype.h"

typedef struct AppTrackingObjectTypeDef AppTrackingObjectTypeDef;

/**
 * @brief 循迹对象指针结构体
 * 
 */
struct AppTrackingObjectTypeDef {
    /* 三路循迹状态 1：黑线 0：白色背景 */
    uint8_t *tracker1_sta; 
    uint8_t *tracker2_sta;
    uint8_t *tracker3_sta;

    AppStatusTypeDef (*Init)(void);
    AppStatusTypeDef (*Scan)(void);
};

extern AppTrackingObjectTypeDef App_Tracking;

#endif /* _APP_TRACKING_H_ */