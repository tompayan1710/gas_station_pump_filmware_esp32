#include <lvgl.h>

#include "./Ui_Manager.h"

//static lv_obj_t * spinner_obj = nullptr;

/*
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
    lv_label_set_text(title, "AUTORISATION BANCAIRE...");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, COLOR_PRIMARY_TEXT, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);

    // Spinner bleu
    spinner_obj = lv_spinner_create(cont);
    lv_obj_set_size(spinner_obj, 70, 70);
    lv_obj_set_style_arc_color(spinner_obj, COLOR_GREY_BG, LV_PART_MAIN);
    lv_obj_set_style_arc_color(spinner_obj, COLOR_GREEN, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(spinner_obj, 6, LV_PART_MAIN);
    lv_obj_set_style_arc_width(spinner_obj, 6, LV_PART_INDICATOR);

    lv_obj_t * subtitle = lv_label_create(cont);
    lv_label_set_text(subtitle, "Veuillez patienter");
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(subtitle, COLOR_PRIMARY_TEXT, 0);
    

    // Sous-texte
    lv_obj_t * subtitle = lv_label_create(cont);
    lv_label_set_text(subtitle, "Veuillez patienter");
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(subtitle, COLOR_SECONDARY_TEXT, 0);
    lv_obj_align(subtitle, LV_ALIGN_BOTTOM_MID, 0, -30);


    lv_scr_load(screen);
}*/

#include <lvgl.h>
#include "./Ui_Manager.h"

static lv_obj_t * spinner_obj = nullptr;

void load_waiting_auth_screen(){

    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen);
    lv_obj_set_style_bg_color(screen, COLOR_WHITE_BG, 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    // ─────────────────────────────
    // TITRE (IDENTIQUE VERIFY PIN)
    // ─────────────────────────────
    lv_obj_t * title = lv_label_create(screen);
    lv_label_set_text(title, "AUTORISATION BANCAIRE");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, COLOR_PRIMARY_TEXT, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);

    // ─────────────────────────────
    // SPINNER (MÊME POSITION)
    // ─────────────────────────────
    spinner_obj = lv_spinner_create(screen);
    lv_obj_set_size(spinner_obj, 80, 80);
    lv_obj_align(spinner_obj, LV_ALIGN_CENTER, 0, 10);

    lv_obj_set_style_arc_color(spinner_obj, COLOR_GREY_BG, LV_PART_MAIN);
    lv_obj_set_style_arc_color(spinner_obj, COLOR_GREEN, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(spinner_obj, 6, LV_PART_MAIN);
    lv_obj_set_style_arc_width(spinner_obj, 6, LV_PART_INDICATOR);

    // ─────────────────────────────
    // SOUS TEXTE (MÊME POSITION)
    // ─────────────────────────────
    lv_obj_t * subtitle = lv_label_create(screen);
    lv_label_set_text(subtitle, "Validation en cours...");
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(subtitle, COLOR_SECONDARY_TEXT, 0);
    lv_obj_align(subtitle, LV_ALIGN_BOTTOM_MID, 0, -30);

    lv_scr_load(screen);
}