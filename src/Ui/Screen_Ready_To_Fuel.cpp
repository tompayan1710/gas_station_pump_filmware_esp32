#include <lvgl.h>

void load_action_prompt() {

    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), 0);

    lv_obj_t * label = lv_label_create(screen);
    lv_label_set_text(label, "DECROCHEZ LE PISTOLET");
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_center(label);

    lv_scr_load(screen);
}