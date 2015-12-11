#include <pebble.h>
#include "hours_pads.h"
#include "util.h"

/**
 * Handle the drawing and updating of the graphics for the LED pads for hours display
 */

static int pad_size = 24;
static int pad_gap = 8;
static Layer *graphics_layer;
typedef struct {
  unsigned short hours;
} HoursContext;

/**
 * Get the coordinates of a specific LED
 *
 * LED ID numbers count left-to-right. Bounds are the layer bounds to be drawn in
 */
static GPoint get_center_of_led(int id_number, GRect bounds) {
  GPoint center = grect_center_point(&bounds);
  int offset_start = center.x - (pad_size+pad_gap)*2 + pad_gap/2;

  #if defined(PBL_ROUND)
    // Round display; arc the pads up into the curved top
    int vertical_offset =  (id_number == 1 || id_number == 2) ? 12 : 32;
  #else
    int vertical_offset = 5;
  #endif
  return GPoint(offset_start + id_number*(pad_size+pad_gap) + pad_size/2, vertical_offset+pad_size/2);
}

/**
 * Get the color of a given LED, given the current hour
 */
static GColor get_color_of_led(int id_number) {
  HoursContext *ctx = (HoursContext*)layer_get_data(graphics_layer);

  switch(id_number) {
    case 0:
      return (ctx->hours & 8) ? LED_ON_COLOR : LED_OFF_COLOR;
    case 1:
      return (ctx->hours & 4) ? LED_ON_COLOR : LED_OFF_COLOR;
    case 2:
      return (ctx->hours & 2) ? LED_ON_COLOR : LED_OFF_COLOR;
    case 3:
      return (ctx->hours & 1) ? LED_ON_COLOR : LED_OFF_COLOR;
  }
  return LED_OFF_COLOR;
}

/**
 * Update the graphics layer
 */
static void draw(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);

  graphics_context_set_stroke_color(ctx, GColorWhite);

  // Draw the LED pads
  for(int i = 0; i < 4; i++) {
    GPoint led_center = get_center_of_led(i, bounds);
    graphics_full_rect(ctx, GRect(led_center.x-pad_size/2, led_center.y-pad_size/2, pad_size, pad_size), GColorBlack, GColorWhite);
    graphics_context_set_fill_color(ctx, get_color_of_led(i));
    graphics_fill_rect(ctx, GRect(led_center.x-pad_size/2+2, led_center.y-pad_size/2+2, pad_size-4, pad_size-4), 0, GCornerNone);
  }
}

/**
 * Draw the initial graphics for the LED pads for hours display
 */
void init_hours(Layer *parent_layer) {
  GRect bounds = layer_get_bounds(parent_layer);

  graphics_layer = layer_create_with_data(bounds, sizeof(HoursContext));
  HoursContext *ctx = (HoursContext*)layer_get_data(graphics_layer);
  ctx->hours = 0;

  layer_set_update_proc(graphics_layer, draw);
  layer_add_child(
    parent_layer,
    graphics_layer
  );
}

/**
 * Set the hours display
 */
void update_hours(short hour) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Minutes set to %d", hour);
  HoursContext *ctx = (HoursContext*)layer_get_data(graphics_layer);
  ctx->hours = hour;
}

/**
 * Unload the graphics from the display
 */
void unload_hours() {
  layer_destroy(graphics_layer);
}
