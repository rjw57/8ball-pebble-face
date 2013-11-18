#include <pebble.h>
#include "eight-ball.h"
#include "eight-ball-layer.h"

static Window *window;
static TextLayer *text_layer;
static BitmapLayer *background_layer;
static GBitmap* background_bitmap;

#define TEXT_LEN 32

static void set_fortune_text(const char* text)
{
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    layer_set_frame(text_layer_get_layer(text_layer), (GRect) {
            .origin = { 20, 20 },
            .size = { bounds.size.w-40, bounds.size.h-40 }
    });

    text_layer_set_text(text_layer, text);

    GSize content_size = text_layer_get_content_size(text_layer);
    layer_set_frame(text_layer_get_layer(text_layer), (GRect) {
            .origin = { 20, ((bounds.size.h-content_size.h)>>1) - 5},
            .size = { bounds.size.w-40, content_size.h+20 }
    });
}

// Called once per minute
static void handle_minute_tick(struct tm* tick_time, TimeUnits units_changed) {
    static char tmp_str[TEXT_LEN];
    strftime(tmp_str, sizeof(tmp_str), "%H:%M", tick_time);
    set_fortune_text(tmp_str);
}

static void handle_tap(AccelAxisType axis, int32_t direction)
{
    set_fortune_text(eight_ball_messages[rand() % EIGHT_BALL_MESSAGE_COUNT]);
}

static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    text_layer = text_layer_create((GRect) {
            .origin = { 20, 20 },
            .size = { bounds.size.w-40, bounds.size.h-40 }
    });
    text_layer_set_background_color(text_layer, GColorClear);
    text_layer_set_text_color(text_layer, GColorWhite);
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
    text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_overflow_mode(text_layer, GTextOverflowModeWordWrap);

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
    layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
    text_layer_destroy(text_layer);
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
