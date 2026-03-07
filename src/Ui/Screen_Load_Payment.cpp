#include <lvgl.h>

static lv_obj_t * pin_label = nullptr;

LV_FONT_DECLARE(font_playfair_display_1bpp);


void load_pin_screen() {

    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), 0);

    // Titre
    lv_obj_t * title = lv_label_create(screen);
    lv_label_set_text(title, "ENTREZ VOTRE CODE");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);

    // 4 tirets (code PIN visuel)
    pin_label = lv_label_create(screen);
    lv_label_set_text(pin_label, "_   _   _   _");
    lv_obj_set_style_text_color(pin_label, lv_color_hex(0x00FF00), 0);
    lv_obj_set_style_text_font(pin_label, &font_playfair_display_1bpp, 0);
    lv_obj_align(pin_label, LV_ALIGN_CENTER, 0, 0);

    // Texte secondaire
    lv_obj_t * subtitle = lv_label_create(screen);
    lv_label_set_text(subtitle, "Utilisez le clavier");
    lv_obj_set_style_text_color(subtitle, lv_color_hex(0xAAAAAA), 0);
    lv_obj_align(subtitle, LV_ALIGN_BOTTOM_MID, 0, -40);

    lv_scr_load(screen);
}
