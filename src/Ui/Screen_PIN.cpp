#include <lvgl.h>
#include "./Ui_Manager.h"

void load_pin_screen() {

    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen);
    lv_obj_set_style_bg_color(screen, COLOR_WHITE_BG, 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    // Titre
    lv_obj_t * title = lv_label_create(screen);
    lv_label_set_text(title, "ENTREZ VOTRE CODE");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, COLOR_PRIMARY_TEXT, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);

    // Boite PIN visuelle
    lv_obj_t * pin_box = lv_obj_create(screen);
    lv_obj_remove_style_all(pin_box);
    lv_obj_set_size(pin_box, 220, 60);
    lv_obj_align(pin_box, LV_ALIGN_CENTER, 0, -10);
    lv_obj_set_style_bg_color(pin_box, COLOR_GREY_BG, 0);
    lv_obj_set_style_bg_opa(pin_box, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(pin_box, 10, 0);
    lv_obj_set_style_border_width(pin_box, 1, 0);
    lv_obj_set_style_border_color(pin_box, COLOR_GREY_BORDER, 0);

    pin_label = lv_label_create(pin_box);
    lv_label_set_text(pin_label, "_   _   _   _");
    lv_obj_set_style_text_font(pin_label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(pin_label, COLOR_SECONDARY_TEXT, 0);
    lv_obj_center(pin_label);

    // Texte secondaire
    lv_obj_t * subtitle = lv_label_create(screen);
    lv_label_set_text(subtitle, "Utilisez le clavier physique");
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(subtitle, COLOR_SECONDARY_TEXT, 0);
    lv_obj_align(subtitle, LV_ALIGN_BOTTOM_MID, 0, -50);

    lv_scr_load(screen);
}
