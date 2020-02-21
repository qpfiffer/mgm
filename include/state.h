// vim: noet ts=4 sw=4
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "app.h"
#include "vector.h"

#define KEY_ESCAPE 27

struct drawable_t;
struct app_state_t {
	bool should_exit;

	WINDOW *left_w_outer, *middle_w_outer, *right_w_outer;
	WINDOW *left_w_i, *middle_w_i, *right_w_i;

	struct drawable_t *windows;

	int current_window_idx;

	time_t current_time;
	time_t last_time_triggered_redraw;
	int last_key_pressed;

	// Update totals:
	time_t last_update_time;
	time_t update_dt;
	time_t update_dtotal;

	// Draw Stuff:
	time_t last_draw_time;
	time_t draw_dt;
	time_t draw_dtotal;
	bool dirty;
};

void init_state(struct app_state_t *state);
void update_state(struct app_state_t *state);
void update_state_with_keypress(struct app_state_t *state, const vector *key_presses);
