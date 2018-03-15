#include <pebble.h>
#include "./def.h"

extern Layer *window_layer;
extern GRect window_bounds;
static TextLayer *s_weather_layer;

extern int position_y;

extern GFont s_time_font;
extern GFont s_time_font_16;
extern GFont s_simple_font;

extern GColor FORE_COLOR;
extern GColor BACK_COLOR;
extern GColor SIMPLE_COLOR;

int weather_recieve_data(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char temperature_buffer[8];
  static char conditions_buffer[32];
  static char weather_layer_buffer[32];
  
  // Read tuples for data
  Tuple *temperature_t = dict_find(iterator, MESSAGE_KEY_Temperature);
  Tuple *conditions_t = dict_find(iterator, MESSAGE_KEY_Conditions);

  // If all data is available, use it
  if(temperature_t && conditions_t) {
    snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)temperature_t->value->int32);
    snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", conditions_t->value->cstring);

    // Assemble full string and display
    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s %s", temperature_buffer, conditions_buffer);
    text_layer_set_text(s_weather_layer, temperature_buffer);
    return 1;
  }else{
    return 0;
  }
}
 
static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}
 
static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}
 
static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void register_weather_server(){
  // Register callbacks
  //app_message_register_inbox_received(weather_recieve_data);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  const int inbox_size = 128;
  const int outbox_size = 128;
  app_message_open(inbox_size, outbox_size);
}

void draw_weather_layer(){
	// Create temperature Layer
  s_weather_layer = text_layer_create(
    GRect(window_bounds.size.w/3*2, WINDOW_PADDING+15, window_bounds.size.w/3*1-WINDOW_PADDING, 16));
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorWhite);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentRight);
  text_layer_set_text(s_weather_layer, "22");
  
  // Create second custom font, apply it and add to Window
  text_layer_set_font(s_weather_layer, s_time_font_16);
  layer_add_child(window_layer, text_layer_get_layer(s_weather_layer));
}

void weather_on_destroy(){
	text_layer_destroy(s_weather_layer);
}