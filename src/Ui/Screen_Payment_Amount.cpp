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

    lv_obj_t * screen_amount = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen_amount, lv_color_hex(0x000000), 0);

    lv_obj_t * title = lv_label_create(screen_amount);
    lv_label_set_text(title, "CHOISIR MONTANT");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

    const char* amounts[] = {"20 €", "50 €", "100 €"};

    for(int i=0;i<3;i++){
        lv_obj_t * btn = lv_btn_create(screen_amount);
        lv_obj_set_size(btn, 160, 45);
        lv_obj_align(btn, LV_ALIGN_CENTER, 0, -40 + (i*50));
        lv_obj_add_event_cb(btn, amount_event_handler, LV_EVENT_CLICKED, (void*)amounts[i]);

        lv_obj_t * lbl = lv_label_create(btn);
        lv_label_set_text(lbl, amounts[i]);
        lv_obj_center(lbl);
    }

    lv_scr_load(screen_amount);
}