#include <lvgl.h>
#include "./Ui_Manager.h"
#include "../PumpState/PumpState.h"

/*
void load_progress_screen() {

    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen);
    lv_obj_set_style_bg_color(screen, COLOR_WHITE_BG, 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    lv_obj_t * cont = lv_obj_create(screen);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(cont, 16, 0);

    lv_obj_t * title = lv_label_create(cont);
    lv_label_set_text(title, "REMPLISSAGE EN COURS");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, COLOR_PRIMARY_TEXT, 0);

    // Volume affiché en grand
    progress_label = lv_label_create(cont);
    lv_label_set_text(progress_label, "0.00 L");
    lv_obj_set_style_text_font(progress_label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(progress_label, COLOR_RED, 0);


        // Prix HT
    price_ht_label = lv_label_create(cont);
    lv_label_set_text(price_ht_label, "HT: 0.00 EUR");
    lv_obj_set_style_text_font(price_ht_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(price_ht_label, COLOR_PRIMARY_TEXT, 0);

    // Prix TTC
    price_ttc_label = lv_label_create(cont);
    lv_label_set_text(price_ttc_label, "TTC: 0.00 EUR");
    lv_obj_set_style_text_font(price_ttc_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(price_ttc_label, COLOR_GREEN, 0);

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Minimum: %.2f L", min_liter);

    min_label = lv_label_create(cont);
    lv_label_set_text(min_label, buffer);
    lv_obj_set_style_text_font(min_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(min_label, COLOR_SECONDARY_TEXT, 0);

    // Barre de progression bleue
    progress_bar = lv_bar_create(cont);
    lv_obj_set_size(progress_bar, 300, 16);
    lv_bar_set_range(progress_bar, 0, 100);
    lv_bar_set_value(progress_bar, 0, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(progress_bar, COLOR_GREY_BG, LV_PART_MAIN);
    lv_obj_set_style_bg_color(progress_bar, COLOR_GREEN, LV_PART_INDICATOR);
    lv_obj_set_style_radius(progress_bar, 8, LV_PART_MAIN);
    lv_obj_set_style_radius(progress_bar, 8, LV_PART_INDICATOR);

    
    lv_obj_t * hint = lv_label_create(cont);
    lv_label_set_text(hint, "Raccrochez le pistolet pour terminer");
    lv_obj_set_style_text_font(hint, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(hint, COLOR_SECONDARY_TEXT, 0);

    lv_scr_load(screen);
}*/
void load_progress_screen() {

    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen);
    lv_obj_set_style_bg_color(screen, COLOR_WHITE_BG, 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    // CONTENEUR GLOBAL
    lv_obj_t * cont = lv_obj_create(screen);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_right(cont, 10, 0);
    // ─────────────────────────────
    // BOX PRINCIPALE
    // ─────────────────────────────
    lv_obj_t * main_box = lv_obj_create(cont);
    lv_obj_set_size(main_box, LV_PCT(80), 120);
    lv_obj_clear_flag(main_box, LV_OBJ_FLAG_SCROLLABLE);

    // STYLE → BORDER UNIQUEMENT
    lv_obj_set_style_bg_opa(main_box, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(main_box, COLOR_GREY_BORDER, 0);
    lv_obj_set_style_border_width(main_box, 2, 0);
    lv_obj_set_style_radius(main_box, 12, 0);
    lv_obj_set_style_pad_all(main_box, 0, 0);

    lv_obj_set_flex_flow(main_box, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(main_box, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_margin_top(main_box, 30, 0);

    // ─ LEFT → LITRES
    lv_obj_t * left = lv_obj_create(main_box);
    lv_obj_remove_style_all(left);
    lv_obj_set_flex_grow(left, 1);
    lv_obj_set_flex_flow(left, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(left, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    progress_label = lv_label_create(left);
    lv_label_set_text(progress_label, "0.00 L");
    lv_obj_set_style_text_font(progress_label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(progress_label, COLOR_RED, 0);

    // ─ SEPARATION VERTICALE
    lv_obj_t * separator_v = lv_obj_create(main_box);
    lv_obj_remove_style_all(separator_v);
    lv_obj_set_width(separator_v, 2);
    lv_obj_set_height(separator_v, LV_PCT(80));
    lv_obj_set_style_bg_opa(separator_v, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(separator_v, COLOR_GREY_BORDER, 0);

    // ─ RIGHT → PRIX
    lv_obj_t * right = lv_obj_create(main_box);
    lv_obj_remove_style_all(right);
    lv_obj_set_flex_grow(right, 1);
    lv_obj_set_flex_flow(right, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(right, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // HT
    price_ht_label = lv_label_create(right);
    lv_label_set_text(price_ht_label, "HT: 0.00 EUR");
    lv_obj_set_style_text_font(price_ht_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(price_ht_label, COLOR_PRIMARY_TEXT, 0);
    lv_obj_set_style_margin_bottom(price_ht_label, 10, 0);

    // ─ SEPARATION HORIZONTALE
    lv_obj_t * separator_h = lv_obj_create(right);
    lv_obj_remove_style_all(separator_h);
    lv_obj_set_width(separator_h, LV_PCT(80));
    lv_obj_set_height(separator_h, 2);
    lv_obj_set_style_bg_opa(separator_h, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(separator_h, COLOR_GREY_BORDER, 0);

    // TTC
    price_ttc_label = lv_label_create(right);
    lv_label_set_text(price_ttc_label, "TTC: 0.00 EUR");
    lv_obj_set_style_text_font(price_ttc_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(price_ttc_label, COLOR_PRIMARY_TEXT, 0);
    lv_obj_set_style_margin_top(price_ttc_label, 10, 0);
    // ─────────────────────────────
    // BARRE DE PROGRESSION
    // ─────────────────────────────
    progress_bar = lv_bar_create(cont);
    lv_obj_set_size(progress_bar, LV_PCT(80), 8);
    lv_bar_set_range(progress_bar, 0, 100);
    lv_bar_set_value(progress_bar, 0, LV_ANIM_OFF);
    lv_obj_set_style_margin_top(progress_bar, 40, 0);

    lv_obj_set_style_bg_color(progress_bar, COLOR_GREY_BORDER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(progress_bar, COLOR_GREEN, LV_PART_INDICATOR);
    lv_obj_set_style_radius(progress_bar, 10, LV_PART_MAIN);
    lv_obj_set_style_radius(progress_bar, 10, LV_PART_INDICATOR);

    // ─────────────────────────────
    // MINIMUM
    // ─────────────────────────────
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Minimum: %.2f L", min_liter);

    min_label = lv_label_create(cont);
    lv_label_set_text(min_label, buffer);
    lv_obj_set_style_text_font(min_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(min_label, COLOR_SECONDARY_TEXT, 0);
    lv_obj_set_style_margin_top(min_label, 10, 0);

    // ─────────────────────────────
    // TEXTE BAS
    // ─────────────────────────────
    lv_obj_t * bottom = lv_obj_create(cont);
    lv_obj_remove_style_all(bottom);
    lv_obj_set_width(bottom, LV_PCT(100));
    lv_obj_set_flex_align(bottom, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * hint = lv_label_create(bottom);
    lv_label_set_text(hint, "Raccrochez le pistolet pour terminer");
    lv_obj_set_style_text_font(hint, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(hint, COLOR_SECONDARY_TEXT, 0);
    // lv_obj_set_style_margin_top(hint, 10, 0);


    lv_scr_load(screen);
}