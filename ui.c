#include "ui.h"
#include "music.h"

static lv_obj_t * speed_scale;
static lv_obj_t * rpm_scale;
static lv_obj_t * speed_label;
static lv_obj_t * rpm_label;
static lv_obj_t * speed_needle;
static lv_obj_t * rpm_needle;

static lv_obj_t * led_left_turn;
static lv_obj_t * led_right_turn;
static lv_obj_t * led_high_beam;
static lv_obj_t * led_handbrake;
static lv_obj_t * odo_label;
static uint32_t odo_base=12000;


static lv_obj_t * Temp_label;
static lv_obj_t * Wind_label;
static lv_obj_t * Temp_slider;
static lv_obj_t * Wind_slider;


/* ---------- 车速表指针动画回调（参数不变）---------- */
static void speed_anim_cb(void * var, int32_t value)
{
    lv_scale_set_line_needle_value(speed_scale, speed_needle,-15, value);
    lv_label_set_text_fmt(speed_label, "%d km/h", (int)value);
}

/* ---------- 转速表指针动画回调 ---------- */
static void rpm_anim_cb(void * var, int32_t value)
{
    lv_scale_set_line_needle_value(rpm_scale, rpm_needle, -15, value);
    lv_label_set_text_fmt(rpm_label, "%d x1000 rpm", (int)value);
}

//指示灯反转
static void turn_blink_timer_cb(lv_timer_t * t)
{
    lv_led_toggle(led_left_turn);
    lv_led_toggle(led_right_turn);
}
/*创建指示灯组*/
static void create_indicator_leds(lv_obj_t * parent)
{
    led_left_turn=lv_led_create(parent);
    lv_obj_set_size(led_left_turn, 20, 20);
    lv_obj_align(led_left_turn, LV_ALIGN_TOP_LEFT, 20, 0);
    lv_led_set_color(led_left_turn, lv_palette_main(LV_PALETTE_GREEN));
    led_right_turn=lv_led_create(parent);
    lv_obj_set_size(led_right_turn, 20, 20);
    lv_obj_align(led_right_turn, LV_ALIGN_TOP_RIGHT, -20, 0);
    lv_led_set_color(led_right_turn, lv_palette_main(LV_PALETTE_GREEN));

    led_high_beam=lv_led_create(parent);
    lv_obj_set_size(led_high_beam, 20, 20);
    lv_obj_align(led_high_beam, LV_ALIGN_TOP_MID, 0, 00);
    lv_led_set_color(led_high_beam, lv_palette_main(LV_PALETTE_YELLOW));
    lv_led_on(led_high_beam);

    led_handbrake=lv_led_create(parent);
    lv_obj_set_size(led_handbrake, 20, 20);
    lv_obj_align(led_handbrake, LV_ALIGN_TOP_MID, 0, 40);
    lv_led_set_color(led_handbrake, lv_palette_main(LV_PALETTE_RED));
    lv_led_on(led_handbrake);

    const char *name[]={"left","right","high beam","handbrake"};
    lv_obj_t * leds[] = {led_left_turn, led_right_turn, led_high_beam, led_handbrake};
    for(int i=0;i<4;i++)
    {
        lv_obj_t * label = lv_label_create(parent);
        lv_label_set_text(label, name[i]);
        lv_obj_align_to(label, leds[i], LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    }

    lv_timer_create(turn_blink_timer_cb, 500, NULL);
}

/* ---------- 里程表动画回调 ---------- */
static void odo_anim_cb(void * var, int32_t v)
{
    lv_label_set_text_fmt(odo_label, "%d km", (int)(v+odo_base));

}
/* ---------- 创建里程表 ---------- */
static void create_odometer(lv_obj_t * parent)
{
    odo_label = lv_label_create(parent);
    lv_obj_set_style_text_font(odo_label, &lv_font_montserrat_20,0);
    lv_obj_align(odo_label, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_label_set_text(odo_label, "ODO 012000 km");

      lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, odo_anim_cb);
    lv_anim_set_values(&a, 0, 999);
    lv_anim_set_duration(&a, 60000);          /* v9：duration，不是 time */
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(&a, lv_anim_path_linear);
    lv_anim_start(&a);
}

/*创建空调*/
static void slider_event_cb(lv_event_t * e)
{
    Temp_slider = lv_event_get_target(e);
    int temp = lv_slider_get_value(Temp_slider);
    lv_label_set_text_fmt(Temp_label, "%d", temp);

}
static void Wind_slider_event_cb(lv_event_t * e)
{
    Wind_slider = lv_event_get_target(e);
    int wind = lv_slider_get_value(Wind_slider);
    lv_label_set_text_fmt(Wind_label, "%d", wind);
}
static void switch_event_cb(lv_event_t * e)
{
    lv_obj_t * switch_btn = lv_event_get_target(e);
    bool switch_btn_on=lv_obj_has_state(switch_btn, LV_STATE_CHECKED);
    if (switch_btn_on)
    {
        lv_obj_remove_state(Temp_slider, LV_STATE_DISABLED);
        lv_obj_remove_state(Wind_slider, LV_STATE_DISABLED);
    }
    else
    {
        lv_obj_add_state(Temp_slider, LV_STATE_DISABLED);
        lv_obj_add_state(Wind_slider, LV_STATE_DISABLED);
    }


}





static void create_kt(lv_obj_t * parent)
{
    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, "Temp:");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20,0);
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -80);

    Temp_slider = lv_slider_create(parent);
    lv_obj_set_size(Temp_slider, 200, 20);
    lv_slider_set_range(Temp_slider, 16,32);
    lv_slider_set_value(Temp_slider, 24, LV_ANIM_OFF);
    lv_obj_align(Temp_slider, LV_ALIGN_BOTTOM_MID, 0, -40);

    Temp_label = lv_label_create(parent);
    lv_obj_align(Temp_label, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_style_text_font(Temp_label, &lv_font_montserrat_28,0);
    lv_label_set_text_fmt(Temp_label, "%" LV_PRId32, lv_slider_get_value(Temp_slider));


    // 2. 滑动条值改变时，更新标签
    lv_obj_add_event_cb(Temp_slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    // 1. 创建 scale 对象
    lv_obj_t * scale = lv_scale_create(parent);
    lv_obj_set_size(scale, 200, 50); // 设置合适的大小

    // 2. 设置数值范围（例如 0-100）
    lv_scale_set_range(scale, 16, 32);

    // 3. 设置刻度数量
    lv_scale_set_total_tick_count(scale, 17); // 总共11个刻度（包含0和100）
    lv_scale_set_major_tick_every(scale, 2);   // 每隔2个小刻度显示一个数字标签

    // 4. 显示数字标签
    lv_scale_set_label_show(scale, true);
    lv_obj_align(scale, LV_ALIGN_BOTTOM_MID, 0, 20);

    //开关按钮
    lv_obj_t * switch_btn = lv_switch_create(parent);
    lv_obj_set_size(switch_btn, 30, 30);
    lv_obj_align(switch_btn, LV_ALIGN_BOTTOM_LEFT, 0, -130);
    lv_obj_t * on_label=lv_label_create(parent);
    lv_label_set_text(on_label, "On/Off");
    lv_obj_set_style_text_font(on_label, &lv_font_montserrat_20,0);
    lv_obj_align(on_label, LV_ALIGN_BOTTOM_LEFT, 0, -100);
    //开关回调函数
    lv_obj_add_event_cb(switch_btn, switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    //风速
    Wind_slider = lv_slider_create(parent);
    lv_obj_set_size(Wind_slider, 100, 10);
    lv_slider_set_range(Wind_slider, 0,10);
    lv_slider_set_value(Wind_slider, 2, LV_ANIM_OFF);
    lv_obj_align(Wind_slider,  LV_ALIGN_BOTTOM_RIGHT, 0, -140);
    // 2. 滑动条值改变时，更新标签
    Wind_label = lv_label_create(parent);
    lv_obj_add_event_cb(Wind_slider, Wind_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_align(Wind_label,LV_ALIGN_TOP_MID, 0, 50);
    lv_obj_set_style_text_font(Wind_label, &lv_font_montserrat_28,0);
    lv_label_set_text_fmt(Wind_label, "%" LV_PRId32, lv_slider_get_value(Wind_slider));
    //添加刻度
    scale = lv_scale_create(parent);
    lv_obj_set_size(scale, 100, 50); // 设置合适的大小
    lv_scale_set_range(scale, 0, 10);

    lv_scale_set_total_tick_count(scale, 11); // 总共11个刻度（包含0和100）
    lv_scale_set_major_tick_every(scale, 2);   // 每隔2个小刻度显示一个数字标签
    lv_scale_set_label_show(scale, true);
    lv_obj_align(scale, LV_ALIGN_BOTTOM_RIGHT, 0, -80);


    lv_obj_add_state(Temp_slider, LV_STATE_DISABLED);
    lv_obj_add_state(Wind_slider, LV_STATE_DISABLED);


   }
/* ---------- 创建仪表盘的通用框架（LVGL 9.5 版）---------- */
static lv_obj_t * create_meter(lv_obj_t * parent, lv_coord_t x_ofs,
                               int32_t min, int32_t max,
                               int32_t angle_range, int32_t rotation,
                               uint32_t total_ticks, uint32_t major_every,
                               const char * unit_str,
                               lv_obj_t ** out_label,
                               lv_obj_t ** out_needle)
{
    /* 1. 创建 scale 对象，并设为圆形模式 */
    lv_obj_t * scale = lv_scale_create(parent);
    lv_obj_set_size(scale, 50, 50);
    lv_obj_align(scale, LV_ALIGN_CENTER,  x_ofs, 0);
    lv_scale_set_mode(scale, LV_SCALE_MODE_ROUND_OUTER);

    /* 2. 刻度总数与主刻度间隔 */
    lv_scale_set_total_tick_count(scale, total_ticks);
    lv_scale_set_major_tick_every(scale, major_every);

    /* 3. 量程、角度跨度、旋转起点 */
    lv_scale_set_range(scale, min, max);
    lv_scale_set_angle_range(scale, angle_range);
    lv_scale_set_rotation(scale, rotation);

    /* 4. 创建指针线（lv_line），并将其挂到 scale 上 */
    lv_obj_t * needle = lv_line_create(scale);
    /* 可在此处用 lv_obj_set_style_line_width / line_color 定制指针外观 */
    *out_needle = needle;
    lv_obj_set_style_line_width(needle, 2, 0);
    lv_obj_set_style_line_color(needle, lv_palette_main(LV_PALETTE_RED), 0);
    //红色指针
    lv_obj_set_style_line_color(needle, lv_color_hex(0xFF0000), LV_PART_MAIN);
    /* 5. 数字读数标签（放在仪表下方） */
    *out_label = lv_label_create(parent);
    lv_obj_set_style_text_font(*out_label, &lv_font_montserrat_16, 0);
    lv_obj_align_to(*out_label, scale, LV_ALIGN_OUT_BOTTOM_MID, -20, 40);
    lv_label_set_text(*out_label, unit_str);

    return scale;
}

/* ---------- 主初始化函数 ---------- */
void ui_init(void)
{
    lv_obj_t * scr = lv_screen_active();          // lv_scr_act() 在 9.x 中已改名为 lv_screen_active()
    lv_obj_t * tabview = lv_tabview_create(scr);
    lv_obj_set_size(tabview, 240, 320);
    lv_tabview_set_tab_bar_position(tabview, LV_DIR_TOP);
    lv_obj_t * tab_bnt=lv_tabview_get_tab_btns(tabview);
    lv_obj_set_height(tab_bnt, 30);
    lv_obj_set_style_text_font(tab_bnt, &lv_font_montserrat_16,0);
    lv_obj_t * tab_yb = lv_tabview_add_tab(tabview, "ybp");
    lv_obj_t * tab_yy = lv_tabview_add_tab(tabview, "yy");
    lv_obj_t * tab_kt    = lv_tabview_add_tab(tabview, "kt");

    /* 车速表：总刻度 41，每 8 个一条主刻度，0–240，跨度 240°，起点 210° */
    speed_scale = create_meter(tab_yb, -60,
                               0, 240,
                               240, 210,
                               41, 8,
                               "0 km/h",
                               &speed_label, &speed_needle);

    /* 转速表：总刻度 41（也可按需调整），每 5 个一条主刻度，0–8，跨度 270°，起点 135° */
    rpm_scale = create_meter(tab_yb, 50,
                             0, 8,
                             270, 135,
                             41, 5,
                             "0 x1000 rpm",
                             &rpm_label, &rpm_needle);

    /* ---------- 车速表指针动画 ---------- */
    lv_anim_t a_speed;
    lv_anim_init(&a_speed);
    lv_anim_set_exec_cb(&a_speed, speed_anim_cb);   // 回调签名 (void*, int32_t) 在 9.x 中保持不变[reference:7]
    lv_anim_set_values(&a_speed, 0, 240);
    lv_anim_set_time(&a_speed, 2500);
    lv_anim_set_playback_time(&a_speed, 1500);
    lv_anim_set_playback_delay(&a_speed, 300);
    lv_anim_set_repeat_count(&a_speed, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a_speed);

    /* ---------- 转速表指针动画 ---------- */
    lv_anim_t a_rpm;
    lv_anim_init(&a_rpm);
    lv_anim_set_exec_cb(&a_rpm, rpm_anim_cb);
    lv_anim_set_values(&a_rpm, 0, 8);
    lv_anim_set_time(&a_rpm, 1800);
    lv_anim_set_playback_time(&a_rpm, 1200);
    lv_anim_set_playback_delay(&a_rpm, 200);
    lv_anim_set_repeat_count(&a_rpm, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a_rpm);

    create_indicator_leds(tab_yb);
    create_odometer(tab_yb);
    create_kt(tab_kt);
    music_init(tab_yy);

}
