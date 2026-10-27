#include <lvgl.h>
#include <TFT_eSPI.h>
#include "ui.h"                       // 你的仪表盘

TFT_eSPI tft = TFT_eSPI();

static const uint32_t scrW = 240;
static const uint32_t scrH = 320;
static lv_display_t * disp;

/* ① 刷屏回调:把 LVGL 的一小块像素推到屏上 */
void my_disp_flush(lv_display_t * display, const lv_area_t * area, uint8_t * px_map)
{
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)px_map, w * h, true);
    tft.endWrite();

    lv_display_flush_ready(display);
}

void setup()
{
    tft.init();
    tft.setRotation(0);               // 0~3 试试哪个方向对,1 是 240 宽 × 320 高
    tft.fillScreen(TFT_BLACK);

    // 背光(如果接在某个 GPIO 上)
    // pinMode(17, OUTPUT); digitalWrite(17, HIGH);

    lv_init();
    lv_tick_set_cb([]() { return (uint32_t)millis(); });   // ③ tick:用 millis

    disp = lv_display_create(scrW, scrH);
    lv_display_set_flush_cb(disp, my_disp_flush);

    static lv_color_t buf[scrW * 40];                       // 240×40 双缓冲够用
    lv_display_set_buffers(disp, buf, NULL, sizeof(buf),
                           LV_DISPLAY_RENDER_MODE_PARTIAL);

    ui_init();                        // ← 你的 UI,原样调用
}

void loop()
{
    lv_timer_handler();
    delay(5);
}

