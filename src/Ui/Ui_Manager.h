#pragma once
#include <LovyanGFX.hpp>
#include <lvgl.h>

extern lv_obj_t * websocket_label;
extern lv_obj_t * wifi_label;
extern lv_obj_t * db_label;
extern lv_obj_t * system_label;
extern lv_obj_t * network_message_label;
extern lv_obj_t * pin_label;
extern lv_obj_t * progress_label;
extern lv_obj_t * progress_bar;

class LGFX : public lgfx::LGFX_Device {
    // lgfx::Panel_GC9A01  _panel_instance;
    lgfx::Panel_ILI9488 _panel_instance;
    lgfx::Bus_SPI       _bus_instance;
    lgfx::Touch_XPT2046 _touch_instance;
public:
    LGFX(void); 
};

extern LGFX tft;    

//#define LVGL_BUFFER_SIZE (240 * 10)
#define LVGL_BUFFER_SIZE (480 * 6)

extern lv_color_t buf[LVGL_BUFFER_SIZE];

void my_disp_flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map);
void ui_init();
void my_touch_read(lv_indev_t *indev, lv_indev_data_t *data);