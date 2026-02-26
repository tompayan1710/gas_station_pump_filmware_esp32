#include <Arduino.h>
#include <lvgl.h>
#include "./Ui_Manager.h"
#include "../PumpState/PumpState.h"

static void payment_type_event_handler(lv_event_t * e) {
    const char * type = (const char *)lv_event_get_user_data(e);

    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        Serial.printf("Type de paiement: %s\n", type);
        currentPumpState = PUMP_SELECT_AMOUNT;
    }
}

void load_payment_type_screen() {

    lv_obj_t * screen_type = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen_type, lv_color_hex(0x000000), 0);

    lv_obj_t * title = lv_label_create(screen_type);
    lv_label_set_text(title, "CHOISIR CARTE");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    // Carte bancaire
    lv_obj_t * btn_cb = lv_btn_create(screen_type);
    lv_obj_set_size(btn_cb, 200, 60);
    lv_obj_align(btn_cb, LV_ALIGN_CENTER, 0, -30);
    lv_obj_add_event_cb(btn_cb, payment_type_event_handler, LV_EVENT_CLICKED, (void*)"CB");

    lv_obj_t * lbl_cb = lv_label_create(btn_cb);
    lv_label_set_text(lbl_cb, "CARTE BANCAIRE");
    lv_obj_center(lbl_cb);

    // Carte Crédit Energie
    lv_obj_t * btn_cce = lv_btn_create(screen_type);
    lv_obj_set_size(btn_cce, 200, 60);
    lv_obj_align(btn_cce, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_event_cb(btn_cce, payment_type_event_handler, LV_EVENT_CLICKED, (void*)"CCE");

    lv_obj_t * lbl_cce = lv_label_create(btn_cce);
    lv_label_set_text(lbl_cce, "CARTE CREDIT ENERGIE");
    lv_obj_center(lbl_cce);

    lv_scr_load(screen_type);
}