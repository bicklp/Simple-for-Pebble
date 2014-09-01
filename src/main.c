#include <pebble.h>
#include <stdlib.h>
#include <stdio.h>
  

Window *window;
TextLayer *hour_layer;
TextLayer *ampm_layer;
TextLayer *date_layer;
TextLayer *day_layer;
TextLayer *battery_layer;
BitmapLayer *bluetooth_layer;
InverterLayer *i_date_layer;

static GBitmap *on_image;
static GBitmap *off_image;

static char hourText[10];
static char batteryPercent[10];

int dayW = 14;
int dayH = 18;
int hourY = 0;
int dayY = 85;
int dateY = 107;

void draw_day(int x, char letter[1])
{
  
  day_layer = text_layer_create(GRect(x, dayY, dayW, dayH));  //(x,y,w,h) (x,y,144*168)
  text_layer_set_text_alignment(day_layer, GTextAlignmentCenter);
  text_layer_set_text(day_layer, letter);
  text_layer_set_background_color(day_layer, GColorBlack);
  text_layer_set_text_color(day_layer, GColorWhite);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(day_layer));
  
}

void set_day_layer(int day)
{
  int startX = 20;
 
  switch(day)
    {
    case 1: startX = 20; break;
    case 2: startX = 35; break;
    case 3: startX = 50; break;
    case 4: startX = 65; break;
    case 5: startX = 80; break;
    case 6: startX = 95; break;
    case 0: startX = 110; break;
  }
  // Create Layer
  
   //Monday
  
  draw_day(20,"M");
  draw_day(35,"T");
  draw_day(50,"W");
  draw_day(65,"T");
  draw_day(80,"F");
  draw_day(95,"S");
  draw_day(110,"S");

  
  //Add Inverted Layer
  i_date_layer = inverter_layer_create(GRect (startX, dayY, dayW, dayH)); //(x,y,w,h) (x,y,144*168)
	layer_add_child(window_get_root_layer(window), inverter_layer_get_layer(i_date_layer));
  
  
}


/*
  Handle battery events
*/
static void handle_battery(BatteryChargeState charge_state) {
  static char battery_text[16] = "--100--";
  static char percent[10];

  if (charge_state.is_charging) {
    snprintf(battery_text, sizeof(battery_text), "--charging--");
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
      case 51 ... 60:
        strcpy(percent,"------");
        break;
      case 61 ... 70:
        strcpy(percent,"-------");
        break;
      case 71 ... 80:
        strcpy(percent,"--------");
        break;
      case 81 ... 90:
        strcpy(percent,"---------");
        break;
      case 91 ... 100:
        strcpy(percent,"----------");
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
  text_layer_set_text(battery_layer, battery_text);
  text_layer_set_background_color(battery_layer, GColorWhite);
  text_layer_set_text_color(battery_layer, GColorBlack);
}


void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  //Allocate long-lived storage (required by TextLayer)
  static char clock_buffer[6];
  static char ampm_text[4];
  static char date_buffer[12];
  
  //Write the time to the buffer in a safe manner
  strftime(date_buffer, sizeof(date_buffer), "%e %b", tick_time);
  
  int hour = tick_time->tm_hour;
  int minute = tick_time->tm_min;
  
  if ( !clock_is_24h_style() )
    {
    strftime(clock_buffer, sizeof(clock_buffer), "%I:%M", tick_time);
  }
  else
    {
     strftime(clock_buffer, sizeof(clock_buffer), "%H:%M", tick_time);
  }
  
  
  // Remove leading zero (only in 12h-mode)
  if ( !clock_is_24h_style() && (clock_buffer[0] == '0') ) {
    
    memmove( clock_buffer, &clock_buffer[1], sizeof( clock_buffer ) - 1 );
    
  }
  
  text_layer_set_text(hour_layer, clock_buffer);
  
  // Update AM/PM indicator (i.e. AM or PM or nothing when using 24-hour style)
  strftime( ampm_text, sizeof( ampm_text ), clock_is_24h_style() ? "" : "%p", tick_time );
  text_layer_set_text( ampm_layer, ampm_text );
  
  text_layer_set_text(date_layer, date_buffer);
  
  int day = tick_time->tm_wday;
  set_day_layer(day);
  
  handle_battery(battery_state_service_peek());
  
}

void set_hour_layer()
{
  // Create Layer
  hour_layer = text_layer_create(GRect(0, 0, 144, 80 ));  //(x,y,w,h) (x,y,144*168)
  
  // Set the text, font, and text alignment
	text_layer_set_text(hour_layer, "");
	text_layer_set_font(hour_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TREK_72)));
	text_layer_set_text_alignment(hour_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(hour_layer));
  
  ampm_layer = text_layer_create(GRect(124, 59, 20, 14 ));  //(x,y,w,h) (x,y,144*168)
  // Set the text, font, and text alignment
	text_layer_set_text(ampm_layer, "");
	text_layer_set_text_alignment(ampm_layer, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(ampm_layer));
  
  
}



void set_date_layer()
{
  // Create Layer
  date_layer = text_layer_create(GRect(0, dateY, 144, 28));  //(x,y,w,h) (x,y,144*168)
  
  // Set the text, font, and text alignment
	text_layer_set_text(date_layer, "");
	text_layer_set_font(date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TREK_24)));
	text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
}

void set_battery_layer()
{
  // Create Layer
  battery_layer = text_layer_create(GRect(0, 150, 144, 18));  //(x,y,w,h) (x,y,144*168)  150
  text_layer_set_background_color(battery_layer, GColorWhite);
  text_layer_set_text_color(battery_layer, GColorBlack);
  // Set the text, font, and text alignment
	text_layer_set_text(battery_layer, "----------");
	text_layer_set_text_alignment(battery_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(battery_layer));
}


void set_bluetooth_layer()
{
  bluetooth_layer = bitmap_layer_create(GRect (62, 5, 20, 10)); //(x,y,w,h) (x,y,144*168)
  off_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTH_OFF);
  bitmap_layer_set_bitmap(bluetooth_layer, on_image);
  //layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bluetooth_layer));

}


void set_inverted()
{
  i_date_layer = inverter_layer_create(GRect (0, 0, 144, 168)); //(x,y,w,h) (x,y,144*168)
	layer_add_child(window_get_root_layer(window), inverter_layer_get_layer(i_date_layer));
}


void handle_deinit(void) {
	// Destroy the text layer
  text_layer_destroy(hour_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(day_layer);
  bitmap_layer_destroy(bluetooth_layer);
  text_layer_destroy(battery_layer);
  
  inverter_layer_destroy(i_date_layer);
	
	// Destroy the window
	window_destroy(window);
  bluetooth_connection_service_unsubscribe();
  battery_state_service_unsubscribe();
}


void bluetooth_connection_callback(bool connected) {
        APP_LOG(APP_LOG_LEVEL_INFO, "bluetooth connected=%d", (int) connected);
        if(!connected)
        {
         // Vibe pattern: ON for 200ms, OFF for 100ms, ON for 400ms:
          static const uint32_t const segments[] = { 200, 100, 200, 100, 200, 1000, 200,100,200,100,200 };
          VibePattern pat = {
            .durations = segments,
            .num_segments = ARRAY_LENGTH(segments),
          };
          vibes_enqueue_custom_pattern(pat);  
          bitmap_layer_set_bitmap(bluetooth_layer, off_image);
          layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bluetooth_layer));
          
        }
        else{ 
          // Vibe pattern: ON for 200ms, OFF for 100ms, ON for 400ms:
          static const uint32_t const segments[] = { 100};
          VibePattern pat = {
            .durations = segments,
            .num_segments = ARRAY_LENGTH(segments),
          };
          vibes_enqueue_custom_pattern(pat);  
          layer_remove_from_parent(bitmap_layer_get_layer(bluetooth_layer));
        }
  
    }





void peekBluetooth()
  {
  
   if (bluetooth_connection_service_peek()) {
       layer_remove_from_parent(bitmap_layer_get_layer(bluetooth_layer));
    } else {
      bitmap_layer_set_bitmap(bluetooth_layer, off_image);
      layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bluetooth_layer));
      
    }
}

void handle_init(void) {
	// Create a window and text layer
	window = window_create();
  
  // Create and Fill Layers
	set_hour_layer();
  set_date_layer(); 
  set_bluetooth_layer();
  set_battery_layer();
  set_inverted();
  peekBluetooth();
 
  
  time_t now = time(NULL);
  tick_handler(localtime(&now),SECOND_UNIT);
  
  
  // Check for Bluetooth and Battery events 
  bluetooth_connection_service_subscribe(bluetooth_connection_callback);
  battery_state_service_subscribe(&handle_battery);
  
  
  // Run Clock
  tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler)tick_handler);
  
  
  // Setup battery layer
  
	// Push the window
	window_stack_push(window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}



int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}

