#include <pebble.h>
#include "./def.h"
#include "./weather.h"

// A struct for our specific settings (see main.h)
extern ClaySettings settings;

// Initialize the default settings
static void prv_default_settings() {
  settings.BackgroundColor = GColorBlack;
  settings.ForeFontColor = GColorFromHEX(0x7BBFEA);
  settings.SimpleFontColor = GColorWhite;
  settings.LowBatteryColor = GColorFromHEX(0x130C0E);
  
  settings.Location = "";
  settings.TemperatureUnit = 0;
  settings.NoticePerForty = false;
}

void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

static void prv_update_display() {
  
}

void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
  // Update the display based on new settings
  prv_update_display();
}

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  if(weather_recieve_data(iter, context)==1) return;
  
  // Background Color
  Tuple *bg_color_t = dict_find(iter, MESSAGE_KEY_BackgroundColor);
  if (bg_color_t) {
    settings.BackgroundColor = GColorFromHEX(bg_color_t->value->int32);
  }

  // Fore Font Color
  Tuple *fg_color_t = dict_find(iter, MESSAGE_KEY_ForeFontColor);
  if (fg_color_t) {
    settings.ForeFontColor = GColorFromHEX(fg_color_t->value->int32);
  }

  // Low Battery Color
  Tuple *lb_color_t = dict_find(iter, MESSAGE_KEY_LowBatteryColor);
  if (fg_color_t) {
    settings.LowBatteryColor = GColorFromHEX(lb_color_t->value->int32);
  }

  // Simple Font Color
  Tuple *sf_color_t = dict_find(iter, MESSAGE_KEY_SimpleFontColor);
  if (fg_color_t) {
    settings.SimpleFontColor = GColorFromHEX(sf_color_t->value->int32);
  }

  // Location
  Tuple *location_t = dict_find(iter, MESSAGE_KEY_Location);
  if (location_t) {
    settings.Location = location_t->value->cstring;
  }

  // Temperature Unit
  Tuple *temperature_unit_t = dict_find(iter, MESSAGE_KEY_TemperatureUnit);
  if (temperature_unit_t) {
    settings.TemperatureUnit =
      strcmp(temperature_unit_t->value->cstring, SETTING_TEMPERATURE_UNIT_1)==0
      ? 0 : 1;
  }

  // NoticePerForty
  Tuple *notice_per_forty_t = dict_find(iter, MESSAGE_KEY_NoticePerForty);
  if (notice_per_forty_t) {
    settings.NoticePerForty = notice_per_forty_t->value->int32 == 1;
  }

  // Save the new settings to persistent storage
  prv_save_settings();
}

void prv_init(){
  prv_load_settings();

  // Listen for AppMessages
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);
}