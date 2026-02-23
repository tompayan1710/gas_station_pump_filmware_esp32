#include <Arduino.h>

#include "Screen_Home.h"
#include "./Ui_Manager.h"

LV_FONT_DECLARE(font_playfair_display_1bpp);

// Callback pour le bouton
static void btn_event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        Serial.println("Bouton de la pompe cliqué !");
        // Ici on pourra changer l'état de la pompe plus tard
    }
}

void load_home_screen() {
    // 1. Créer un nouvel écran vide
    lv_obj_t * screen_home = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen_home, lv_color_hex(0x000000), 0); // Fond noir

    // 2. Message de Bienvenue
    lv_obj_t * welcome_label = lv_label_create(screen_home);
    lv_label_set_text(welcome_label, "BIENVENUE");
    lv_obj_set_style_text_font(welcome_label, &font_playfair_display_1bpp, 0);
    lv_obj_set_style_text_color(welcome_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(welcome_label, LV_ALIGN_CENTER, 0, -40);

    // 3. Bouton principal
    lv_obj_t * btn = lv_btn_create(screen_home);
    lv_obj_set_size(btn, 140, 50);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 30);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x00FF00), 0); // Bouton Vert

    lv_obj_t * btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "DEMARRER");
    lv_obj_center(btn_label);

    // 4. TRANSITION : Charger l'écran avec une animation de fondu
    // Le 'true' à la fin supprime l'écran précédent (le boot) pour libérer la RAM
    lv_scr_load_anim(screen_home, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, true);
}