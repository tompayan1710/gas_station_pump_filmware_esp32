#include <lvgl.h>
#include <cstdio>

static lv_obj_t * spinner_obj = nullptr;

void load_transaction_waiting_complete_screen(float totalLiters, float pricePerLiter, float totalPrice) {

    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0xFFFFFF), 0);
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
    lv_obj_set_style_text_color(title, lv_color_hex(0x1A1A1A), 0);

    // Carte récapitulatif
    lv_obj_t * card = lv_obj_create(cont);
    lv_obj_remove_style_all(card);
    lv_obj_set_size(card, LV_PCT(75), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(card, lv_color_hex(0xF9FAFB), 0);
    lv_obj_set_style_bg_opa(card, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(card, 10, 0);
    lv_obj_set_style_border_width(card, 1, 0);
    lv_obj_set_style_border_color(card, lv_color_hex(0xE5E7EB), 0);
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
        lv_obj_set_style_text_color(row, (i == 2) ? lv_color_hex(0x1A1A1A) : lv_color_hex(0x6B7280), 0);
    }

    // Spinner bleu
    spinner_obj = lv_spinner_create(cont);
    lv_obj_set_size(spinner_obj, 60, 60);
    lv_obj_set_style_arc_color(spinner_obj, lv_color_hex(0xE5E7EB), LV_PART_MAIN);
    lv_obj_set_style_arc_color(spinner_obj, lv_color_hex(0x2563EB), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(spinner_obj, 5, LV_PART_MAIN);
    lv_obj_set_style_arc_width(spinner_obj, 5, LV_PART_INDICATOR);

    lv_obj_t * subtitle = lv_label_create(cont);
    lv_label_set_text(subtitle, "Veuillez patienter");
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(subtitle, lv_color_hex(0x6B7280), 0);

    lv_scr_load(screen);
}
