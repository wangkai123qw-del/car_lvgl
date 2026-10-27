#include "music.h"
#define PROGRESS_MAX 30000
static bool playing = false;
lv_obj_t * play_icon = NULL;
lv_obj_t * progress = NULL;
bool dragging = false;
uint32_t pos = 0;
lv_timer_t * timer = NULL;

static void progress_cb(lv_event_t * e)
{
    lv_obj_t * bar  = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_PRESSED || code == LV_EVENT_PRESSING || code == LV_EVENT_CLICKED)
    {
        lv_point_t p;
        lv_indev_get_point(lv_indev_active(), &p);

        lv_area_t a;
        lv_obj_get_coords(bar, &a);

        int32_t w = lv_area_get_width(&a);
        int32_t relative_x = p.x - a.x1;

        dragging = true;
    }
    else if (code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST){
    dragging = false;


    uint32_t dur = PROGRESS_MAX;
    if (dur == 0) return;

    uint32_t pos = (uint32_t)((uint64_t)lv_bar_get_value(bar) * dur / PROGRESS_MAX);

    }



}




static void prev_cb(lv_event_t * e)
{
    lv_obj_t * btn = lv_event_get_target(e);
    pos=0;
    lv_bar_set_value(progress, 0,LV_ANIM_OFF);


}
static void next_cb(lv_event_t * e)
{
    lv_obj_t * btn = lv_event_get_target(e);
    pos=0;
     lv_bar_set_value(progress, 0,LV_ANIM_OFF);

}
static void play_cb(lv_event_t * e)
{
    if(playing)
    {
        playing = false;
    }
    else
    {
        playing = true;
    }
    lv_label_set_text(play_icon,
                      playing ? LV_SYMBOL_PAUSE : LV_SYMBOL_PLAY);
}

static void timer_cb(lv_timer_t * timer)
{
    if(playing)
    {
        pos += 200;
        lv_bar_set_value(progress, pos,LV_ANIM_OFF);
        if(pos >= PROGRESS_MAX)
        {
            pos = 0;
        }
     }

}

static lv_obj_t * make_btn(lv_obj_t * parent, const char * symbol,
                           lv_event_cb_t cb, int32_t size)
{
    lv_obj_t * btn = lv_button_create(parent);
    lv_obj_set_size(btn, size, size);
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);

    lv_obj_t * lbl = lv_label_create(btn);
    lv_label_set_text(lbl, symbol);
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_24, 0);
    lv_obj_center(lbl);

    lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, NULL);
    return btn;
}

void music_init(lv_obj_t * parent)
{
    progress =  lv_bar_create(parent);
        /* 阻止事件向上传递给 tabview（防止拖进度条变成切页面） */
    lv_obj_clear_flag(progress, LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_clear_flag(progress, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_flag(progress, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(progress, 200, 10);
    lv_obj_align(progress, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_bar_set_range(progress, 0, PROGRESS_MAX);
    lv_obj_set_style_bg_color(progress, lv_color_hex(0xE0E0E0), LV_PART_MAIN);
    lv_obj_set_style_bg_color(progress, lv_color_hex(0x2196F3), LV_PART_INDICATOR);


    lv_obj_add_flag(progress, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_ext_click_area(progress, 15);
    lv_obj_add_event_cb(progress, progress_cb, LV_EVENT_ALL, NULL);


    lv_obj_t * row = lv_obj_create(parent);
    lv_obj_remove_style_all(row);
    lv_obj_set_size(row, 200, 300);

    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_EVENLY,
                      LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * prev_btn = make_btn(row, LV_SYMBOL_PREV, prev_cb, 25);
    lv_obj_t * play_btn = make_btn(row, LV_SYMBOL_PLAY, play_cb, 25);
    play_icon = lv_obj_get_child(play_btn, 0);
    lv_obj_t * next_btn = make_btn(row, LV_SYMBOL_NEXT, next_cb, 25);
    timer = lv_timer_create(timer_cb, 200, NULL);





}
