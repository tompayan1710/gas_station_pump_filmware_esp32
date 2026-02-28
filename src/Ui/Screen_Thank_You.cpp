#include <lvgl.h>
#include <Arduino.h>

void load_thank_you_screen(float totalLiters, float totalPrice) {

    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), 0);

    lv_obj_t * title = lv_label_create(screen);
    lv_label_set_text(title, "TRANSACTION VALIDEE");
    lv_obj_set_style_text_color(title, lv_color_hex(0x00FF00), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 30);

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Volume : %.2f L\nTotal : %.2f EUR", totalLiters, totalPrice);

    lv_obj_t * recap = lv_label_create(screen);
    lv_label_set_text(recap, buffer);
    lv_obj_set_style_text_color(recap, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(recap, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * thanks = lv_label_create(screen);
    lv_label_set_text(thanks, "Merci de votre visite !");
    lv_obj_set_style_text_color(thanks, lv_color_hex(0xAAAAAA), 0);
    lv_obj_align(thanks, LV_ALIGN_BOTTOM_MID, 0, -30);

    lv_scr_load(screen);
}