#include <Arduino.h>
#include <lvgl.h>

#include "./Ui_Manager.h"

/*
LV_IMG_DECLARE(img_hs)

void load_hs_screen() {
    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen);
    lv_obj_set_style_bg_color(screen, COLOR_WHITE_BG, 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    lv_obj_t * main_cont = lv_obj_create(screen);
    lv_obj_remove_style_all(main_cont);
    lv_obj_set_size(main_cont, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(main_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(main_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(main_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(main_cont, 0, 0);
    lv_obj_set_style_bg_color(main_cont, lv_color_hex(0x40FF00), 0);
    lv_obj_set_style_bg_opa(main_cont, LV_OPA_COVER, 0);

    lv_obj_t * img = lv_img_create(main_cont);
    lv_img_set_src(img, &img_hs);
    lv_obj_set_style_pad_bottom(img, 0, 0);


    lv_obj_t * title = lv_label_create(main_cont);
    lv_label_set_text(title, "Pompe Hors Service");
    lv_obj_set_style_text_font(title, LV_FONT_MONTSERRAT_16, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x1A1A1A), 0);

    lv_obj_t * subtitle = lv_label_create(main_cont);
    lv_label_set_text(subtitle, "Veuillez contacter un employé en caisse pour obtenir de l'assistance.");
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(subtitle, lv_color_hex(0x6B7280), 0);


    lv_scr_load(screen);
}*/


void load_hs_screen() {
    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen);

    lv_obj_set_style_bg_color(screen, COLOR_RED, 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    lv_obj_t * label = lv_label_create(screen);

    lv_label_set_text(label, "POMPE HS");

    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(label, COLOR_WHITE_BG, 0);

    lv_obj_center(label);

    lv_scr_load(screen);
}