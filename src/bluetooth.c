#include <pebble.h>
#include "bluetooth.h"
#include "util.h"

static int pad_size = 11;
static Layer *graphics_layer;
typedef struct {
  bool connected;
} BluetoothContext;

/**
 * Update the graphics layer
 */
static void draw(Layer *layer, GContext *ctx) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Redrawing Bluetooth layer");
  if (SHOW_BLUETOOTH == false) return;
  
  GRect bounds = layer_get_bounds(layer);
  GPoint led_center = point_add(grect_center_point(&bounds), GPoint(55, -18));

  graphics_full_rect(ctx, GRect(led_center.x-pad_size/2, led_center.y-pad_size/2, pad_size, pad_size), GColorBlack, GColorWhite);
  
  BluetoothContext *bt_ctx = (BluetoothContext*)layer_get_data(graphics_layer);
  if (bt_ctx->connected) {
    graphics_context_set_fill_color(ctx, LED_ON_COLOR);
  } else {
    graphics_context_set_fill_color(ctx, LED_OFF_COLOR);
  }
  graphics_fill_rect(ctx, GRect(led_center.x-pad_size/2+2, led_center.y-pad_size/2+2, pad_size-4, pad_size-4), 0, GCornerNone);
}

/**
 * Draw the initial graphics for the LED pads for bluetooth connectivity
 */
void init_bluetooth(Layer *parent_layer) {
  GRect bounds = layer_get_bounds(parent_layer);

  graphics_layer = layer_create_with_data(bounds, sizeof(BluetoothContext));
  BluetoothContext *ctx = (BluetoothContext*)layer_get_data(graphics_layer);
  ctx->connected = false;

  layer_set_update_proc(graphics_layer, draw);
  layer_add_child(
    parent_layer,
    graphics_layer
  );
}

/**
 * Set the bluetooth display
 */
void update_bluetooth(bool connected) {
  if (connected) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Bluetooth connected");
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Bluetooth disconnected");
  }
  BluetoothContext *ctx = (BluetoothContext*)layer_get_data(graphics_layer);
  ctx->connected = connected;
  layer_mark_dirty(graphics_layer);
}

/**
 * Unload the graphics from the display
 */
void unload_bluetooth() {
  layer_destroy(graphics_layer);
}