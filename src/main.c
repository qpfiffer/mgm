// vim: noet ts=4 sw=4
#include "app.h"
#include "draw.h"
#include "state.h"
#include "vector.h"

#define PADDING_X 5
#define PADDING_Y 5

#define TICKER_RATE 1/60
#define DRAW_RATE 1/60

void init(struct app_state_t *main_state) {
	initscr();
	noecho();
	nodelay(stdscr, true);
	cbreak();
	curs_set(FALSE);

	int max_row, max_col;
	getmaxyx(stdscr, max_row, max_col);

	const int window_col = max_col / 3;
	/* LEFT */
	main_state->left_w_outer = newwin(max_row, window_col, 0, 0);
	box(main_state->left_w_outer, 0, 0);
	wrefresh(main_state->left_w_outer);

	main_state->left_w_i = newwin(max_row - PADDING_X, window_col - PADDING_Y, 1, 1);
	wrefresh(main_state->left_w_i);
	/* ----- */

	/* MIDDLE */
	main_state->middle_w_outer = newwin(max_row, window_col, 0, window_col);
	box(main_state->middle_w_outer, 0, 0);
	wrefresh(main_state->middle_w_outer);

	main_state->middle_w_i = newwin(max_row - PADDING_X, window_col - PADDING_Y, 1, window_col + 2);
	wrefresh(main_state->middle_w_i);
	/* ----- */

	/* RIGHT */
	main_state->right_w_outer = newwin(max_row, window_col, 0, window_col * 2);
	box(main_state->right_w_outer, 0, 0);
	wrefresh(main_state->right_w_outer);

	main_state->right_w_i = newwin(max_row - PADDING_X, window_col - PADDING_Y, 1, window_col * 2 + 2);
	wrefresh(main_state->right_w_i);
	/* ----- */

	struct drawable_t windows[] = {
		{main_state->left_w_outer, main_state->left_w_i, &draw_left_items, 0},
		{main_state->middle_w_outer, main_state->middle_w_i, &draw_middle_items, 0},
		{main_state->right_w_outer, main_state->right_w_i, &draw_right_items, 0},
	};
	main_state->windows = malloc(sizeof(windows));
	memset(main_state->windows, '\0', sizeof(windows));
	memcpy(main_state->windows, &windows, sizeof(windows));
}

void cleanup() {
	endwin();
}

void update(struct app_state_t *main_state) {
	if (main_state->last_update_time != 0) {
		main_state->update_dt = time(NULL) - main_state->last_update_time;
		main_state->update_dtotal += main_state->update_dt;
	} else {
		main_state->last_update_time = time(NULL);
		main_state->update_dt = 0;
		main_state->update_dtotal += TICKER_RATE;
	}

	if (main_state->update_dtotal >= TICKER_RATE) {
		main_state->update_dtotal -= TICKER_RATE;

		update_state(main_state);

		int ch = getch();
		if (ch == ERR)
			return;

		vector *key_presses = vector_new(sizeof(int), 8);
		vector_append(key_presses, &ch, sizeof(int));

		while ((ch = getch()) != ERR) {
			vector_append(key_presses, &ch, sizeof(int));
		}

		update_state_with_keypress(main_state, key_presses);
	}
}

void draw(struct app_state_t *main_state) {
	main_state->draw_dt = time(NULL) - main_state->last_draw_time;
	main_state->draw_dtotal += main_state->draw_dt;
	if (!main_state->last_draw_time || main_state->draw_dtotal >= DRAW_RATE) {
		main_state->draw_dtotal -= DRAW_RATE;

		if (main_state->dirty == false)
			return;

		int i = 0;
		for (i = 0; i < 3; i++) {
			const struct drawable_t *current_drawable = &main_state->windows[i];
			WINDOW *outer_w = current_drawable->outer_w;
			WINDOW *inner_w = current_drawable->inner_w;

			wmove(outer_w, 0, 0);
			wmove(inner_w, 0, 0);

			if (main_state->current_window_idx == i) {
				current_drawable->drawable_func(current_drawable, main_state, true);
				box(outer_w, '*', '*');
			} else {
				current_drawable->drawable_func(current_drawable, main_state, false);
				box(outer_w, 0, 0);
			}

			wrefresh(outer_w);
			wrefresh(inner_w);
		}

		main_state->dirty = false;
	}
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	struct app_state_t main_state = {0};

	init_state(&main_state);
	init(&main_state);

	while (!main_state.should_exit) {
		update(&main_state);
		draw(&main_state);
	}

	cleanup();

	return 0;
}
