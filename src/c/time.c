#include <pebble.h>
#include "./def.h"

extern Layer *window_layer;
extern GRect window_bounds;
static TextLayer *s_hour_layer[2];
static TextLayer *s_minute_layer[6];
static TextLayer *s_minute_zero_layer[6];
TextLayer *s_date_layer;

extern char *HOUR_CHARS[10];
extern char *MINITE_CHARS[11];
extern char *WEEKDAYS[7];

extern int position_y;

extern GFont s_time_font;
extern GFont s_time_font_16;
extern GFont s_simple_font;

extern GColor FORE_COLOR;
extern GColor BACK_COLOR;
extern GColor SIMPLE_COLOR;
extern ClaySettings settings;

void set_date(struct tm *time){
  static char buff[10];
  snprintf(buff, 10, "%s▓%d", WEEKDAYS[time->tm_wday], time->tm_mday);
  APP_LOG(APP_LOG_LEVEL_INFO, "set date: %s", buff);
  text_layer_set_text(s_date_layer, buff);
}

void set_hour(int hour){
  char buff[2];
  if(hour>=10){
    //snprintf(buff, sizeof(buff), "%d", hour/10);
    text_layer_set_text(s_hour_layer[0], HOUR_CHARS[hour/10]);
    //snprintf(buff, sizeof(buff), "%d", hour%10);
    text_layer_set_text(s_hour_layer[1], HOUR_CHARS[hour%10]);
  }else{
    //snprintf(buff, sizeof(buff), "%d", hour);
    text_layer_set_text(s_hour_layer[0], HOUR_CHARS[hour]);
    text_layer_set_text(s_hour_layer[1], "");
  }
  APP_LOG(APP_LOG_LEVEL_INFO, "%d:%s", hour, buff);
}

void set_minute(int index, int minute){
  APP_LOG(APP_LOG_LEVEL_INFO, "minute %d:%s", minute, MINUTE_CHARS[minute]);
  if(minute==0){
    text_layer_set_text(s_minute_layer[index], "");
  }else{
   text_layer_set_text(s_minute_layer[index], MINUTE_CHARS[minute]);
  }
}

void set_minutes(int minute){
  int i=0;
  if(minute == 0){
    for(i=0; i<6; i++){
      set_minute(i, 0);
    }
    return;
  }
  
  int tens_digit = minute / 10;
  int single_digit = minute % 10;
  
  for(i=0; i<6; i++){
    if(i<tens_digit){
      set_minute(i, 10);
    }else if(i==tens_digit){
      set_minute(i, single_digit);
    }else{
      set_minute(i, 0);
    }
  }
}

void init_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  set_hour(tick_time->tm_hour);
  set_minutes(tick_time->tm_min);
  set_date(tick_time);
}

static void update_minute(int minute){
  if(minute == 0){
    set_minutes(0);
  }else{
    int tens_digit = minute / 10;
    int single_digit = minute % 10;
    
    if(single_digit==0){
      tens_digit--;
      single_digit = 10;
    }
    
    set_minute(
      tens_digit,
      single_digit
    );
  }
}

void update_time(struct tm *tick_time) {
  set_hour(tick_time->tm_hour);
  update_minute(tick_time->tm_min);
  set_date(tick_time);
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);
  
  // Get weather update every 30 minutes
  /*if(tick_time->tm_min % 30 == 0) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
 
    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);
 
    // Send the message!
    app_message_outbox_send();
  }*/
}

void register_time_server(){
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

void draw_date_layer(){
  APP_LOG(APP_LOG_LEVEL_INFO, "drawing s_date_layer.");
    APP_LOG(APP_LOG_LEVEL_INFO, "drawing s_date_layer on [%d, %d, %d, %d].",
            WINDOW_PADDING, position_y, window_bounds.size.w, BOX_BOUND);
  position_y -= BOX_BOUND+16;
  s_date_layer = text_layer_create(
    GRect(WINDOW_PADDING, WINDOW_PADDING, window_bounds.size.w, 20));
//  APP_LOG(APP_LOG_LEVEL_INFO, "created GRect: %d, %d, %d, %d", WINDOW_PADDING, position_y);
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentLeft);
  text_layer_set_text(s_date_layer, "d");
  text_layer_set_font(s_date_layer, s_time_font_16);
  APP_LOG(APP_LOG_LEVEL_INFO, "add to window");
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  APP_LOG(APP_LOG_LEVEL_INFO, "s_hour_layer done");
}

void draw_hour_layer(){
  int i=0;
  for(; i<2; i++){
    APP_LOG(APP_LOG_LEVEL_INFO, "drawing s_hour_layer[%d].", i);
    APP_LOG(APP_LOG_LEVEL_INFO, "drawing s_hour_layer[%d] on [%d, %d, %d, %d].",
            i, WINDOW_PADDING + ( BOX_BOUND + BOX_PADDING)*i, position_y, BOX_BOUND, BOX_BOUND);
    s_hour_layer[i] = text_layer_create(
      GRect(WINDOW_PADDING + ( BOX_BOUND + BOX_PADDING)*i, position_y, BOX_BOUND, BOX_BOUND));
    text_layer_set_background_color(s_hour_layer[i], GColorRed);
    text_layer_set_text_color(s_hour_layer[i], settings.ForeFontColor);
    text_layer_set_text_alignment(s_hour_layer[i], GTextAlignmentLeft);
    text_layer_set_font(s_hour_layer[i], s_time_font);
    layer_add_child(window_layer, text_layer_get_layer(s_hour_layer[i]));
    APP_LOG(APP_LOG_LEVEL_INFO, "drawed s_hour_layer[%d].", i);
  }
  text_layer_set_text(s_hour_layer[0], "a");
}

void draw_minute_layers(){
  position_y -= WINDOW_PADDING + BOX_BOUND;
  int i, h = 2-1;
  for(; h>=0; h--){
    for(i=0; i<3; i++) {
      int index = h*3+i;
      APP_LOG(APP_LOG_LEVEL_INFO, "drawing s_minute_layer[%d] done", index);
      s_minute_zero_layer[index] = text_layer_create(
        GRect(WINDOW_PADDING+(BOX_BOUND+BOX_PADDING)*i, position_y, BOX_BOUND, BOX_BOUND));
      text_layer_set_background_color(s_minute_zero_layer[index], GColorClear);
      text_layer_set_text_color(s_minute_zero_layer[index], GColorWhite);
      text_layer_set_text_alignment(s_minute_zero_layer[index], GTextAlignmentCenter);
      text_layer_set_text(s_minute_zero_layer[index], "◎");
      text_layer_set_font(s_minute_zero_layer[index], s_time_font);
      layer_add_child(window_layer, text_layer_get_layer(s_minute_zero_layer[index]));
      
      s_minute_layer[index] = text_layer_create(
        GRect(WINDOW_PADDING+(BOX_BOUND+BOX_PADDING)*i, position_y, BOX_BOUND, BOX_BOUND));
      text_layer_set_background_color(s_minute_layer[index], GColorClear);
      text_layer_set_text_color(s_minute_layer[index], FORE_COLOR);
      text_layer_set_text_alignment(s_minute_layer[index], GTextAlignmentCenter);
      text_layer_set_text(s_minute_layer[index], "◎");
      text_layer_set_font(s_minute_layer[index], s_time_font);
      layer_add_child(window_layer, text_layer_get_layer(s_minute_layer[index]));
      APP_LOG(APP_LOG_LEVEL_INFO, "drawed s_minute_layer[%d] done", index);
    }
    position_y -= BOX_BOUND+BOX_PADDING;
  }
}

void time_on_destroy(){
	int i;
	for(i=0; i<(int)sizeof(s_hour_layer); i++){
		text_layer_destroy(s_hour_layer[i]);
	}
	for(i=0; i<(int)sizeof(s_minute_layer); i++){
		text_layer_destroy(s_minute_layer[i]);
	}
	for(i=0; i<(int)sizeof(s_minute_zero_layer); i++){
		text_layer_destroy(s_minute_zero_layer[i]);
	}
	text_layer_destroy(s_date_layer);
}