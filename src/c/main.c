#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_day_layer;
static TextLayer *s_active_day_layer;
static TextLayer *s_battery_layer;
static TextLayer *s_date_layer;
static TextLayer *s_background_layer;
static char date_buffer[12];
static char time_buffer[8];
Window *main_window;


int dayW = 14;
int dayH = 18;
int hourY = 0;
int dayY = 85;
int dateY = 107;

int x; 
char letter[1];



static void draw_inverted_day(int x, char letter[1]){
  Layer *window_layer = window_get_root_layer(main_window);
  s_active_day_layer = text_layer_create(GRect(x, dayY, dayW, dayH));  //(x,y,w,h) (x,y,144*168)
  
  text_layer_set_text_alignment(s_active_day_layer, GTextAlignmentCenter);
  text_layer_set_text(s_active_day_layer, letter);
  text_layer_set_background_color(s_active_day_layer, GColorWhite);
  text_layer_set_text_color(s_active_day_layer, GColorBlack);
  layer_add_child(window_layer, text_layer_get_layer(s_active_day_layer));
}

static void draw_day(int x, char letter[1])
{
    Layer *window_layer = window_get_root_layer(main_window);
    s_day_layer = text_layer_create(GRect(x, dayY, dayW, dayH));  //(x,y,w,h) (x,y,144*168)
  
    text_layer_set_text_alignment(s_day_layer, GTextAlignmentCenter);
    text_layer_set_text(s_day_layer, letter);
    text_layer_set_background_color(s_day_layer, GColorBlack);
    text_layer_set_text_color(s_day_layer, GColorWhite);
  
    layer_add_child(window_layer, text_layer_get_layer(s_day_layer));
}

void day_ui(int day)
{
  int startX = 20;
 
  draw_day(20,"S");
  draw_day(35,"M");
  draw_day(50,"T");
  draw_day(65,"W");
  draw_day(80,"T");
  draw_day(95,"F");
  draw_day(110,"S");
  
  
  switch(day)
  {
    case 0: {startX = 20;  draw_inverted_day(startX,"S");} break;
    case 1: { startX = 35; draw_inverted_day(startX,"M"); } break;
    case 2: {startX = 50; draw_inverted_day(startX,"T");} break;
    case 3: {startX = 65; draw_inverted_day(startX,"W"); } break;
    case 4: {startX = 80; draw_inverted_day(startX,"T"); } break;
    case 5: {startX = 95; draw_inverted_day(startX,"F"); } break;
    case 6: {startX = 110; draw_inverted_day(startX,"S"); } break;
    
  }

  

}

static void handle_battery(BatteryChargeState charge_state) 
{
  static char battery_text[16] = "--100--";
  static char percent[10];

  if (charge_state.is_charging) {
    
  } else {
    switch ( charge_state.charge_percent ) {
      case 0 ... 10:
        strcpy(percent,"-");
        break;
      case 11 ... 20:
        strcpy(percent,"--");
        break;
      case 21 ... 30:
        strcpy(percent,"---");
        break;
      case 31 ... 40:
        strcpy(percent,"-----");
        break;
      case 41 ... 50:
        strcpy(percent,"-----");
        break;

      }
    if(charge_state.charge_percent < 100)
      {
        strcpy(battery_text,percent);
        //snprintf(battery_text, sizeof(battery_text), "%", charge_state.charge_percent);
      }
    else
      {
      strcpy(battery_text,"");
    }
  }
  text_layer_set_text(s_battery_layer, battery_text);
  text_layer_set_background_color(s_battery_layer, GColorBlack);
  text_layer_set_text_color(s_battery_layer, GColorWhite);
  
}

void battery_ui()
{
  Layer *window_layer = window_get_root_layer(main_window);
  s_battery_layer = text_layer_create(GRect(0, 150, 144, 18));  //(x,y,w,h) (x,y,144*168)  150
  
  text_layer_set_background_color(s_battery_layer, GColorBlack);
  text_layer_set_text_color(s_battery_layer, GColorWhite);
	text_layer_set_text(s_battery_layer, "----------");
	text_layer_set_text_alignment(s_battery_layer, GTextAlignmentCenter);
  
  layer_add_child(window_layer, text_layer_get_layer(s_battery_layer));
  handle_battery(battery_state_service_peek());
  
  
}

void date_ui()
{
  Layer *window_layer = window_get_root_layer(main_window);
  s_date_layer = text_layer_create(GRect(0, dateY, 144, 28));  //(x,y,w,h) (x,y,144*168)
	
	text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  text_layer_set_background_color(s_date_layer, GColorBlack);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_text(s_date_layer, date_buffer);
  
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
}

static void time_ui(){
  
  Layer *window_layer = window_get_root_layer(main_window);
  s_time_layer = text_layer_create(GRect(0, 25, 144, 168 )); 
  
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, time_buffer);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
}

static void background_ui(){
  Layer *window_layer = window_get_root_layer(main_window);
  s_background_layer = text_layer_create(GRect(0, 0, 144, 168 ));
  
  text_layer_set_background_color(s_background_layer, GColorBlack);
  text_layer_set_text_color(s_background_layer, GColorWhite);
  
  layer_add_child(window_layer, text_layer_get_layer(s_background_layer));
}


static void update_time() {
  
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  int day = tick_time->tm_wday;

  strftime(date_buffer, sizeof(date_buffer), "%e %b", tick_time);
  strftime(time_buffer, sizeof(time_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);

   //draw time
  background_ui();
  time_ui();
  date_ui();
  day_ui(day);
  battery_ui();
  
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}


static void main_window_load(Window *window) {
 
  main_window = window;
  update_time();
  //time_t temp = time(NULL); 
  //struct tm *tick_time = localtime(&temp);
  //int day = tick_time->tm_wday;
 
  
  
  //draw time
  //background_ui();
  //time_ui();
  //date_ui();
  //day_ui(day);
  //battery_ui();
  
 
}




static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_day_layer);
  text_layer_destroy(s_active_day_layer);
  text_layer_destroy(s_battery_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_background_layer);
}


static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  update_time();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
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