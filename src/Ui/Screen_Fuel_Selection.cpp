#include <Arduino.h>
#include "Screen_Home.h"
#include "./Ui_Manager.h"
#include "../PumpState/PumpState.h"

static void fuel_event_handler(lv_event_t * e)
{
    const char * fuel_type = (const char *)lv_event_get_user_data(e);

    if(lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        Serial.printf("Carburant sélectionné : %s\n", fuel_type);
        currentPumpState = PUMP_SELECT_PAYMENT;
    }
}

static lv_obj_t* create_fuel_button(lv_obj_t * parent,
                                    const char * name,
                                    const char * octane,
                                    const char * description,
                                    const char * price,
                                    const char * info,
                                    lv_color_t color,
                                    const char * id)
{
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_remove_style_all(btn);
    lv_obj_set_size(btn, 140, 210);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(btn, 10, 0);
    lv_obj_set_style_border_width(btn, 1, 0);
    lv_obj_set_style_border_color(btn, lv_color_hex(0xF5F5F5), 0);
    lv_obj_set_style_shadow_width(btn, 0, 0);
    lv_obj_set_style_clip_corner(btn, true, 0);
    lv_obj_set_style_pad_all(btn, 0, 0);
    lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_COLUMN);
    lv_obj_add_event_cb(btn, fuel_event_handler, LV_EVENT_CLICKED, (void*)id);

    // ----- BANDEAU HAUT coloré -----

    static lv_style_t style_btn_red;


    lv_obj_t * header = lv_obj_create(btn);
    lv_obj_remove_style_all(header);
    lv_obj_set_size(header, LV_PCT(100), 40);
    lv_obj_set_style_bg_color(header, color, 0);
    lv_obj_set_style_bg_grad_color(header, lv_color_darken(color, 40), 0);
    lv_obj_set_style_bg_grad_dir(header, LV_GRAD_DIR_HOR, 0);
    lv_obj_set_style_bg_opa(header, LV_OPA_COVER, 0);

    lv_obj_t * name_label = lv_label_create(header);
    lv_label_set_text(name_label, name);
    lv_obj_set_style_text_font(name_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(name_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_center(name_label);

    // ----- CONTENU -----
    lv_obj_t * body = lv_obj_create(btn);
    lv_obj_remove_style_all(body);
    lv_obj_set_size(body, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(body, 12, 0);
    lv_obj_set_style_pad_gap(body, 8, 0);
    lv_obj_set_flex_flow(body, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(body, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * octane_label = lv_label_create(body);
    lv_label_set_text(octane_label, octane);
    lv_obj_set_style_text_font(octane_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(octane_label, lv_color_hex(0x6B7280), 0);

    lv_obj_t * desc_label = lv_label_create(body);
    lv_label_set_text(desc_label, description);
    lv_obj_set_style_text_font(desc_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(desc_label, lv_color_hex(0x6B7280), 0);

    lv_obj_t * price_label = lv_label_create(body);
    lv_label_set_text(price_label, price);
    lv_obj_set_style_text_font(price_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(price_label, lv_color_hex(0x1A1A1A), 0);


    // ===== TEXTE INFO (ITALIC + GRIS) =====
    lv_obj_t * info_label = lv_label_create(body);
    lv_label_set_text(info_label, info);

    lv_obj_set_style_text_font(info_label, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(info_label, lv_color_hex(0x9CA3AF), 0);
    lv_obj_set_style_text_opa(info_label, LV_OPA_70, 0);

    // ⚠️ Italique → nécessite une font italic
    // sinon ça ne marchera pas
    // ex: &lv_font_montserrat_12_italic

    return btn;
}

void load_fuel_selection_screen()
{
    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    // Container principal en COLONNE (titre + boutons), tout centré
    lv_obj_t * main_container = lv_obj_create(screen);
    lv_obj_remove_style_all(main_container);
    lv_obj_set_size(main_container, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(main_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(main_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(main_container,
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(main_container, 18, 0);

    // Titre
    lv_obj_t * title = lv_label_create(main_container);
    lv_label_set_text(title, "SELECTIONNER UN CARBURANT");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x1A1A1A), 0);

    // Sous-container en LIGNE pour les 3 cartes
    lv_obj_t * btn_container = lv_obj_create(main_container);
    lv_obj_remove_style_all(btn_container);
    lv_obj_set_size(btn_container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(btn_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btn_container,
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(btn_container, 14, 0);

    // Diesel : couleur ambrée
    create_fuel_button(btn_container,
        "DIESEL B7",
        "Octane 51",
        "Voitures diesel",
        "1.72 EUR/L",
        "Ce carburant contient jusqu'à 7% de biodiesel",
        lv_color_hex(0xD97706),
        "DIESEL");

    // SP95 : vert
    create_fuel_button(btn_container,
        "SP95-E10",
        "Octane 95",
        "Voitures essence",
        "1.89 EUR/L",
        "Ce carburant contient jusqu'à 10% de biocarburants (éthanol)",
        lv_color_hex(0x16A34A),
        "SP95");

    // SP98 : orange
    create_fuel_button(btn_container,
        "SP98",
        "Octane 98",
        "Moteurs sport",
        "1.96 EUR/L",
        "Ce carburant contient jusqu'à 5% de biocarburants (éthanol)",
        lv_color_hex(0xEA580C),
        "SP98");

    lv_scr_load(screen);
}
