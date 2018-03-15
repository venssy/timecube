#ifndef DEF_H
#define DEF_H

#include <pebble.h>

#define SETTINGS_KEY 1

#define SETTING_TEMPERATURE_UNIT_1 "Centigrade"
// A structure containing our settings
typedef struct ClaySettings {
  GColor BackgroundColor;
  GColor ForeFontColor;
  GColor SimpleFontColor;
  GColor LowBatteryColor;
  char* Location;
  int TemperatureUnit;
  bool NoticePerForty;
} __attribute__((__packed__)) ClaySettings;

ClaySettings settings;

// 天气数据关键字
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS  1

// 窗口布局边距
#define BOX_BOUND 38
#define BOX_PADDING 6
#define WINDOW_PADDING 9

static char *HOUR_CHARS[10] = {
  "0", "1", "2", "3", "4",
  "5", "6", "7", "8", "9"
};

// 分钟字符
static char *MINUTE_CHARS[11] = {
	"◎", "㈠", "㈡", "㈢", "㈣",
	"㈤", "㈥", "㈦", "㈧", "㈨", "㈩"
};

// 周字符
static char *WEEKDAYS[7] = {"Sun", "Mon", "Thus", "Wen", "Thur", "Fri", "Sat"};

// 窗口
//Window *s_main_window;
Layer *window_layer;
GRect window_bounds;

// 布局

// 字体
GFont s_time_font;
GFont s_time_font_16;
GFont s_time_font_20;
GFont s_time_font_38;
GFont s_time_font_10;
GFont s_simple_font;

// 颜色
GColor FORE_COLOR;
GColor BACK_COLOR;
GColor SIMPLE_COLOR;

// 位置
int position_y;

static char buff_b[10];

#endif