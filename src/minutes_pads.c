#include <pebble.h>
#include "minutes_pads.h"
#include "util.h"

/**
 * Handle the drawing and updating of the graphics for the LED pads for minutes display
 */

static int pad_size = 18;
static int pad_gap = 4;
static Layer *graphics_layer;
typedef struct {
  unsigned short minutes;
} MinutesContext;

/**
 * Get the coordinates of a specific LED
 *
 * LED ID numbers count left-to-right. Bounds are the layer bounds to be drawn in
 */
static GPoint get_center_of_led(int id_number, GRect bounds) {
  GPoint center = grect_center_point(&bounds);
  int offset_start = center.x - (pad_size+pad_gap)*3 + pad_gap/2;

  #if defined(PBL_ROUND)
    // Round display; arc the pads up into the curved top
    int vertical_offset = 36;
    if (id_number == 2 || id_number == 3) {
      vertical_offset = 8;
    } else if (id_number == 1 || id_number == 4) {
      vertical_offset = 22;
    }
  #else
    int vertical_offset = 5;
  #endif
  return GPoint(offset_start + id_number*(pad_size+pad_gap) + pad_size/2, bounds.size.h-vertical_offset-pad_size/2);
}

/**
 * Get the color of a given LED, given the current hour
 */
static GColor get_color_of_led(int id_number) {
  MinutesContext *ctx = (MinutesContext*)layer_get_data(graphics_layer);

  switch(id_number) {
    case 0:
      return (ctx->minutes & 32) ? LED_ON_COLOR : LED_OFF_COLOR;
    case 1:
      return (ctx->minutes & 16) ? LED_ON_COLOR : LED_OFF_COLOR;
    case 2:
      return (ctx->minutes & 8) ? LED_ON_COLOR : LED_OFF_COLOR;
    case 3:
      return (ctx->minutes & 4) ? LED_ON_COLOR : LED_OFF_COLOR;
    case 4:
      return (ctx->minutes & 2) ? LED_ON_COLOR : LED_OFF_COLOR;
    case 5:
      return (ctx->minutes & 1) ? LED_ON_COLOR : LED_OFF_COLOR;
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
  for(int i = 0; i < 6; i++) {
    GPoint led_center = get_center_of_led(i, bounds);
    graphics_full_rect(ctx, GRect(led_center.x-pad_size/2, led_center.y-pad_size/2, pad_size, pad_size), GColorBlack, GColorWhite);
    graphics_context_set_fill_color(ctx, get_color_of_led(i));
    graphics_fill_rect(ctx, GRect(led_center.x-pad_size/2+2, led_center.y-pad_size/2+2, pad_size-4, pad_size-4), 0, GCornerNone);
  }
}

/**
 * Draw the initial graphics for the LED pads for hours display
 */
void init_minutes(Layer *parent_layer) {
  GRect bounds = layer_get_bounds(parent_layer);

  graphics_layer = layer_create_with_data(bounds, sizeof(MinutesContext));
  MinutesContext *ctx = (MinutesContext*)layer_get_data(graphics_layer);
  ctx->minutes = 0;

  layer_set_update_proc(graphics_layer, draw);
  layer_add_child(
    parent_layer,
    graphics_layer
  );
}

/**
 * Set the minutes display
 */
void update_minutes(short minutes) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Minutes set to %d", minutes);
  MinutesContext *ctx = (MinutesContext*)layer_get_data(graphics_layer);
  ctx->minutes = minutes;
}

/**
 * Unload the graphics from the display
 */
void unload_minutes() {
  layer_destroy(graphics_layer);
}
