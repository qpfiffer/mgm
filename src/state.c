// vim: noet ts=4 sw=4
#include "draw.h"
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
	struct drawable_t *current_drawable = &state->windows[state->current_window_idx];
	unsigned int i = 0;
	memset(state->last_key_pressed, '\0', sizeof(state->last_key_pressed));

	int print_iter = 0;
	bool escape_triggered = false;
	bool square_thingie_triggered = false;

	for (i = 0; i < key_presses->count; i++) {
		const int *_ch = (int *)vector_get(key_presses, i);
		const int ch = *_ch;

		print_iter += snprintf(state->last_key_pressed + print_iter, sizeof(state->last_key_pressed) - print_iter, " %i,", ch);
		switch (ch) {
			case KEY_ESCAPE:
				escape_triggered = true;
				if (key_presses->count <= 1) {
					state->should_exit = true;
				}
				break;
			case '[':
				if (escape_triggered) {
					square_thingie_triggered = true;
				}
				break;
			case 'Z':
				if (escape_triggered && square_thingie_triggered) {
					state->current_window_idx -= 1;
					if (state->current_window_idx < 0)
						state->current_window_idx = 2;
					state->dirty = true;
				}
				break;
			case '\t':
			case 'l':
			case 67: /* Right */
				state->current_window_idx += 1;
				state->current_window_idx = state->current_window_idx % 3;
				state->dirty = true;
				break;
			case 'h':
			case 68: /* Left */
				state->current_window_idx -= 1;
				if (state->current_window_idx < 0)
					state->current_window_idx = 2;
				state->dirty = true;
				break;
			case 'k':
			case 65: /* Up Arrow */
				current_drawable->highlighted_idx -= 1;
				state->dirty = true;
				break;
			case 'j':
			case 66: /* Down Arrow */
				current_drawable->highlighted_idx += 1;
				state->dirty = true;
				break;
			default:
				state->dirty = true;
				break;
		}
	}
}
