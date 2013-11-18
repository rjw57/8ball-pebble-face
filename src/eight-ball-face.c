#include <pebble.h>
#include "eight-ball.h"
#include "eight-ball-layer.h"

static Window *window;
static EightBallLayer *eight_ball_layer;
static BitmapLayer *background_layer;
static GBitmap* background_bitmap;

#define TEXT_LEN 32

// Called once per minute
static void handle_minute_tick(struct tm* tick_time, TimeUnits units_changed) {
    static char tmp_str[TEXT_LEN];
    strftime(tmp_str, sizeof(tmp_str), "%H:%M", tick_time);
    eight_ball_layer_set_text(eight_ball_layer, tmp_str);
}

static void handle_tap(AccelAxisType axis, int32_t direction)
{
    eight_ball_layer_set_text(eight_ball_layer, eight_ball_messages[rand() % EIGHT_BALL_MESSAGE_COUNT]);
}

static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    eight_ball_layer = eight_ball_layer_create((GRect) {
            .origin = { 0, (bounds.size.h-bounds.size.w)>>1},
            .size = { bounds.size.w, bounds.size.w }
    });

    background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND);

    background_layer = bitmap_layer_create(bounds);
    bitmap_layer_set_bitmap(background_layer, background_bitmap);

    // Ensures time is displayed immediately (will break if NULL tick event accessed).
    // (This is why it's a good idea to have a separate routine to do the update itself.)
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    handle_minute_tick(current_time, MINUTE_UNIT);
    tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);

    accel_tap_service_subscribe(&handle_tap);

    layer_add_child(window_layer, bitmap_layer_get_layer(background_layer));
    layer_add_child(window_layer, eight_ball_layer_get_layer(eight_ball_layer));
}

static void window_unload(Window *window) {
    eight_ball_layer_destroy(eight_ball_layer);
    bitmap_layer_destroy(background_layer);
    gbitmap_destroy(background_bitmap);
}

static void init(void) {
    window = window_create();
    window_set_window_handlers(window, (WindowHandlers) {
            .load = window_load,
            .unload = window_unload,
            });
    window_stack_push(window, /* animated = */ true);
}

static void deinit(void) {
    window_destroy(window);
}

int main(void) {
    init();

    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

    app_event_loop();
    deinit();
}
