#pragma once
GColor BACKGROUND_COLOR;
GColor LED_ON_COLOR;
GColor LED_OFF_COLOR;
GColor CHIP_COLOR;
GColor CHIP_OUTLINE_COLOR;
GColor CHIP_TEXT_COLOR;
GColor TRACE_COLOR;

void drawPathAt(GContext *ctx, GPath *path, GPoint offset);
GPoint point_add(GPoint a, GPoint b);
void graphics_full_rect(GContext *ctx, GRect bounds, GColor fill, GColor stroke);
