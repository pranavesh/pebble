#include <pebble.h>

static Window *s_window;
static TextLayer *s_text_layer;
static TextLayer *date_layer;

static void handle_timechange(struct tm *tick_time, TimeUnits units_changed) {
  static char time_buffer[10];
  static char date_buffer[10];
  
  strftime(time_buffer, sizeof(time_buffer), "%H:%M %S", tick_time);
  text_layer_set_text(s_text_layer, time_buffer);
  strftime(date_buffer, sizeof(date_buffer), "%b %e", tick_time);
  text_layer_set_text(date_layer, date_buffer);
}

static void init(void) {
	// Create a window and get information about the window
	s_window = window_create();
  Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);
	
  // Create a text layer and set the text
	s_text_layer = text_layer_create(bounds);
	//text_layer_set_text(s_text_layer, "Hi, I'm Veeru!");
  
  // Set the font and text alignment
	text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
	text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);

	// Add the text layer to the window
	layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_text_layer));
  
  // Enable text flow and paging on the text layer, with a slight inset of 10, for round screens
  text_layer_enable_screen_text_flow_and_paging(s_text_layer, 0);
  
  //initialize and setup new layer for date
	date_layer = text_layer_create(GRect(0, 112, 144, 56));
  
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_background_color(date_layer, GColorBlack);

	text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
	text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);

	layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(date_layer));

  text_layer_enable_screen_text_flow_and_paging(date_layer, 0);
  
  //add a listener for time change
  tick_timer_service_subscribe(SECOND_UNIT, handle_timechange);
	
  // Push the window, setting the window animation to 'true'
	window_stack_push(s_window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}

static void deinit(void) {
	// Destroy the text layer
	text_layer_destroy(s_text_layer);
	text_layer_destroy(date_layer);
	
	// Destroy the window
	window_destroy(s_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
