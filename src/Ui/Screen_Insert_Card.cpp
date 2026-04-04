#include <Arduino.h>
#include <lvgl.h>
#include "./Ui_Manager.h"
#include "../PumpState/PumpState.h"

static lv_obj_t * dots_label;
LV_IMG_DECLARE(insert_card);

// Animation des points
static void dots_anim_cb(lv_timer_t * timer) {
    (void)timer;

    static int state = 0;

    if(state == 0) {
        lv_label_set_text(dots_label, ".");
    } else if(state == 1) {
        lv_label_set_text(dots_label, "..");
    } else {
        lv_label_set_text(dots_label, "...");
    }

    state = (state + 1) % 3;
}

void load_insert_card_screen() {
    // ui_reset_refs();
    lv_obj_t * screen = lv_obj_create(NULL);

    //lv_obj_remove_style_all(screen);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    lv_obj_t * main_cont = lv_obj_create(screen);
    lv_obj_remove_style_all(main_cont);
    lv_obj_set_size(main_cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_flow(main_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(main_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(main_cont, 20, 0);

    // Titre
    lv_obj_t * title = lv_label_create(main_cont);
    lv_label_set_text(title, "INSERER CARTE");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x000000), 0);

    // Points animés
    dots_label = lv_label_create(main_cont);
    lv_label_set_text(dots_label, ".");
    lv_obj_set_style_text_font(dots_label, &lv_font_montserrat_20, 0);

    // Image
    lv_obj_t * img = lv_img_create(main_cont);
    lv_img_set_src(img, &insert_card);
    lv_obj_set_size(img, 117, 100);

    // Timer animation
    lv_timer_create(dots_anim_cb, 1000, NULL);

    lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
}

