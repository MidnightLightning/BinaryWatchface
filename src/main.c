#include <pebble.h>
#include "util.h"
#include "traces.h"
#include "processor.h"
#include "battery_pads.h"
#include "bluetooth.h"
#include "hours_pads.h"
#include "minutes_pads.h"

#define KEY_SHOW_DATE 0
#define KEY_SHOW_BLUETOOTH 1
#define KEY_THEME 2

Window *window;
TextLayer *processor_text;

static bool showDate = true;
static int theme = 2;

/**
 * Update the watchface display
 */
static void update_time(struct tm *tick_time) {
  if (tick_time == NULL) {
    // Get a time structure
    time_t temp = time(NULL);
    tick_time = localtime(&temp);
  }

  if (showDate) {
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
  } else {
    update_processor("");
  }

  unsigned short hour = tick_time->tm_hour % 12;
  if (hour == 0) {
    hour = 12;
  }
  update_hours(hour);
  update_minutes(tick_time->tm_min);
}

/**
 * Update the color theme
 */
static void set_theme(int theme) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Theme set to %d", theme);
  #ifdef PBL_COLOR
    switch(theme) {
      case 0: // Mono Day
        BACKGROUND_COLOR = GColorWhite;
        LED_ON_COLOR = GColorWhite;
        LED_OFF_COLOR = GColorBlack;    
        CHIP_COLOR = GColorWhite;
        CHIP_OUTLINE_COLOR= GColorBlack;
        CHIP_TEXT_COLOR = GColorBlack;
        TRACE_COLOR = GColorLightGray;
        break;
      case 1: // Mono Night
        BACKGROUND_COLOR = GColorBlack;
        LED_ON_COLOR = GColorWhite;
        LED_OFF_COLOR = GColorBlack;    
        CHIP_COLOR = GColorBlack;
        CHIP_OUTLINE_COLOR= GColorWhite;
        CHIP_TEXT_COLOR = GColorLightGray;
        TRACE_COLOR = GColorDarkGray;
        break;
      case 2: // Greenboard
        BACKGROUND_COLOR = GColorDarkGreen;
        LED_ON_COLOR = GColorPastelYellow;
        LED_OFF_COLOR = GColorArmyGreen;
        CHIP_COLOR = GColorBlack;
        CHIP_OUTLINE_COLOR= GColorWhite;
        CHIP_TEXT_COLOR = GColorLightGray;
        TRACE_COLOR = GColorMayGreen;
        break;
      case 3: // Matrix
        BACKGROUND_COLOR = GColorBlack;
        LED_ON_COLOR = GColorBrightGreen;
        LED_OFF_COLOR = GColorDarkGreen;
        CHIP_COLOR = GColorBlack;
        CHIP_OUTLINE_COLOR= GColorDarkGray;
        CHIP_TEXT_COLOR = GColorLightGray;
        TRACE_COLOR = GColorDarkGreen;
        break;
      case 4: // Ponies!!!
        BACKGROUND_COLOR = GColorVeryLightBlue;
        LED_ON_COLOR = GColorBrilliantRose;
        LED_OFF_COLOR = GColorImperialPurple;
        CHIP_COLOR = GColorChromeYellow;
        CHIP_OUTLINE_COLOR= GColorImperialPurple;
        CHIP_TEXT_COLOR = GColorImperialPurple;
        TRACE_COLOR = GColorRichBrilliantLavender;
        break;
    }
  #else
    if (theme == 0) { // Mono Day
      BACKGROUND_COLOR = GColorLightGray;
      LED_ON_COLOR = GColorWhite;
      LED_OFF_COLOR = GColorBlack;    
      CHIP_COLOR = GColorWhite;
      CHIP_OUTLINE_COLOR= GColorBlack;
      CHIP_TEXT_COLOR = GColorBlack;
      TRACE_COLOR = GColorWhite;
    } else { // Mono Night
      BACKGROUND_COLOR = GColorBlack;
      LED_ON_COLOR = GColorWhite;
      LED_OFF_COLOR = GColorBlack;    
      CHIP_COLOR = GColorBlack;
      CHIP_OUTLINE_COLOR= GColorWhite;
      CHIP_TEXT_COLOR = GColorWhite;
      TRACE_COLOR = GColorWhite;
    }
  #endif
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
 * Handle updates from the Bluetooth connection service
 */
static void bluetooth_handler(bool connected) {
  update_bluetooth(connected);
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Received AppMessage");
  Tuple *show_date_t = dict_find(iter, KEY_SHOW_DATE);
  Tuple *show_bluetooth_t = dict_find(iter, KEY_SHOW_BLUETOOTH);
  Tuple *theme_t = dict_find(iter, KEY_THEME);
  
  if (show_date_t) {
    showDate = show_date_t->value->int8;
    persist_write_bool(KEY_SHOW_DATE, showDate);
  }
  if (show_bluetooth_t) {
    SHOW_BLUETOOTH = show_bluetooth_t->value->int8;
    persist_write_bool(KEY_SHOW_BLUETOOTH, SHOW_BLUETOOTH);
  }
  if (theme_t) {
    theme = theme_t->value->int8;
    persist_write_int(KEY_THEME, theme);
    set_theme(theme);
    window_set_background_color(window, BACKGROUND_COLOR);
  }
  update_time(NULL);  
}

/**
 * Main window loading
 */
static void window_load(Window *window) {
  Layer *root_layer = window_get_root_layer(window);

  if (persist_read_bool(KEY_SHOW_DATE)) {
    showDate = persist_read_bool(KEY_SHOW_DATE);
  }
  if (persist_read_bool(KEY_SHOW_BLUETOOTH)) {
    SHOW_BLUETOOTH = persist_read_bool(KEY_SHOW_BLUETOOTH);
  } else {
    SHOW_BLUETOOTH = true;
  }
  if (persist_read_int(KEY_THEME)) {
    theme = persist_read_int(KEY_THEME);
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Window Loading...");
  set_theme(theme);
  window_set_background_color(window, BACKGROUND_COLOR);

  init_traces(root_layer);
  init_processor(root_layer);
  init_battery(root_layer);
  init_bluetooth(root_layer);
  init_hours(root_layer);
  init_minutes(root_layer);

  update_time(NULL);
  battery_handler(battery_state_service_peek());
  bluetooth_handler(connection_service_peek_pebble_app_connection());
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  battery_state_service_subscribe(battery_handler);
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_handler
  });
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(128, 64);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Window Loading done");
}

/**
 * Main window unloading
 */
static void window_unload(Window *window) {
  unload_traces();
  unload_processor();
  unload_battery();
  unload_bluetooth();
  unload_hours();
  unload_minutes();
}

/**
 * Initialize the application
 */
void init() {
  window = window_create(); // Create a new window

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
