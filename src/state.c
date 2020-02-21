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
	struct drawable_t *current_drawable = &state->windows[state->current_window_idx];
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
			case 67:
				state->current_window_idx += 1;
				state->current_window_idx = state->current_window_idx % 3;
				state->dirty = true;
				break;
			case 68:
				state->current_window_idx -= 1;
				if (state->current_window_idx < 0)
					state->current_window_idx = 2;
				state->dirty = true;
				break;
			case 65:
				current_drawable->highlighted_idx -= 1;
				break;
			case 66:
				current_drawable->highlighted_idx += 1;
				break;
			default:
				state->dirty = true;
				break;
		}
	}
}

void draw_left_items(const struct drawable_t *self, const struct app_state_t *main_state, const bool is_focused) {
	(void)is_focused;
	char buf[128] = {0};
	struct tm *info = NULL;
	info = localtime(&main_state->current_time);
	strftime(buf, sizeof(buf), "%y/%m/%d (%H:%M:%S):", info);

	waddstr(self->inner_w, buf);
	memset(buf, '\0', sizeof(buf));
	snprintf(buf, sizeof(buf), " %i", main_state->last_key_pressed);
	waddstr(self->inner_w, buf);
}

void draw_middle_items(const struct drawable_t *self, const struct app_state_t *main_state, const bool is_focused) {
	(void)main_state;
	int cursor_iter = 0;
	wmove(self->inner_w, cursor_iter++, 0);

	const char *items[] = {
		"Personal Log:",
		"* Coffee: ",
		"* Meals: ",
		"  * Breakfast: ",
		"  * Lunch: ",
		"  * Dinner: ",
		"* Ailments: ",
	};

	unsigned int i = 0;
	for (i = 0; i < sizeof(items)/sizeof(items[0]); i++) {
		const char *item = items[i];
		if (is_focused && i == self->highlighted_idx)
			wattron(self->inner_w, A_REVERSE);
		waddstr(self->inner_w, item);
		wattroff(self->inner_w, A_REVERSE);

		wmove(self->inner_w, cursor_iter++, 0);
	}
}

void draw_right_items(const struct drawable_t *self, const struct app_state_t *main_state, const bool is_focused) {
	(void)main_state;
	(void)is_focused;
	int cursor_iter = 0;
	wmove(self->inner_w, cursor_iter++, 0);

	waddstr(self->inner_w, "Search: ________________");
	wmove(self->inner_w, cursor_iter++, 0);

	waddstr(self->inner_w, "* Test item 1: ");
	wmove(self->inner_w, cursor_iter++, 0);

	waddstr(self->inner_w, "* Test item 2: ");
	wmove(self->inner_w, cursor_iter++, 0);

	waddstr(self->inner_w, "* Test item 3: ");
	wmove(self->inner_w, cursor_iter++, 0);
}

