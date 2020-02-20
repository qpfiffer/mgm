// vim: noet ts=4 sw=4
#include "state.h"

void init_state(struct app_state_t *state) {
	time_t current_time = time(NULL);
	state->current_time = current_time;
	state->last_time_redraw = current_time;
	state->last_update_time = current_time;
	state->last_draw_time = current_time;
	state->dirty = true;
}

void update_state(struct app_state_t *state) {
	state->current_time = time(NULL);
	if (state->current_time - state->last_time_redraw >= 1) {
		state->last_time_redraw = state->current_time;
		state->dirty = true;
	}
}
