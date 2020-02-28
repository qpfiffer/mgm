// vim: noet ts=4 sw=4
#include "app.h"
#include "vector.h"

#define KEY_ESCAPE 27

#define USE_HOME true
#define DATABASE_PATH ".mgm"
#define DATABASE_FILENAME "database.sqlite3"

struct drawable_t;
struct app_state_t {
	bool should_exit;

	/* The raw ncurses handles to each window */
	WINDOW *left_w_outer, *middle_w_outer, *right_w_outer;
	WINDOW *left_w_i, *middle_w_i, *right_w_i;

	/* The three windows */
	struct drawable_t *windows;

	/* Which window is highlighted */
	int current_window_idx;

	/* Current time is the current time in the top-left corner */
	time_t current_time;
	/* The last time we triggered a redraw based on the clock ticking */
	time_t last_time_triggered_redraw;
	/* The last key that was pressed */
	char last_key_pressed[32];

	/* Update totals: */
	time_t last_update_time;
	time_t update_dt;
	time_t update_dtotal;

	/* Draw Stuff: */
	time_t last_draw_time;
	time_t draw_dt;
	time_t draw_dtotal;
	/* 'dirty' flags whether or not we should redraw everything */
	bool dirty;

	/* Data stuff: */
	sqlite3 *database;
};

void init_state(struct app_state_t *state);
void update_state(struct app_state_t *state);
void update_state_with_keypress(struct app_state_t *state, const vector *key_presses);
