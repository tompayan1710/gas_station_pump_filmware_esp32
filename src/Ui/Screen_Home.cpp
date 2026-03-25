#include <Arduino.h>

#include "Screen_Home.h"
#include "./Ui_Manager.h"
#include "../PumpState/PumpState.h"

LV_FONT_DECLARE(font_playfair_display_1bpp);
LV_IMG_DECLARE(logo_goon_station);

// Callback bouton
static void btn_event_handler(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        Serial.println("START PUMP");
        currentPumpState = PUMP_SELECT_FUEL;
    }
}

void load_home_screen()
{
    lv_obj_t * screen_home = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen_home);
    lv_obj_set_style_bg_color(screen_home, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(screen_home, LV_OPA_COVER, 0);

    lv_obj_t * container = lv_obj_create(screen_home);
    lv_obj_remove_style_all(container);
    lv_obj_set_size(container, LV_PCT(100), LV_PCT(100));

    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Logo
    lv_obj_t * logo = lv_img_create(container);
    lv_img_set_src(logo, &logo_goon_station);
    lv_obj_set_style_pad_bottom(logo, 10, 0);

    // Texte bienvenue
    lv_obj_t * label = lv_label_create(container);
    lv_label_set_text(label, "Bienvenue");
    lv_obj_set_style_text_font(label, &font_playfair_display_1bpp, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
    lv_obj_set_style_pad_bottom(label, 80, 0);

    // Bouton démarrer
    lv_obj_t * btn = lv_btn_create(container);
    lv_obj_remove_style_all(btn);
    lv_obj_set_size(btn, 180, 55);

    lv_obj_set_style_bg_color(btn, lv_color_hex(0xFAFAFA), 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(btn, 10, 0);

    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t * btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "COMMENCER");
    lv_obj_set_style_text_color(btn_label, lv_color_hex(0x111111), 0);
    lv_obj_center(btn_label);

    // Texte encadrement en bas à droite
    /*lv_obj_t * credit = lv_label_create(screen_home);
    lv_label_set_text(credit,
    "Projet encadree par Mme. Nathalie Daumet Feneon\n"
    "Professeure Agregee Normalienne");
    lv_obj_set_style_text_color(credit, lv_color_hex(0x888888), 0);
    lv_obj_set_style_text_font(credit, &lv_font_montserrat_8, 0);
    lv_obj_align(credit, LV_ALIGN_BOTTOM_RIGHT, -10, -10);*/

    lv_scr_load(screen_home);
}