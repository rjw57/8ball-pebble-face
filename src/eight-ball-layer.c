#include "eight-ball-layer.h"

#include <pebble.h>

struct EightBallLayer_ {
    Layer       *base_layer;
    TextLayer   *fortune_layer;
};

void eight_ball_layer_update_proc_(Layer* layer, GContext* ctx)
{
    GRect bounds = layer_get_bounds(layer);
    GPoint mid = {bounds.size.w>>1, bounds.size.h>>1};
    const GPathInfo path_info = {
        .num_points = 4,
        .points = (GPoint []) {
            {mid.x-50, mid.y+30},
            {mid.x+50, mid.y+30},
            {mid.x, mid.y-50},
            {mid.x-50, mid.y+30},
        },
    };

    EightBallLayer* ebl = *((EightBallLayer**)layer_get_data(layer));

    /* Draw ball */
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_circle(ctx, (GPoint) { bounds.size.w>>1, bounds.size.h>>1 }, bounds.size.w>>1);

    /* Draw triangle */
    GPath* tri = gpath_create(&path_info);
    graphics_context_set_fill_color(ctx, GColorWhite);
    gpath_draw_filled(ctx, tri);
    gpath_destroy(tri);
}

EightBallLayer* eight_ball_layer_create(GRect frame)
{
    /* Create EightBallLayer structure. */
    EightBallLayer* ebl = (EightBallLayer*) malloc(sizeof(EightBallLayer));
    if(ebl == NULL)
        return NULL;

    /* Initialise struct */
    *ebl = (EightBallLayer) {
        .base_layer = layer_create_with_data(frame, sizeof(EightBallLayer*)),
        .fortune_layer = text_layer_create( (GRect) {
            .origin = {(frame.size.w-80)>>1, (frame.size.h-40)>>1},
            .size = {80, 40},
        }),
    };

    *((EightBallLayer**)layer_get_data(ebl->base_layer)) = ebl;
    layer_set_update_proc(ebl->base_layer, eight_ball_layer_update_proc_);

    text_layer_set_text_alignment(ebl->fortune_layer, GTextAlignmentCenter);
    text_layer_set_overflow_mode(ebl->fortune_layer, GTextOverflowModeWordWrap);
    text_layer_set_font(ebl->fortune_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));

    layer_add_child(ebl->base_layer, text_layer_get_layer(ebl->fortune_layer));

    return ebl;
}

void eight_ball_layer_destroy(EightBallLayer* eight_ball_layer)
{
    if(NULL == eight_ball_layer)
        return;

    text_layer_destroy(eight_ball_layer->fortune_layer);
    layer_destroy(eight_ball_layer->base_layer);
    free(eight_ball_layer);
}

Layer* eight_ball_layer_get_layer(const EightBallLayer* eight_ball_layer)
{
    return eight_ball_layer->base_layer;
}

void eight_ball_layer_set_text(EightBallLayer* eight_ball_layer, const char* text)
{
    text_layer_set_text(eight_ball_layer->fortune_layer, text);
}

const char* eight_ball_layer_get_text(const EightBallLayer* eight_ball_layer)
{
    return text_layer_get_text(eight_ball_layer->fortune_layer);
}
