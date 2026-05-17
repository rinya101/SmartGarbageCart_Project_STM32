#include "app_tracking.h"
#include "bsp_tracking.h"
#include "PeripheralParamConfig.h"
#include "app_message.h"

static AppStatusTypeDef App_TrackingInit(void);
static AppStatusTypeDef App_TrackingScan(void);

/**
 * @brief 循迹实例
 * 
 */
AppTrackingObjectTypeDef App_Tracking = {
    .Init = App_TrackingInit,
    .Scan = App_TrackingScan,
};

/**
 * @brief 循迹句柄
 * 
 */
track_handle_t tracking1;
track_handle_t tracking2;
track_handle_t tracking3;

/**
 * @brief 初始化
 * 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_TrackingInit(void)
{
    App_Tracking.tracker1_sta = &(tracking1.sta);
    App_Tracking.tracker2_sta = &(tracking2.sta);
    App_Tracking.tracker3_sta = &(tracking3.sta);
    bsp_tracking_init(&tracking1, &TRACKER_A_DEFAULT_CONFIG());
    bsp_tracking_init(&tracking2, &TRACKER_B_DEFAULT_CONFIG());
    bsp_tracking_init(&tracking3, &TRACKER_C_DEFAULT_CONFIG());
    return APP_OK;
}
/**
 * @brief 扫描
 * 
 * @return AppStatusTypeDef 
 */
static AppStatusTypeDef App_TrackingScan(void)
{
    /* 左触发 */
    if (tracking1.sta) /* sta == 1 黑线触发 */
    {
        App_Message.Send("[Tracking] 左边触发");
    }
    /* 中间触发 */
    if (tracking2.sta)
    {
        App_Message.Send("[Tracking] 中间触发");
    }
    /* 右触发 */
    if (tracking3.sta)
    {
        App_Message.Send("[Tracking] 右边触发");
    }
    return APP_OK;
}