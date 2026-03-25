#include <Arduino.h>
#include <lvgl.h>
#include "./Ui_Manager.h"
#include "../PumpState/PumpState.h"

LV_IMG_DECLARE(carte_cce);
LV_IMG_DECLARE(carte_cb);

static void payment_type_event_handler(lv_event_t * e) {
    const char * type = (const char *)lv_event_get_user_data(e);

    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        Serial.printf("Type de paiement: %s\n", type);
        currentPumpState = PUMP_SELECT_AMOUNT;
    }
}

static lv_obj_t* create_card_option(lv_obj_t * parent,
                                    const char * type_text,
                                    const char * subtitle_text,
                                    const void * img_src,
                                    const char * id)
{    
    lv_obj_t * container_payement_type = lv_obj_create(parent);
    lv_obj_remove_style_all(container_payement_type);
    lv_obj_set_size(container_payement_type, 200, 170);
    lv_obj_set_flex_flow(container_payement_type, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container_payement_type, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(container_payement_type, lv_color_hex(0xFF0000), 0);
    lv_obj_set_style_bg_opa(container_payement_type, LV_OPA_COVER, 0);

    lv_obj_add_event_cb(container_payement_type,
                   payment_type_event_handler,
                   LV_EVENT_CLICKED,
                   (void*)id);

    lv_obj_t * label = lv_label_create(container_payement_type);
    lv_label_set_text(label, type_text);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
    lv_obj_set_style_pad_bottom(label, 4, 0);

    lv_obj_t * label_description = lv_label_create(container_payement_type);
    lv_label_set_text(label_description, subtitle_text);
    lv_obj_set_style_text_font(label_description, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(label_description, lv_color_hex(0x000000), 0);
    lv_obj_set_style_pad_bottom(label_description, 4, 0);

    lv_obj_t * img = lv_img_create(container_payement_type);
    lv_img_set_src(img, img_src);
    lv_obj_set_style_pad_bottom(img, 0, 0);


    return container_payement_type;
}

void load_payment_type_screen() {

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
    lv_obj_set_style_pad_gap(main_cont, 0, 0);
    lv_obj_set_style_bg_color(main_cont, lv_color_hex(0x40FF00), 0);
    lv_obj_set_style_bg_opa(main_cont, LV_OPA_COVER, 0);

    lv_obj_t * title = lv_label_create(main_cont);
    lv_label_set_text(title, "MODE DE PAIEMENT");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x1A1A1A), 0);

    lv_obj_t * subtitle = lv_label_create(main_cont);
    lv_label_set_text(subtitle, "Choisissez votre moyen de paiement");
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(subtitle, lv_color_hex(0x6B7280), 0);

    lv_obj_t * spacer = lv_obj_create(main_cont);
    lv_obj_remove_style_all(spacer);
    lv_obj_set_size(spacer, 1, 8);

    //create_card_button(main_cont, "CARTE BANCAIRE", "Visa, Mastercard, CB", "CB");
    //create_card_button(main_cont, "CARTE CREDIT ENERGIE", "Programme fidelite", "CCE");

    lv_obj_t * row_payment_type_selection = lv_obj_create(main_cont);
    lv_obj_remove_style_all(row_payment_type_selection);
    lv_obj_set_size(row_payment_type_selection, 480, 200);
    lv_obj_set_flex_flow(row_payment_type_selection, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row_payment_type_selection, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(row_payment_type_selection, lv_color_hex(0xF3F4F6), 0);
    lv_obj_set_style_bg_opa(row_payment_type_selection, LV_OPA_COVER, 0);

    create_card_option(row_payment_type_selection, "CARTE BANCAIRE", "Visa, Mastercard, CB", &carte_cb, "CB");

    //Separation Choix
    lv_obj_t * container_ou = lv_obj_create(row_payment_type_selection);
    lv_obj_remove_style_all(container_ou);
    lv_obj_set_size(container_ou, 20, 150);
    lv_obj_set_flex_flow(container_ou, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container_ou, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * start_bar = lv_obj_create(container_ou);
    lv_obj_remove_style_all(start_bar);
    lv_obj_set_size(start_bar, 2, 60);
    lv_obj_set_style_bg_color(start_bar, lv_color_hex(0xD1D5DB), 0);
    lv_obj_set_style_bg_opa(start_bar, LV_OPA_COVER, 0);

    lv_obj_t * ou = lv_label_create(container_ou);
    lv_label_set_text(ou, "ou");
    lv_obj_set_style_pad_top(ou, 10, 0);
    lv_obj_set_style_pad_bottom(ou, 10, 0);
    lv_obj_set_style_bg_color(start_bar, lv_color_hex(0x1A75FF), 0);
    lv_obj_set_style_bg_opa(start_bar, LV_OPA_COVER, 0);

    lv_obj_t * end_bar = lv_obj_create(container_ou);
    lv_obj_remove_style_all(end_bar);
    lv_obj_set_size(end_bar, 2, 60);
    lv_obj_set_style_bg_color(end_bar, lv_color_hex(0xD1D5DB), 0);
    lv_obj_set_style_bg_opa(end_bar, LV_OPA_COVER, 0);
    //End Separation Choix

    create_card_option(row_payment_type_selection, "CARTE CREDIT ENERGIE", "Carte fidélité carburant", &carte_cce , "CB");

    lv_scr_load(screen);
}
