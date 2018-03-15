#include <pebble.h>
#include "./def.h"

extern Layer *window_layer;
extern GRect window_bounds;
static TextLayer *s_battery_layer;

extern GFont s_time_font_10;
extern GFont s_simple_font;

extern GColor FORE_COLOR;
extern GColor BACK_COLOR;
extern GColor SIMPLE_COLOR;

//extern char buff_b[10];

void handle_battery(BatteryChargeState charge_state) {
  APP_LOG(APP_LOG_LEVEL_INFO, "on battery change");
  if (charge_state.is_charging) {
    text_layer_set_text_color(s_battery_layer, GColorBlue);
  } else if(charge_state.charge_percent>10){
    text_layer_set_text_color(s_battery_layer, GColorWhite);
  } else {
    text_layer_set_text_color(s_battery_layer, GColorRed);
  }
  
  if(charge_state.charge_percent<20){
    text_layer_set_text(s_battery_layer, "▉");
  }else{
    int level = charge_state.charge_percent/20;
  //APP_LOG(APP_LOG_LEVEL_INFO, "battery set text..Red.");
  static char bat[] = "▉□▉□▉□▉□▉";
  //APP_LOG(APP_LOG_LEVEL_INFO, "bat[%d]:%s", sizeof("▉"), bat);
  static char buff_b[28];
  snprintf(buff_b, level*4+(level-1)*2, "%s", bat);
  //strncpy(buff_b, bat, /*charge_state.charge_percent / 20*2 -1*/4*2+2);
  APP_LOG(APP_LOG_LEVEL_INFO, "battery[%d]: [%d] %s",
          charge_state.charge_percent,
          level, buff_b);
  //snprintf(buff_b, sizeof(buff_b), "%d", charge_state.charge_percent);
  text_layer_set_text(s_battery_layer, buff_b);
  }
}

void register_battery_server(){
  battery_state_service_subscribe(handle_battery);
  handle_battery(battery_state_service_peek());
}

//▓□
void draw_battery_layer(){
  APP_LOG(APP_LOG_LEVEL_INFO, "drawing s_battery_layer.");
  s_battery_layer = text_layer_create(
    GRect(WINDOW_PADDING, WINDOW_PADDING, window_bounds.size.w - WINDOW_PADDING * 2, BOX_BOUND));
  text_layer_set_background_color(s_battery_layer, GColorClear);
  text_layer_set_text_color(s_battery_layer, GColorWhite);
  text_layer_set_text_alignment(s_battery_layer, GTextAlignmentRight);
  text_layer_set_text(s_battery_layer, "▉□▉□▉□▉□▉");
  text_layer_set_font(s_battery_layer, s_time_font_10);
  layer_add_child(window_layer, text_layer_get_layer(s_battery_layer));
  APP_LOG(APP_LOG_LEVEL_INFO, "s_battery_layer done");
}

void battery_on_destroy(){
  battery_state_service_unsubscribe();
	text_layer_destroy(s_battery_layer);
}