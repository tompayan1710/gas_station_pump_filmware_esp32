#include <Arduino.h>
#include <lvgl.h>
#include "./Ui_Manager.h"
#include "../PumpState/PumpState.h"

static void amount_event_handler(lv_event_t * e) {
    const char * amount = (const char *)lv_event_get_user_data(e);

    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        Serial.printf("Montant choisi: %s\n", amount);
        currentPumpState = PUMP_WAITING_PIN;
    }
}

void load_amount_selection_screen() {

    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    lv_obj_t * main_cont = lv_obj_create(screen);
    lv_obj_remove_style_all(main_cont);
    lv_obj_set_size(main_cont, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(main_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(main_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(main_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(main_cont, 14, 0);

    lv_obj_t * title = lv_label_create(main_cont);
    lv_label_set_text(title, "MONTANT A PREPAYER");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x1A1A1A), 0);

    lv_obj_t * sub = lv_label_create(main_cont);
    lv_label_set_text(sub, "Selectionnez le montant souhaite");
    lv_obj_set_style_text_font(sub, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(sub, lv_color_hex(0x6B7280), 0);

    lv_obj_t * spacer = lv_obj_create(main_cont);
    lv_obj_remove_style_all(spacer);
    lv_obj_set_size(spacer, 1, 8);

    // Sous-conteneur en ligne pour les 3 boutons
    lv_obj_t * btn_row = lv_obj_create(main_cont);
    lv_obj_remove_style_all(btn_row);
    lv_obj_set_size(btn_row, LV_PCT(90), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(btn_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btn_row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(btn_row, 14, 0);

    const char* amounts[] = {"20 EUR", "50 EUR", "100 EUR"};

    for(int i = 0; i < 3; i++) {
        lv_obj_t * btn = lv_btn_create(btn_row);
        lv_obj_remove_style_all(btn);
        lv_obj_set_size(btn, 110, 80);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0xFFFFFF), 0);
        lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, 0);
        lv_obj_set_style_radius(btn, 10, 0);
        lv_obj_set_style_border_width(btn, 1, 0);
        lv_obj_set_style_border_color(btn, lv_color_hex(0xE5E7EB), 0);
        lv_obj_set_style_shadow_width(btn, 0, 0);
        lv_obj_add_event_cb(btn, amount_event_handler, LV_EVENT_CLICKED, (void*)amounts[i]);

        lv_obj_t * lbl = lv_label_create(btn);
        lv_label_set_text(lbl, amounts[i]);
        lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, 0);
        lv_obj_set_style_text_color(lbl, lv_color_hex(0x1A1A1A), 0);
        lv_obj_center(lbl);
    }

    lv_scr_load(screen);
}
