#include <Arduino.h>
#include <lvgl.h>
#include "./Ui_Manager.h"

void load_thank_you_screen() {

    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen);
    lv_obj_set_style_bg_color(screen, COLOR_WHITE_BG, 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    // Texte principal centré
    lv_obj_t * label = lv_label_create(screen);
    lv_label_set_text(label, "Merci pour votre visite");

    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(label, COLOR_PRIMARY_TEXT, 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);

    lv_obj_center(label);

    lv_scr_load(screen);
}