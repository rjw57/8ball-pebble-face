/* A Layer which displays one of two bitmaps depending on a toggle state. */

#ifndef EIGHT_BALL_LAYER_H__
#define EIGHT_BALL_LAYER_H__

#include <pebble.h>

/* An opaque type representing a EightBallLayer */
typedef struct EightBallLayer_ EightBallLayer;

/* Create a new EightBallLayer */
EightBallLayer* eight_ball_layer_create(GRect frame);

/* Destroy an existing EightBallLayer. */
void eight_ball_layer_destroy(EightBallLayer* eight_ball_layer);

/* Gets the "root" Layer of the bitmap layer, which is the parent for the sub-
 * layers used for its implementation. */
Layer* eight_ball_layer_get_layer(const EightBallLayer* eight_ball_layer);

/* Set fortune text. */
void eight_ball_layer_set_text(EightBallLayer* eight_ball_layer, const char* text);

/* Get the fortune text. */
const char* eight_ball_layer_get_text(const EightBallLayer* eight_ball_layer);

#endif /* EIGHT_BALL_LAYER_H__ */
