#include <lvgl.h>

static lv_obj_t * spinner_obj = nullptr;

void load_verify_pin(){

    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    // Titre
    lv_obj_t * title = lv_label_create(screen);
    lv_label_set_text(title, "VERIFICATION DU PIN");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x1A1A1A), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);

    // Spinner bleu
    spinner_obj = lv_spinner_create(screen);
    lv_obj_set_size(spinner_obj, 80, 80);
    lv_obj_align(spinner_obj, LV_ALIGN_CENTER, 0, 10);
    lv_obj_set_style_arc_color(spinner_obj, lv_color_hex(0xE5E7EB), LV_PART_MAIN);
    lv_obj_set_style_arc_color(spinner_obj, lv_color_hex(0x2563EB), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(spinner_obj, 6, LV_PART_MAIN);
    lv_obj_set_style_arc_width(spinner_obj, 6, LV_PART_INDICATOR);

    // Sous-texte
    lv_obj_t * subtitle = lv_label_create(screen);
    lv_label_set_text(subtitle, "Verification securisee...");
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(subtitle, lv_color_hex(0x6B7280), 0);
    lv_obj_align(subtitle, LV_ALIGN_BOTTOM_MID, 0, -40);

    lv_scr_load(screen);
}
