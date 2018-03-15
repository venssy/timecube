#ifndef TIME_H
#define TIME_H

void init_time();
void register_time_server();

void draw_date_layer();
void draw_hour_layer();
void draw_minute_layers();

void time_on_destroy();

#endif