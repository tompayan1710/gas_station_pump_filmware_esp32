#include <lvgl.h>

#include "./Ui_Manager.h"

void load_action_prompt() {

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

    // Icone simulee (grand cercle bleu)
    lv_obj_t * icon_circle = lv_obj_create(cont);
    lv_obj_remove_style_all(icon_circle);
    lv_obj_set_size(icon_circle, 70, 70);
    lv_obj_set_style_bg_color(icon_circle, COLOR_GREY_BG, 0);
    lv_obj_set_style_bg_opa(icon_circle, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(icon_circle, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(icon_circle, 2, 0);
    lv_obj_set_style_border_color(icon_circle, COLOR_GREEN, 0);

    lv_obj_t * icon_lbl = lv_label_create(icon_circle);
    lv_label_set_text(icon_lbl, "OK");
    lv_obj_set_style_text_font(icon_lbl, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(icon_lbl, COLOR_GREEN, 0);
    lv_obj_center(icon_lbl);

    lv_obj_t * label = lv_label_create(cont);
    lv_label_set_text(label, "DECROCHEZ LE PISTOLET");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(label, COLOR_PRIMARY_TEXT, 0);

    lv_obj_t * hint = lv_label_create(cont);
    lv_label_set_text(hint, "Paiement autorise - vous pouvez commencer");
    lv_obj_set_style_text_font(hint, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(hint, COLOR_SECONDARY_TEXT, 0);

    lv_scr_load(screen);
}
