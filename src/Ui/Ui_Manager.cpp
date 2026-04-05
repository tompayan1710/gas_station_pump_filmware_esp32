#include <LovyanGFX.hpp>
#include <lvgl.h>

#include "./Ui_Manager.h"

lv_obj_t * websocket_label = NULL;
lv_obj_t * wifi_label = NULL;
lv_obj_t * db_label = NULL;
lv_obj_t * system_label = NULL;
lv_obj_t * network_message_label = NULL;
lv_obj_t * pin_label = NULL;
lv_obj_t * progress_label = NULL;
lv_obj_t * progress_bar = NULL;

LGFX tft;
lv_color_t buf[LVGL_BUFFER_SIZE]; // Buffer agrandi pour plus de fluidité

/* --- CONFIGURATION ÉCRAN --- */
LGFX::LGFX(void) {
    {
        auto cfg = _bus_instance.config();
        cfg.spi_host = VSPI_HOST;
        cfg.pin_sclk = 18; 
        cfg.pin_mosi = 23;
        cfg.pin_miso = 19;
        cfg.pin_dc = 17;

        cfg.freq_write = 20000000;
        cfg.freq_read  = 16000000;

        _bus_instance.config(cfg);
        _panel_instance.setBus(&_bus_instance);
    }
    {
        auto cfg = _panel_instance.config();

        cfg.pin_cs  = 5;
        cfg.pin_rst = 15;

        cfg.panel_width  = 320;
        cfg.panel_height = 480;

        cfg.memory_width  = 320;
        cfg.memory_height = 480;

        cfg.offset_x = 0;
        cfg.offset_y = 0;
        
        // cfg.dlen_16bit = false;
        cfg.rgb_order = false;
        cfg.bus_shared = true;

        _panel_instance.config(cfg);
    }

    {
        auto cfg = _touch_instance.config();

        cfg.spi_host = VSPI_HOST;

        cfg.pin_sclk = 18;
        cfg.pin_mosi = 23;
        cfg.pin_miso = 19;

        cfg.pin_cs  = 16;
        cfg.pin_int = 21;

        cfg.bus_shared = true;

        _touch_instance.config(cfg);
        _panel_instance.setTouch(&_touch_instance);
    }
    setPanel(&_panel_instance);
}


/* --- INTERFACE LVGL <-> LOVYANGFX --- */
/*void my_disp_flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
    if (tft.getStartCount() == 0) tft.startWrite();

    tft.pushImage(
        area->x1,
        area->y1,
        area->x2 - area->x1 + 1,
        area->y2 - area->y1 + 1,
        (lgfx::rgb565_t*)px_map
    );

    lv_display_flush_ready(disp);
}*/

/*
void my_disp_flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    tft.startWrite();

    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushPixels((uint16_t*)px_map, w * h);

    tft.endWrite();

    lv_display_flush_ready(disp);
}*/

/*
void my_disp_flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    tft.startWrite();
    tft.pushImageDMA(
        area->x1,
        area->y1,
        w,
        h,
        (uint16_t*)px_map
    );
    tft.endWrite();

    lv_display_flush_ready(disp);
}*/

void my_disp_flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushPixels((uint16_t*)px_map, w * h);
    tft.endWrite();

    lv_display_flush_ready(disp);
}

/* --- TES FONCTIONS UI (Adaptées pour 240x240) --- */

// Note: Pour les images (img_bg, img_user, etc.), tu dois les convertir 
// avec l'outil en ligne LVGL et les ajouter à ton projet !
LV_IMG_DECLARE(img_bg);


void ui_init() {
    // Style de base sans bordure
    static lv_style_t style_base;
    lv_style_init(&style_base);
    lv_style_set_border_width(&style_base, 0);
    lv_style_set_radius(&style_base, 0);

    // Fond d'écran
    lv_obj_t * main_bg = lv_img_create(lv_scr_act());
    lv_img_set_src(main_bg, &img_bg);
    lv_obj_center(main_bg);


    wifi_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_color(wifi_label, COLOR_WHITE_BG, 0);
    lv_obj_align(wifi_label, LV_ALIGN_CENTER, 0, -20);
    lv_label_set_text(wifi_label, "WiFi: En attente...");


    websocket_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_color(websocket_label, COLOR_WHITE_BG, 0);
    lv_obj_align(websocket_label, LV_ALIGN_CENTER, 0, 00);
    lv_label_set_text(websocket_label, "WS: En attente");

    // ... tes autres labels ...
    db_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_color(db_label, COLOR_WHITE_BG, 0);
    lv_obj_align(db_label, LV_ALIGN_CENTER, 0, 20); // Placé un peu plus bas
    lv_label_set_text(db_label, "DB: En attente");

    system_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_color(system_label, COLOR_WHITE_BG, 0);
    lv_obj_align(system_label, LV_ALIGN_CENTER, 0, 40); // Placé un peu plus bas
    lv_label_set_text(system_label, "Systeme : Non fonctionnel");

    network_message_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_color(network_message_label, COLOR_WHITE_BG, 0);
    lv_obj_align(network_message_label, LV_ALIGN_CENTER, 0, 60); // Placé un peu plus bas
    lv_label_set_text(network_message_label, "aucun message");
}

void my_touch_read(lv_indev_t * indev, lv_indev_data_t * data)
{
    uint16_t x, y;

    if (tft.getTouch(&x, &y))
    {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = 480 - x;
        data->point.y = y;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}