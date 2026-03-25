#include <lvgl.h>
#include "./Ui_Manager.h"

void load_pin_screen() {

    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    // Titre
    lv_obj_t * title = lv_label_create(screen);
    lv_label_set_text(title, "ENTREZ VOTRE CODE");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x1A1A1A), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);

    // Boite PIN visuelle
    lv_obj_t * pin_box = lv_obj_create(screen);
    lv_obj_remove_style_all(pin_box);
    lv_obj_set_size(pin_box, 220, 60);
    lv_obj_align(pin_box, LV_ALIGN_CENTER, 0, -10);
    lv_obj_set_style_bg_color(pin_box, lv_color_hex(0xF3F4F6), 0);
    lv_obj_set_style_bg_opa(pin_box, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(pin_box, 10, 0);
    lv_obj_set_style_border_width(pin_box, 1, 0);
    lv_obj_set_style_border_color(pin_box, lv_color_hex(0xE5E7EB), 0);

    pin_label = lv_label_create(pin_box);
    lv_label_set_text(pin_label, "_   _   _   _");
    lv_obj_set_style_text_font(pin_label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(pin_label, lv_color_hex(0x2563EB), 0);
    lv_obj_center(pin_label);

    // Texte secondaire
    lv_obj_t * subtitle = lv_label_create(screen);
    lv_label_set_text(subtitle, "Utilisez le clavier physique");
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(subtitle, lv_color_hex(0x6B7280), 0);
    lv_obj_align(subtitle, LV_ALIGN_BOTTOM_MID, 0, -40);

    lv_scr_load(screen);
}
