#include <pebble.h>
#include "util.h"

/**
 * Draw an open-ended path, offset by a given amount
 */
#ifdef PBL_SDK_3
void drawPathAt(GContext *ctx, GPath *path, GPoint offset) {
  gpath_move_to(path, offset);
  gpath_draw_outline_open(ctx, path);
}
#else
void drawPathAt(GContext *ctx, GPath *path, GPoint offset) {
}
#endif

/**
 * Add two GPoints together.
 *
 * Effectively, a means to offset one GPoint by a certain amount.
 */
GPoint point_add(GPoint a, GPoint b) {
  return GPoint(a.x + b.x, a.y + b.y);
}

/**
 * Move the origin of a GRect by a certain amount
 */
GRect rect_shift(GRect rect, GPoint offset) {
  return GRect(rect.origin.x+offset.x, rect.origin.y+offset.y, rect.size.w, rect.size.h);
}

/**
 * Draw a filled and stroked rectangle at a given location
 */
void graphics_full_rect(GContext *ctx, GRect bounds, GColor fill, GColor stroke) {
  graphics_context_set_stroke_color(ctx, stroke);
  graphics_context_set_fill_color(ctx, fill);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);
  graphics_draw_rect(ctx, bounds);
}