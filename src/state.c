// vim: noet ts=4 sw=4
#include "state.h"

void init_state(struct app_state_t *state) {
	state->current_time = time(NULL);
	state->last_time_triggered_redraw = 0;
	state->last_update_time = 0;
	state->last_draw_time = 0;
	state->dirty = true;
}

void update_state(struct app_state_t *state) {
	state->current_time = time(NULL);
	if (!state->last_time_triggered_redraw ||
			state->current_time - state->last_time_triggered_redraw >= 1) {
		state->last_time_triggered_redraw = state->current_time;
		state->dirty = true;
	}
}

void update_state_with_keypress(struct app_state_t *state, const int ch) {
	state->last_key_pressed = ch;
	switch (ch) {
		case KEY_ESCAPE:
			//state->should_exit = true;
			break;
		case '\t':
			state->current_window_idx += 1;
			state->current_window_idx = state->current_window_idx % 3;
			state->dirty = true;
			break;
	}
}
