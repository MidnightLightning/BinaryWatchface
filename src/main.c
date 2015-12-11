#include <pebble.h>
#include "util.h"
#include "traces.h"
#include "processor.h"
#include "battery_pads.h"
#include "hours_pads.h"
#include "minutes_pads.h"

Window *window;
TextLayer *processor_text;

/**
 * Update the watchface display
 */
static void update_time(struct tm *tick_time) {
  if (tick_time == NULL) {
    // Get a time structure
    time_t temp = time(NULL);
    tick_time = localtime(&temp);
  }

  // Get current month abbreviation
  static char processor_text[8]; // Needs to be static, since it's used in a text_layer_set_text() call
  strftime(
    processor_text,
    sizeof(processor_text),
    "%b",
    tick_time
  );
  processor_text[1] = 0; // Null-terminate at second character to make it one character long

  char day_char[3];
  strftime(
    day_char,
    sizeof(day_char),
    "%d",
    tick_time
  );

  strcat(processor_text, day_char); // Concatenate day on end of month character
  update_processor(processor_text); // Display this time on the TextLayer

  unsigned short hour = tick_time->tm_hour % 12;
  if (hour == 0) {
    hour = 12;
  }
  update_hours(hour);
  update_minutes(tick_time->tm_min);
}

/**
 * Handle updates from the Tick Timer service
 */
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);
}

/**
 * Handle updates from the Battery Charge service
 */
static void battery_handler(BatteryChargeState state) {
  update_battery(state.charge_percent);
}

/**
 * Main window loading
 */
static void window_load(Window *window) {
  Layer *root_layer = window_get_root_layer(window);
  init_traces(root_layer);
  init_processor(root_layer);
  init_battery(root_layer);
  init_hours(root_layer);
  init_minutes(root_layer);

  update_time(NULL);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  battery_state_service_subscribe(battery_handler);
}

/**
 * Main window unloading
 */
static void window_unload(Window *window) {
  unload_traces();
  unload_processor();
  unload_battery();
  unload_hours();
  unload_minutes();
}

/**
 * Initialize the application
 */
void init() {
  window = window_create(); // Create a new window
  #ifdef PBL_COLOR
    window_set_background_color(window, GColorDarkGreen);
    LED_ON_COLOR = GColorPastelYellow;
    LED_OFF_COLOR = GColorArmyGreen;
  #else
    window_set_background_color(window, GColorBlack);
    LED_ON_COLOR = GColorWhite;
    LED_OFF_COLOR = GColorBlack;
  #endif

  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_stack_push(window, true); // Add to visible stack
}

/**
 * Unload the application
 */
void deinit() {
  window_destroy(window); // Destroy the window
}

/**
 * Main program execution
 */
int main() {
  init(); // Initialize
  app_event_loop(); // Wait for app events
  deinit(); // Exiting
  return 0; // Return without error
}
