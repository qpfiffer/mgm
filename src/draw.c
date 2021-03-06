// vim: noet ts=4 sw=4
#include "app.h"
#include "state.h"
#include "draw.h"

void draw_time(const struct drawable_t *self, const struct app_state_t *main_state) {
	char buf[128] = {0};
	struct tm *info = NULL;
	info = localtime(&main_state->current_time);
	strftime(buf, sizeof(buf), "%y/%m/%d (%H:%M:%S):", info);

	waddstr(self->inner_w, buf);
	waddstr(self->inner_w, main_state->last_key_pressed);
}

void draw_left_items(const struct drawable_t *self, const struct app_state_t *main_state, const bool is_focused) {
	(void)is_focused;
	int cursor_iter = 0;
	wmove(self->inner_w, cursor_iter++, 0);

	draw_time(self, main_state);
	wmove(self->inner_w, cursor_iter++, 0);

	/* Maybe move this to state eventually? We probably don't
	 * need to query it *all* the time.
	 */
	unsigned int i = 0;
	if (self->entries) {
		for (i = 0; i < self->entries->count; i++) {
			const struct entry_t *entry = vector_get(self->entries, i);
			const char *text = entry->text;

			waddstr(self->inner_w, "* ");
			if (is_focused && i == self->highlighted_idx) {
				if (!main_state->insert_mode_on) {
					/* Focused, insert mode off, highlighted. */
					wattron(self->inner_w, A_REVERSE);
					waddstr(self->inner_w, text);
					wattroff(self->inner_w, A_REVERSE);
				} else if (main_state->insert_mode_on && main_state->cursor_flash_show) {
					wattron(self->inner_w, A_REVERSE);
					waddstr(self->inner_w, text);
					waddstr(self->inner_w, " ");
					wattroff(self->inner_w, A_REVERSE);
				}
			} else {
				/* Non-focused */
				waddstr(self->inner_w, text);
			}

			wattroff(self->inner_w, A_REVERSE);
			wmove(self->inner_w, cursor_iter++, 0);
		}
	}
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


