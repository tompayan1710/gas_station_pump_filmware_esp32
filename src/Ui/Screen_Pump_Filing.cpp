#include <lvgl.h>

#include "./Ui_Manager.h"

void load_progress_screen() {

    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), 0);

    lv_obj_t * title = lv_label_create(screen);
    lv_label_set_text(title, "REMPLISSAGE EN COURS");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    progress_label = lv_label_create(screen);
    lv_label_set_text(progress_label, "0.00 L");
    lv_obj_set_style_text_color(progress_label, lv_color_hex(0x00FF00), 0);
    lv_obj_align(progress_label, LV_ALIGN_CENTER, 0, -20);

    progress_bar = lv_bar_create(screen);
    lv_obj_set_size(progress_bar, 200, 20);
    lv_obj_align(progress_bar, LV_ALIGN_CENTER, 0, 40);
    lv_bar_set_range(progress_bar, 0, 100);
    lv_bar_set_value(progress_bar, 0, LV_ANIM_OFF);

    lv_scr_load(screen);
}