#include <lvgl.h>
#include <Arduino.h>

void load_thank_you_screen(float totalLiters, float totalPrice) {

    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    lv_obj_t * cont = lv_obj_create(screen);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(cont, 16, 0);

    // Badge succès (cercle vert)
    lv_obj_t * badge = lv_obj_create(cont);
    lv_obj_remove_style_all(badge);
    lv_obj_set_size(badge, 64, 64);
    lv_obj_set_style_bg_color(badge, lv_color_hex(0xF0FDF4), 0);
    lv_obj_set_style_bg_opa(badge, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(badge, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(badge, 2, 0);
    lv_obj_set_style_border_color(badge, lv_color_hex(0x16A34A), 0);

    lv_obj_t * check = lv_label_create(badge);
    lv_label_set_text(check, "OK");
    lv_obj_set_style_text_font(check, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(check, lv_color_hex(0x16A34A), 0);
    lv_obj_center(check);

    // Titre vert
    lv_obj_t * title = lv_label_create(cont);
    lv_label_set_text(title, "TRANSACTION VALIDEE");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x16A34A), 0);

    // Carte récapitulatif
    lv_obj_t * card = lv_obj_create(cont);
    lv_obj_remove_style_all(card);
    lv_obj_set_size(card, LV_PCT(70), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(card, lv_color_hex(0xF9FAFB), 0);
    lv_obj_set_style_bg_opa(card, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(card, 10, 0);
    lv_obj_set_style_border_width(card, 1, 0);
    lv_obj_set_style_border_color(card, lv_color_hex(0xE5E7EB), 0);
    lv_obj_set_style_pad_all(card, 16, 0);
    lv_obj_set_flex_flow(card, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(card, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(card, 6, 0);

    char buf_vol[32], buf_tot[32];
    snprintf(buf_vol, sizeof(buf_vol), "Volume : %.2f L", totalLiters);
    snprintf(buf_tot, sizeof(buf_tot), "Total  : %.2f EUR", totalPrice);

    lv_obj_t * r1 = lv_label_create(card);
    lv_label_set_text(r1, buf_vol);
    lv_obj_set_style_text_font(r1, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(r1, lv_color_hex(0x6B7280), 0);

    lv_obj_t * r2 = lv_label_create(card);
    lv_label_set_text(r2, buf_tot);
    lv_obj_set_style_text_font(r2, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(r2, lv_color_hex(0x1A1A1A), 0);

    // Message de fin
    lv_obj_t * thanks = lv_label_create(cont);
    lv_label_set_text(thanks, "Merci de votre visite !");
    lv_obj_set_style_text_font(thanks, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(thanks, lv_color_hex(0x6B7280), 0);

    lv_scr_load(screen);
}
