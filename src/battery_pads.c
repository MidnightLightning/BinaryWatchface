#include <pebble.h>
#include "battery_pads.h"
#include "util.h"

/**
 * Handle the drawing and updating of the graphics for the LED pads for battery level
 */

static int pad_size = 2;
static int pad_gap = 1;
static Layer *graphics_layer;
typedef struct {
  int battery;
} BatteryContext;

/**
 * Get the coordinates of a specific LED
 *
 * LED ID numbers count left-to-right.
 */
static GPoint get_center_of_led(int id_number, GPoint center) {
  int offset_start = center.y + (pad_size+pad_gap)*4 + pad_size/2;
  return GPoint(center.x, offset_start - id_number*(pad_size+pad_gap) - pad_size/2);
}

/**
 * Get the color of a given LED, given the current hour
 */
static GColor get_color_of_led(int id_number) {
  BatteryContext *ctx = (BatteryContext*)layer_get_data(graphics_layer);

  int num_leds = (int)ctx->battery/10;
  if (num_leds >= id_number+1) {
    return LED_ON_COLOR;
  } else {
    return LED_OFF_COLOR;
  }

}

/**
 * Update the graphics layer
 */
static void draw(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GPoint offset = point_add(grect_center_point(&bounds), GPoint(-56, 18));

  graphics_full_rect(ctx, GRect(offset.x-7, offset.y-(pad_size+pad_gap)*4-pad_gap/2-3, 15, (pad_size+pad_gap)*9+pad_gap+2), GColorBlack, GColorWhite);

  // Draw the LED pads
  for(int i = 0; i < 9; i++) {
    GPoint led_center = get_center_of_led(i, offset);
    graphics_context_set_fill_color(ctx, get_color_of_led(i));
    graphics_fill_rect(ctx, GRect(led_center.x-5, led_center.y-pad_size/2, 11, pad_size), 0, GCornerNone);
  }
}

/**
 * Draw the initial graphics for the LED pads for hours display
 */
void init_battery(Layer *parent_layer) {
  GRect bounds = layer_get_bounds(parent_layer);

  graphics_layer = layer_create_with_data(bounds, sizeof(BatteryContext));
  BatteryContext *ctx = (BatteryContext*)layer_get_data(graphics_layer);
  ctx->battery = 0;

  layer_set_update_proc(graphics_layer, draw);
  layer_add_child(
    parent_layer,
    graphics_layer
  );
}

/**
 * Set the battery display
 */
void update_battery(int battery) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Battery level set to %d", battery);
  BatteryContext *ctx = (BatteryContext*)layer_get_data(graphics_layer);
  ctx->battery = battery;
  layer_mark_dirty(graphics_layer);
}

/**
 * Unload the graphics from the display
 */
void unload_battery() {
  layer_destroy(graphics_layer);
}
