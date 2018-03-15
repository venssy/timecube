#include <pebble.h>
#include "./def.h"
#include "./time.h"
#include "src/c/battery.h"
#include "src/c/bluetooth.h"
#include "./weather.h"
#include "./settings.h"
 
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static Window *s_main_window;
extern GRect window_bounds;
extern Layer *window_layer;

extern int position_y;

extern GFont s_time_font;
extern GFont s_time_font_16;
extern GFont s_time_font_20;
extern GFont s_time_font_38;
extern GFont s_time_font_10;
//extern GFont s_pebble_font;
extern GFont s_simple_font;

extern GColor FORE_COLOR;
extern GColor BACK_COLOR;
extern GColor SIMPLE_COLOR;

static void main_window_load(Window *window) {
  // 初始化颜色
  FORE_COLOR = GColorBlue;
  BACK_COLOR = GColorBlack;
  SIMPLE_COLOR = GColorWhite;
  
  // 初始化字体
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PEBBLE_38));
  s_time_font_16 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PEBBLE_16));
  s_time_font_20 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PEBBLE_20));
  s_time_font_38 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PEBBLE_38));
  s_time_font_10 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PEBBLE_10));
  //s_simple_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_48));
  s_simple_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_10));
  
  prv_init();
  
  // 初始化窗口相关数据
  window_layer = window_get_root_layer(window);
  window_bounds = layer_get_bounds(window_layer);
  position_y = window_bounds.size.h;
  
  APP_LOG(APP_LOG_LEVEL_INFO, "window width:%d, height:%d", window_bounds.size.w, window_bounds.size.h);
  APP_LOG(APP_LOG_LEVEL_INFO, "BOX_BOUND: %d", BOX_BOUND);
  // 设置背景
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  // Create BitmapLayer to display the GBitmap
  s_background_layer = bitmap_layer_create(window_bounds);
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  //layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  
  // 绘制时间布局
  draw_minute_layers();
  draw_hour_layer();
  //draw_minute_layers();
  draw_date_layer();
  
  // 初始化时间
  init_time();
  
  // 绘制其他布局
  draw_battery_layer();
  //draw_bluetooth_layer();
  draw_weather_layer();
  
  register_time_server();
  register_battery_server();
  
  APP_LOG(APP_LOG_LEVEL_INFO, "finish draw window and layer. ");
}
 
static void main_window_unload(Window *window) {
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_simple_font);
  
  gbitmap_destroy(s_background_bitmap);
  
  bitmap_layer_destroy(s_background_layer);
  //text_layer_destroy(s_time_layer);
  //text_layer_destroy(s_weather_layer);
}
  
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);
}
 
static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}
 
int main(void) {
  init();
  app_event_loop();
  deinit();
}