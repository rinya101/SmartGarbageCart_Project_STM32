#include "app_oled.h"
#include "PeripheralParamConfig.h"
#include "bsp_oled.h"
#include "welcome.h"


static AppStatusTypeDef App_OledInit(void);
static AppStatusTypeDef App_OledRefresh(void);
static AppStatusTypeDef App_OledClear(void);
static AppStatusTypeDef App_OledDrawPoint(uint8_t x, uint8_t y, uint8_t color);
static AppStatusTypeDef App_OledDrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
static AppStatusTypeDef App_OledDrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
static AppStatusTypeDef App_OledDrawFillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
static AppStatusTypeDef App_OledWelcome(void);

AppOledObjectTypedef App_Oled = {
    .Init = App_OledInit,
    .Refresh = App_OledRefresh,
    .Clear = App_OledClear,
    .DrawPoint = App_OledDrawPoint,
    .DrawLine = App_OledDrawLine,
    .DrawRect = App_OledDrawRect,
    .DrawFillRect = App_OledDrawFillRect,
    .Welcome = App_OledWelcome,
};

oled_handle_t oled = {0};


static AppStatusTypeDef App_OledInit(void)
{
    bsp_oled_init(&oled, &OLED_DEFAULT_CONFIG());
    return APP_OK;
}

static AppStatusTypeDef App_OledRefresh(void)
{
    bsp_oled_refresh(&oled);
    return APP_OK;
}

static AppStatusTypeDef App_OledClear(void)
{
    for (uint32_t i = 0; i < oled.width * oled.height; i++)
    {
        oled.screen_buf[i] = 0x00;
    }
    return APP_OK;
}

/**
 * @brief OLED 画点
 *
 * @param x      水平向右
 *     @arg 0 - 127
 * @param y      竖直向下
 *     @arg 0 - 63
 * @param color
 *     @arg 0 空白
 *     @arg 1 填充
 * @return AppStatusTypeDef
 */
static AppStatusTypeDef App_OledDrawPoint(uint8_t x, uint8_t y, uint8_t color)
{

    bsp_oled_draw_point(&oled, x, y, color);
    return APP_OK;
}

static AppStatusTypeDef App_OledDrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
    int16_t dx = x2 - x1;
    int16_t dy = y2 - y1;
    int16_t stepX = dx >= 0 ? 1 : -1;
    int16_t stepY = dy >= 0 ? 1 : -1;

    dx = dx > 0 ? dx : -dx;
    dy = dy > 0 ? dy : -dy;

    if (dx > dy)
    {
        int16_t p = 2 * dy - dx;
        while (1)
        {
            App_OledDrawPoint(x1, y1, color);
            if (x1 == x2) break;
            x1 += stepX;
            if (p < 0)
                p += 2 * dy;
            else
            {
                y1 += stepY;
                p += 2 * dy - 2 * dx;
            }
        }
    }
    else
    {
        int16_t p = 2 * dx - dy;
        while (1)
        {
            App_OledDrawPoint(x1, y1, color);
            if (y1 == y2) break;
            y1 += stepY;
            if (p < 0)
                p += 2 * dx;
            else
            {
                x1 += stepX;
                p += 2 * dx - 2 * dy;
            }
        }
    }
    return APP_OK;
}

static AppStatusTypeDef App_OledDrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color)
{
    App_OledDrawLine(x,      y,      x + w - 1, y,         color);
    App_OledDrawLine(x,      y + h - 1, x + w - 1, y + h - 1, color);
    App_OledDrawLine(x,      y,      x,      y + h - 1, color);
    App_OledDrawLine(x + w - 1, y,      x + w - 1, y + h - 1, color);
    return APP_OK;
}

static AppStatusTypeDef App_OledDrawFillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color)
{
    uint16_t i, j;
    for (j = 0; j < h; j++)
    {
        for (i = 0; i < w; i++)
        {
            App_OledDrawPoint(x + i, y + j, color);
        }
    }
    return APP_OK;
}

static AppStatusTypeDef App_OledWelcome(void)
{
    const uint8_t *buf = (const uint8_t *)welcome;
    for (uint16_t i = 0; i < 1024; i++)
    {
        oled.screen_buf[i] = buf[i];
    }

    bsp_oled_refresh(&oled);
    return APP_OK;
}