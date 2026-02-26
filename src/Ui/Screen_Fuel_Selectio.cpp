#include <Arduino.h>
#include "Screen_Home.h"
#include "./Ui_Manager.h"
#include "../PumpState/PumpState.h" // Pour changer l'état au clic

LV_FONT_DECLARE(font_playfair_display_1bpp);

// Callback unique pour identifier quel carburant est choisi
static void fuel_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    const char * fuel_type = (const char *)lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED) {
        Serial.printf("Carburant sélectionné : %s\n", fuel_type);
        
        // On passe à l'étape suivante de la machine à état
        currentPumpState = PUMP_SELECT_PAYMENT; 
    }
}

void load_fuel_selection_screen() {
    // 1. Créer l'écran
    lv_obj_t * screen_fuel = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen_fuel, lv_color_hex(0x000000), 0);

    // 2. Titre
    lv_obj_t * title_label = lv_label_create(screen_fuel);
    lv_label_set_text(title_label, "CHOISIR CARBURANT");
    lv_obj_set_style_text_color(title_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);

    // --- BOUTON DIESEL ---
    lv_obj_t * btn_diesel = lv_btn_create(screen_fuel);
    lv_obj_set_size(btn_diesel, 180, 50);
    lv_obj_align(btn_diesel, LV_ALIGN_CENTER, 0, -30);
    lv_obj_set_style_bg_color(btn_diesel, lv_color_hex(0xFFA500), 0); // Orange/Jaune pour le Diesel
    // On passe "DIESEL" comme donnée utilisateur pour le callback
    lv_obj_add_event_cb(btn_diesel, fuel_event_handler, LV_EVENT_CLICKED, (void*)"DIESEL");

    lv_obj_t * lbl_diesel = lv_label_create(btn_diesel);
    lv_label_set_text(lbl_diesel, "DIESEL (B7)");
    lv_obj_center(lbl_diesel);

    // --- BOUTON SANS PLOMB 95 ---
    lv_obj_t * btn_sp95 = lv_btn_create(screen_fuel);
    lv_obj_set_size(btn_sp95, 180, 50);
    lv_obj_align(btn_sp95, LV_ALIGN_CENTER, 0, 40);
    lv_obj_set_style_bg_color(btn_sp95, lv_color_hex(0x008000), 0); // Vert pour le Sans Plomb
    // On passe "SP95" comme donnée utilisateur
    lv_obj_add_event_cb(btn_sp95, fuel_event_handler, LV_EVENT_CLICKED, (void*)"SP95");

    lv_obj_t * lbl_sp95 = lv_label_create(btn_sp95);
    lv_label_set_text(lbl_sp95, "SANS PLOMB 95 (E10)");
    lv_obj_center(lbl_sp95);

    // 4. Animation de chargement
    lv_scr_load(screen_fuel);
}