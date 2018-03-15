#include <pebble.h>
#include "./def.h"

extern Layer *window_layer;
extern GRect window_bounds;
static TextLayer *s_bluetooth_layer;

extern GFont s_simple_font;

extern GColor FORE_COLOR;
extern GColor BACK_COLOR;
extern GColor SIMPLE_COLOR;

static void handle_bluetooth(bool connected) {
  text_layer_set_text(s_bluetooth_layer, connected ? "O" : "X");
}

void register_bluetooth_server(){
  handle_bluetooth(connection_service_peek_pebble_app_connection());
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = handle_bluetooth
  });
}

void draw_bluetooth_layer(){
  APP_LOG(APP_LOG_LEVEL_INFO, "drawing s_bluetooth_layer.");
  s_bluetooth_layer = text_layer_create(
    GRect(WINDOW_PADDING, position_y, BOX_BOUND, BOX_BOUND));
  text_layer_set_background_color(s_bluetooth_layer, GColorClear);
  text_layer_set_text_color(s_bluetooth_layer, GColorWhite);
  text_layer_set_text_alignment(s_bluetooth_layer, GTextAlignmentLeft);
  text_layer_set_text(s_bluetooth_layer, "O");
  text_layer_set_font(s_bluetooth_layer, s_simple_font);
  layer_add_child(window_layer, text_layer_get_layer(s_bluetooth_layer));
  APP_LOG(APP_LOG_LEVEL_INFO, "s_bluetooth_layer done");
}

void bluetooth_on_destroy(){
  connection_service_unsubscribe();
	text_layer_destroy(s_bluetooth_layer);
}