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

void update_state_with_keypress(struct app_state_t *state, const vector *key_presses) {
	unsigned int i = 0;
	for (i = 0; i < key_presses->count; i++) {
		const int *_ch = (int *)vector_get(key_presses, i);
		const int ch = *_ch;

		state->last_key_pressed = ch;
		switch (ch) {
			case KEY_ESCAPE:
				if (key_presses->count <= 1)
					state->should_exit = true;
				break;
			case '\t':
				state->current_window_idx += 1;
				state->current_window_idx = state->current_window_idx % 3;
				state->dirty = true;
				break;
			default:
				state->dirty = true;
				break;
		}
	}
}
