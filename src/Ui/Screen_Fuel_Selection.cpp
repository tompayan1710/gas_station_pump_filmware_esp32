#include <Arduino.h>
#include "Screen_Home.h"
#include "./Ui_Manager.h"
#include "../PumpState/PumpState.h"


static void fuel_event_handler(lv_event_t * e)
{
    const char * fuel = (const char *)lv_event_get_user_data(e);

    if(lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        Serial.printf("Carburant sélectionné : %s\n", fuel);

        fuelType = fuel;

        if(strcmp(fuel, "SP95") == 0){
            price_per_liter = price_SP95;
        }
        else if(strcmp(fuel, "DIESEL") == 0){
            price_per_liter = price_DIESEL;
        }

        Serial.printf("Prix sélectionné : %.2f\n", price_per_liter);

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
    lv_obj_set_size(btn, 180, 210);
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
    lv_obj_set_style_text_color(name_label, COLOR_WHITE_TEXT, 0);
    lv_obj_center(name_label);

    // ----- CONTENU -----
    lv_obj_t * body = lv_obj_create(btn);
    lv_obj_remove_style_all(body);
    lv_obj_set_size(body, LV_PCT(100), 170);
    lv_obj_set_style_pad_all(body, 12, 0);
    lv_obj_set_style_pad_gap(body, 0, 0);
    lv_obj_set_flex_flow(body, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(body, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER);
    // lv_obj_set_flex_align(body, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * top = lv_obj_create(body);
    lv_obj_remove_style_all(top);
    lv_obj_set_size(top, LV_PCT(100), 100); // limite haute
    lv_obj_clear_flag(top, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(top, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(top,
        LV_FLEX_ALIGN_START,   // align horizontal
        LV_FLEX_ALIGN_START,    // align vertical
        LV_FLEX_ALIGN_CENTER    // align cross axis
    );
    lv_obj_set_style_pad_gap(top, 4, 0);

    lv_obj_t * octane_label = lv_label_create(top);
    lv_label_set_text(octane_label, octane);
    lv_obj_set_style_text_font(octane_label, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(octane_label, COLOR_SECONDARY_TEXT, 0);
    lv_obj_set_style_pad_top(octane_label, 0, 0);
    lv_obj_set_style_text_align(octane_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(octane_label, LV_PCT(100));

    lv_obj_t * desc_label = lv_label_create(top);
    lv_label_set_text(desc_label, description);
    lv_obj_set_style_text_font(desc_label, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(desc_label, COLOR_SECONDARY_TEXT, 0);
    lv_obj_set_style_text_align(desc_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(desc_label, LV_PCT(100));
    lv_obj_set_style_pad_bottom(desc_label, 10, 0);

    lv_obj_t * price_label = lv_label_create(top);
    lv_label_set_text(price_label, price);
    lv_obj_set_style_text_font(price_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(price_label, COLOR_PRIMARY_TEXT, 0);
    lv_obj_set_style_text_align(price_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_width(price_label, LV_PCT(100));
    
    // ===== TEXTE INFO (ITALIC + GRIS) =====
    lv_obj_t * info_label = lv_label_create(body);
    lv_label_set_text(info_label, info);

    lv_obj_set_style_text_font(info_label, &lv_font_montserrat_10, 0);
    lv_obj_set_style_text_color(info_label, COLOR_SECONDARY_TEXT, 0);
    lv_obj_set_style_text_opa(info_label, LV_OPA_70, 0);

    lv_obj_set_width(info_label, LV_PCT(100));
    lv_label_set_long_mode(info_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(info_label, LV_TEXT_ALIGN_CENTER, 0);

    lv_obj_set_style_pad_bottom(info_label, 10, 0);




    lv_obj_add_flag(header, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(body, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(top, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(name_label, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(octane_label, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(desc_label, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(price_label, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_flag(info_label, LV_OBJ_FLAG_EVENT_BUBBLE);

    return btn;
}

void load_fuel_selection_screen()
{
    lv_obj_t * screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(screen);
    lv_obj_set_style_bg_color(screen, COLOR_BG, 0);
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
    lv_obj_set_style_text_color(title, COLOR_PRIMARY_TEXT, 0);

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

    // ===== FORMAT PRIX =====
    static char priceDieselStr[20];
    static char priceSP95Str[20];

    snprintf(priceDieselStr, sizeof(priceDieselStr), "%.2f EUR/L", price_DIESEL);
    snprintf(priceSP95Str, sizeof(priceSP95Str), "%.2f EUR/L", price_SP95);

    // Diesel
    create_fuel_button(btn_container,
        "DIESEL B7",
        "Octane 51",
        "Voitures diesel",
        priceDieselStr,
        "Ce carburant contient jusqu'a 7% de biodiesel",
        COLOR_DIESEL,
        "DIESEL");

    // SP95
    create_fuel_button(btn_container,
        "SP95-E10",
        "Octane 95",
        "Voitures essence",
        priceSP95Str,
        "Ce carburant contient jusqu'a 10% de biocarburants (ethanol)",
        COLOR_SP95,
        "SP95");

     lv_scr_load(screen);
}
