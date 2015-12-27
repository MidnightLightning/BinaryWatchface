#include <pebble.h>
#include "traces.h"
#include "util.h"

#define NUM_PATHS 17

/**
 * Handle the drawing and updating of the graphics for the circuit board traces
 */
static const GPathInfo TRACE1 = {  // Hours 8
  .num_points = 4,
  .points = (GPoint []) {
    {0, 0},
    {0, -25},
    {-34, -42}, // 2/1
    {-34, -70}
  }
};
static const GPathInfo TRACE2 = { // Hours 4
  .num_points = 4,
  .points = (GPoint []) {
    {0, 0},
    {0, -37},
    {-22, -48}, // 2/1
    {-22, -70}
  }
};
static const GPathInfo TRACE3 = { // Hours 2
  .num_points = 2,
  .points = (GPoint []) {
    {0, 0},
    {0, -70}
  }
};
static const GPathInfo TRACE4 = { // Hours 1
  .num_points = 4,
  .points = (GPoint []) {
    {0, 0},
    {0, -35},
    {22, -46}, // 2/1
    {22, -70}
  }
};
static const GPathInfo TRACE5 = {
  .num_points = 2,
  .points = (GPoint []) {
    {0, 0},
    {0, -30}
  }
};
static const GPathInfo TRACE6 = { // Minutes 1
  .num_points = 3,
  .points = (GPoint []) {
    {0, 0},
    {30, 0},
    {30, 70}
  }
};
static const GPathInfo TRACE7 = { // Minutes 2
  .num_points = 5,
  .points = (GPoint []) {
    {0, 0},
    {20, 0},
    {20, 40},
    {8, 46},
    {8, 70}
  }
};
static const GPathInfo TRACE8 = { // Minutes 4
  .num_points = 4,
  .points = (GPoint []) {
    {0, 0},
    {0, 40},
    {-5, 43},
    {-5, 60}
  }
};
static const GPathInfo TRACE9 = { // Minutes 8
  .num_points = 4,
  .points = (GPoint []) {
    {0, 0},
    {0, 35},
    {-16, 43},
    {-16, 60}
  }
};
static const GPathInfo TRACE10 = { // Minutes 16
  .num_points = 4,
  .points = (GPoint []) {
    {0, 0},
    {0, 30},
    {-28, 44},
    {-28, 60}
  }
};
static const GPathInfo TRACE11 = { // Minutes 32
  .num_points = 4,
  .points = (GPoint []) {
    {0, 0},
    {0, 25},
    {-40, 45},
    {-40, 60}
  }
};
static const GPathInfo TRACE12 = {
  .num_points = 2,
  .points = (GPoint []) {
    {0, 0},
    {0, 30}
  }
};
static const GPathInfo TRACE13 = {
  .num_points = 3,
  .points = (GPoint []) {
    {0, 0},
    {0, -30},
    {40, -50}
  }
};
static const GPathInfo TRACE14 = { // Battery 1
  .num_points = 4,
  .points = (GPoint []) {
    {0, 0},
    {-20, 0},
    {-50, -15},
    {-60, -15}
  }
};
static const GPathInfo TRACE15 = { // Battery 2
  .num_points = 4,
  .points = (GPoint []) {
    {0, 0},
    {-25, 0},
    {-35, -5},
    {-60, -5}
  }
};
static const GPathInfo TRACE16 = { // Battery 3
  .num_points = 2,
  .points = (GPoint []) {
    {0, 0},
    {-20, 0}
  }
};
static const GPathInfo TRACE17 = { // Battery 4
  .num_points = 3,
  .points = (GPoint []) {
    {0, 0},
    {0, 45},
    {-30, 60}
  }
};

static GPath *trace_paths[NUM_PATHS];

static Layer *graphics_layer;


static void drawCirclePad(GContext *ctx, GPoint offset) {
  graphics_fill_circle(ctx, offset, 3);
  graphics_draw_circle(ctx, offset, 3);
}

/**
 * Update the graphics layer
 */
static void draw(Layer *layer, GContext *ctx) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Redrawing Traces layer");
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);

  graphics_context_set_stroke_color(ctx, TRACE_COLOR);
  graphics_context_set_fill_color(ctx, BACKGROUND_COLOR);
  #ifdef PBL_SDK_3
    graphics_context_set_stroke_width(ctx, 5); // Stroke width isn't available on Aplite
  #endif

  drawPathAt(ctx, trace_paths[0], point_add(center, GPoint(-14, 0))); // Hours 8
  drawPathAt(ctx, trace_paths[1], point_add(center, GPoint(6, 0))); // Hours 4
  drawPathAt(ctx, trace_paths[2], point_add(center, GPoint(16, 0))); // Hours 2
  drawPathAt(ctx, trace_paths[3], point_add(center, GPoint(26, 0))); // Hours 1
  drawPathAt(ctx, trace_paths[4], point_add(center, GPoint(-4, 0))); // Stub

  drawPathAt(ctx, trace_paths[5], point_add(center, GPoint(26, -8))); // Minutes 1
  drawPathAt(ctx, trace_paths[6], point_add(center, GPoint(26, 2))); // Minutes 2
  drawPathAt(ctx, trace_paths[7], point_add(center, GPoint(16, 10))); // Minutes 4
  drawPathAt(ctx, trace_paths[8], point_add(center, GPoint(6, 10))); // Minutes 8
  drawPathAt(ctx, trace_paths[9], point_add(center, GPoint(-4, 10))); // Minutes 16
  drawPathAt(ctx, trace_paths[10], point_add(center, GPoint(-14, 10))); // Minutes 32
  drawPathAt(ctx, trace_paths[11], point_add(center, GPoint(26, 10))); // Stub
  drawPathAt(ctx, trace_paths[12], point_add(center, GPoint(56, -8))); // Stub

  drawPathAt(ctx, trace_paths[13], point_add(center, GPoint(-14, -8))); // Battery
  drawPathAt(ctx, trace_paths[14], point_add(center, GPoint(-14, 2))); // Battery
  drawPathAt(ctx, trace_paths[15], point_add(center, GPoint(-14, 12))); // Battery
  drawPathAt(ctx, trace_paths[16], point_add(center, GPoint(-55, -3))); // Battery

  #ifdef PBL_SDK_3
    graphics_context_set_stroke_width(ctx, 3); // Stroke width isn't available on Aplite
  #endif
  drawCirclePad(ctx, point_add(center, GPoint(-4, -30)));
  drawCirclePad(ctx, point_add(center, GPoint(26, 30)));
  drawCirclePad(ctx, point_add(center, GPoint(26, 40)));
  drawCirclePad(ctx, point_add(center, GPoint(56, -18)));
  drawCirclePad(ctx, point_add(center, GPoint(69, -18)));
  drawCirclePad(ctx, point_add(center, GPoint(56, 2)));
  drawCirclePad(ctx, point_add(center, GPoint(69, 2)));
  drawCirclePad(ctx, point_add(center, GPoint(-34, 12)));
}

/**
 * Draw the initial graphics for the circuit board traces
 */
void init_traces(Layer *parent_layer) {
  GRect bounds = layer_get_bounds(parent_layer);

  trace_paths[0] = gpath_create(&TRACE1);
  trace_paths[1] = gpath_create(&TRACE2);
  trace_paths[2] = gpath_create(&TRACE3);
  trace_paths[3] = gpath_create(&TRACE4);
  trace_paths[4] = gpath_create(&TRACE5);
  trace_paths[5] = gpath_create(&TRACE6);
  trace_paths[6] = gpath_create(&TRACE7);
  trace_paths[7] = gpath_create(&TRACE8);
  trace_paths[8] = gpath_create(&TRACE9);
  trace_paths[9] = gpath_create(&TRACE10);
  trace_paths[10] = gpath_create(&TRACE11);
  trace_paths[11] = gpath_create(&TRACE12);
  trace_paths[12] = gpath_create(&TRACE13);
  trace_paths[13] = gpath_create(&TRACE14);
  trace_paths[14] = gpath_create(&TRACE15);
  trace_paths[15] = gpath_create(&TRACE16);
  trace_paths[16] = gpath_create(&TRACE17);

  graphics_layer = layer_create(bounds);

  layer_set_update_proc(graphics_layer, draw);
  layer_add_child(
    parent_layer,
    graphics_layer
  );
}

/**
 * Unload the graphics from the display
 */
void unload_traces() {
  layer_destroy(graphics_layer);
  for (int i = 0; i < NUM_PATHS; i++) {
    gpath_destroy(trace_paths[i]);
  }
}
