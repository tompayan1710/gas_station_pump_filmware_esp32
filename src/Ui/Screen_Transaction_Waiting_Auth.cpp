#include <lvgl.h>

#include "./Ui_Manager.h"

static lv_obj_t * spinner_obj = nullptr;

void load_waiting_auth_screen() {

    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen);
    lv_obj_set_style_bg_color(screen, COLOR_WHITE_BG, 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    lv_obj_t * cont = lv_obj_create(screen);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(cont, 20, 0);

    lv_obj_t * title = lv_label_create(cont);
    lv_label_set_text(title, "VALIDATION EN COURS...");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x1A1A1A), 0);

    // Spinner bleu
    spinner_obj = lv_spinner_create(cont);
    lv_obj_set_size(spinner_obj, 70, 70);
    lv_obj_set_style_arc_color(spinner_obj, lv_color_hex(0xE5E7EB), LV_PART_MAIN);
    lv_obj_set_style_arc_color(spinner_obj, lv_color_hex(0x2563EB), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(spinner_obj, 6, LV_PART_MAIN);
    lv_obj_set_style_arc_width(spinner_obj, 6, LV_PART_INDICATOR);

    lv_obj_t * subtitle = lv_label_create(cont);
    lv_label_set_text(subtitle, "Veuillez patienter");
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(subtitle, lv_color_hex(0x6B7280), 0);

    lv_scr_load(screen);
}
