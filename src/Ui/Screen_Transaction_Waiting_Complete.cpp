#include <lvgl.h>
#include <cstdio>

static lv_obj_t * spinner_obj = nullptr;

void load_transaction_waiting_complete_screen(float totalLiters, float pricePerLiter, float totalPrice) {

    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), 0);

    // Titre
    lv_obj_t * title = lv_label_create(screen);
    lv_label_set_text(title, "VALIDATION DU PAIEMENT...");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 40);

    // Récapitulatif complet
    char buffer[96];
    snprintf(buffer, sizeof(buffer),
             "Volume : %.2f L\nPrix/L : %.3f EUR\nTotal : %.2f EUR",
             totalLiters,
             pricePerLiter,
             totalPrice);

    lv_obj_t * recap = lv_label_create(screen);
    lv_label_set_text(recap, buffer);
    lv_obj_set_style_text_color(recap, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(recap, LV_ALIGN_CENTER, 0, -10);

    // Spinner
    spinner_obj = lv_spinner_create(screen);
    lv_obj_set_size(spinner_obj, 60, 60);
    lv_obj_align(spinner_obj, LV_ALIGN_CENTER, 0, 70);

    // Texte secondaire
    lv_obj_t * subtitle = lv_label_create(screen);
    lv_label_set_text(subtitle, "Veuillez patienter");
    lv_obj_set_style_text_color(subtitle, lv_color_hex(0xAAAAAA), 0);
    lv_obj_align(subtitle, LV_ALIGN_BOTTOM_MID, 0, -40);

    lv_scr_load(screen);
}