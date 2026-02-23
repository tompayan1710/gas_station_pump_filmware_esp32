#pragma once
#include <LovyanGFX.hpp>
#include <lvgl.h>

extern lv_obj_t * websocket_label;
extern lv_obj_t * wifi_label;
extern lv_obj_t * db_label;
extern lv_obj_t * system_label;
extern lv_obj_t * network_message_label;

class LGFX : public lgfx::LGFX_Device {
    lgfx::Panel_GC9A01  _panel_instance;
    lgfx::Bus_SPI       _bus_instance;
public:
    LGFX(void); 
};

extern LGFX tft;    

#define LVGL_BUFFER_SIZE (240 * 10)
extern lv_color_t buf[LVGL_BUFFER_SIZE];

void my_disp_flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map);
void ui_init();