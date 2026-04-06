/*
#include <lvgl.h>
#include <cstdio>
#include "./Ui_Manager.h"


static lv_obj_t * spinner_obj = nullptr;

void load_transaction_waiting_complete_screen(float totalLiters, float pricePerLiter, float totalPrice) {

    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen);
    lv_obj_set_style_bg_color(screen, COLOR_WHITE_BG, 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    lv_obj_t * cont = lv_obj_create(screen);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(cont, 14, 0);

    lv_obj_t * title = lv_label_create(cont);
    lv_label_set_text(title, "VALIDATION DU PAIEMENT...");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, COLOR_PRIMARY_TEXT, 0);

    // Carte récapitulatif
    lv_obj_t * card = lv_obj_create(cont);
    lv_obj_remove_style_all(card);
    lv_obj_set_size(card, LV_PCT(75), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(card, COLOR_GREY_BG, 0);
    lv_obj_set_style_bg_opa(card, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(card, 10, 0);
    lv_obj_set_style_border_width(card, 1, 0);
    lv_obj_set_style_border_color(card, COLOR_GREY_BORDER, 0);
    lv_obj_set_style_pad_all(card, 16, 0);
    lv_obj_set_flex_flow(card, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(card, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(card, 6, 0);

    char buf_vol[32], buf_ppl[32], buf_tot[32];
    snprintf(buf_vol, sizeof(buf_vol), "Volume  : %.2f L", totalLiters);
    snprintf(buf_ppl, sizeof(buf_ppl), "Prix/L  : %.3f EUR", pricePerLiter);
    snprintf(buf_tot, sizeof(buf_tot), "Total   : %.2f EUR", totalPrice);

    const char* lines[] = {buf_vol, buf_ppl, buf_tot};
    for(int i = 0; i < 3; i++) {
        lv_obj_t * row = lv_label_create(card);
        lv_label_set_text(row, lines[i]);
        lv_obj_set_style_text_font(row, &lv_font_montserrat_14, 0);
        lv_obj_set_style_text_color(row, (i == 2) ? COLOR_PRIMARY_TEXT : COLOR_SECONDARY_TEXT, 0);
    }

    // Spinner bleu
    spinner_obj = lv_spinner_create(cont);
    lv_obj_set_size(spinner_obj, 60, 60);
    lv_obj_set_style_arc_color(spinner_obj, COLOR_GREY_BG, LV_PART_MAIN);
    lv_obj_set_style_arc_color(spinner_obj, COLOR_GREEN, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(spinner_obj, 5, LV_PART_MAIN);
    lv_obj_set_style_arc_width(spinner_obj, 5, LV_PART_INDICATOR);

    lv_obj_t * subtitle = lv_label_create(cont);
    lv_label_set_text(subtitle, "Veuillez patienter");
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(subtitle, COLOR_SECONDARY_TEXT, 0);

    lv_scr_load(screen);
}
*/

#include <lvgl.h>
#include <cstdio>
#include "./Ui_Manager.h"

// Animation taille cercle
static void anim_size_cb(void * obj, int32_t v) {
    lv_obj_set_size((lv_obj_t *)obj, v, v);
}

// Animation opacité check
static void anim_opa_cb(void * obj, int32_t v) {
    lv_obj_set_style_opa((lv_obj_t *)obj, v, 0);
}

void load_transaction_waiting_complete_screen(float totalLiters, float pricePerLiter, float totalPrice) {

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

    // Titre
    lv_obj_t * title = lv_label_create(cont);
    lv_label_set_text(title, "PAIEMENT EN COURS...");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, COLOR_PRIMARY_TEXT, 0);

    // Cercle de validation
    lv_obj_t * circle = lv_obj_create(cont);
    lv_obj_set_size(circle, 20, 20);
    lv_obj_set_style_radius(circle, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(circle, COLOR_GREEN, 0);
    lv_obj_set_style_border_width(circle, 0, 0);

    // Icône check (cachée)
    lv_obj_t * check = lv_label_create(circle);
    lv_label_set_text(check, LV_SYMBOL_OK);
    lv_obj_set_style_text_font(check, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(check, lv_color_white(), 0);
    lv_obj_center(check);
    lv_obj_set_style_opa(check, LV_OPA_TRANSP, 0);

    // Texte
    lv_obj_t * subtitle = lv_label_create(cont);
    lv_label_set_text(subtitle, "Validation du paiement...");
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(subtitle, COLOR_SECONDARY_TEXT, 0);

    lv_scr_load(screen);

    // Animation cercle (grossissement)
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, circle);
    lv_anim_set_values(&a, 20, 80);
    lv_anim_set_time(&a, 400);
    lv_anim_set_exec_cb(&a, anim_size_cb);
    lv_anim_set_path_cb(&a, lv_anim_path_overshoot);
    lv_anim_start(&a);

    // Animation apparition check
    lv_anim_t a2;
    lv_anim_init(&a2);
    lv_anim_set_var(&a2, check);
    lv_anim_set_values(&a2, LV_OPA_TRANSP, LV_OPA_COVER);
    lv_anim_set_time(&a2, 200);
    lv_anim_set_delay(&a2, 300);
    lv_anim_set_exec_cb(&a2, anim_opa_cb);
    lv_anim_start(&a2);
}