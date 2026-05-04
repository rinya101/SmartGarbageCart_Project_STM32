#ifndef _APP_TRACKING_H_
#define _APP_TRACKING_H_
#include "app_ctype.h"

typedef struct AppTrackingObjectTypeDef AppTrackingObjectTypeDef;

/**
 * @brief 循迹对象指针结构体
 * 
 */
struct AppTrackingObjectTypeDef {
    AppStatusTypeDef (*Init)(void);
    AppStatusTypeDef (*Scan)(void);
};

extern AppTrackingObjectTypeDef App_Tracking;

#endif /* _APP_TRACKING_H_ */