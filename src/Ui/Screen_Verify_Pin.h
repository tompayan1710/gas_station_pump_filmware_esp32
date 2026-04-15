#pragma once
#include <lvgl.h>

void load_verify_pin();
void update_verify_pin_text(const char* text, bool success);
void hide_verify_spinner();