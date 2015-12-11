#include <pebble.h>
#include "processor.h"
#include "util.h"

/**
 * Handle the drawing and updating of the graphics for the processor chip
 * in the middle of the display
 */

static TextLayer *processor_text;

static const GPathInfo PROCESSOR_CHIP = {
  .num_points = 8,
  .points = (GPoint []) {
    {30, -15},
    {30, 15},
    {25, 20},
    {-25, 20},
    {-30, 15},
    {-30, -15},
    {-25, -20},
    {25, -20}
  }
};
static GPath *processor_chip_path;

/**
 * Update the graphics layer
 */
static void draw(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);

  gpath_move_to(processor_chip_path, point_add(center, GPoint(6, 2)));

  #ifdef PBL_COLOR
    graphics_context_set_fill_color(ctx, GColorBlack);
    gpath_draw_filled(ctx, processor_chip_path);
  #endif

  graphics_context_set_stroke_color(ctx, GColorWhite);
  #ifdef PBL_SDK_3
    graphics_context_set_stroke_width(ctx, 3); // Stroke width isn't available on Aplite
  #endif
  gpath_draw_outline(ctx, processor_chip_path);

  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, point_add(center, GPoint(26, -8)), 3);
}

/**
 * Draw the initial graphics for the processor chip in the middle of the display
 */
void init_processor(Layer *parent_layer) {
  GRect bounds = layer_get_bounds(parent_layer);
  GPoint center = grect_center_point(&bounds);

  // Draw the chip outline
  processor_chip_path = gpath_create(&PROCESSOR_CHIP);

  Layer *graphics_layer = layer_create(bounds);
  layer_set_update_proc(graphics_layer, draw);
  layer_add_child(
    parent_layer,
    graphics_layer
  );

  // Draw the text on top
  processor_text = text_layer_create(GRect(center.x-17, center.y-17, 45, 25));
  text_layer_set_text(processor_text, "Hello, Pebble!");

  #ifdef PBL_COLOR
    text_layer_set_text_color(processor_text, GColorLightGray);
  #else
    text_layer_set_text_color(processor_text, GColorWhite);
  #endif
  text_layer_set_background_color(processor_text, GColorClear);
  text_layer_set_font(processor_text, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(processor_text, GTextAlignmentLeft);

  // Add text layer to window stack
  layer_add_child(
    parent_layer,
    text_layer_get_layer(processor_text)
  );
}

/**
 * Set the text for the "serial number" of the processor
 */
void update_processor(char *message) {
  text_layer_set_text(processor_text, message);
}

/**
 * Unload the graphics from the display
 */
void unload_processor() {
  text_layer_destroy(processor_text);
  gpath_destroy(processor_chip_path);
}
