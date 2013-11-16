#include <pebble.h>

static Window *window;
static TextLayer *text_layer;

#define TEXT_LEN 32

// Called once per minute
static void handle_minute_tick(struct tm* tick_time, TimeUnits units_changed) {
    static char tmp_str[TEXT_LEN];
    strftime(tmp_str, sizeof(tmp_str), "%H:%M", tick_time);
    text_layer_set_text(text_layer, tmp_str);
}

static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
    text_layer_set_text(text_layer, "Clem is a poo");
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);

    // Ensures time is displayed immediately (will break if NULL tick event accessed).
    // (This is why it's a good idea to have a separate routine to do the update itself.)
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    handle_minute_tick(current_time, MINUTE_UNIT);
    tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);

    layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
    text_layer_destroy(text_layer);
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
