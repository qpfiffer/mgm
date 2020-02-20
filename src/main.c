// vim: noet ts=4 sw=4
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

#include "state.h"

#define KEY_ESCAPE 27

#define PADDING_X 1
#define PADDING_Y 1

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
}

void cleanup() {
	endwin();
}

void update(struct app_state_t *main_state) {
	main_state->update_dt = time(NULL) - main_state->last_update_time;
	main_state->update_dtotal += main_state->update_dt;
	if (main_state->update_dtotal > TICKER_RATE) {
		main_state->update_dtotal -= TICKER_RATE;

		update_state(main_state);

		int ch = getch();
		if (ch == ERR)
			return;

		if (ch == KEY_ESCAPE) {
			main_state->should_exit = true;
		} else if (ch == '\t') {
			main_state->current_window_idx += 1;
			main_state->current_window_idx = main_state->current_window_idx % 3;
			main_state->dirty = true;
		}
	}
}

void draw_left_items(const struct drawable_t *self, const struct app_state_t *main_state) {
	(void)main_state;
	(void)self;

	char buf[128] = {0};
	struct tm *info = NULL;
	info = localtime(&main_state->current_time);
	strftime(buf, sizeof(buf), "%y/%m/%d (%H:%M:%S):", info);

	waddstr(self->inner_w, buf);
}

void draw_middle_items(const struct drawable_t *self, const struct app_state_t *main_state) {
	(void)main_state;
	(void)self;
}

void draw_right_items(const struct drawable_t *self, const struct app_state_t *main_state) {
	(void)main_state;
	(void)self;
}

void draw(struct app_state_t *main_state) {
	struct drawable_t windows[] = {
		{main_state->left_w_outer, main_state->left_w_i, &draw_left_items},
		{main_state->middle_w_outer, main_state->middle_w_i, &draw_middle_items},
		{main_state->right_w_outer, main_state->right_w_i, &draw_right_items},
	};

	main_state->draw_dt = time(NULL) - main_state->last_draw_time;
	main_state->draw_dtotal += main_state->draw_dt;
	if (main_state->draw_dtotal > DRAW_RATE) {
		main_state->draw_dtotal -= DRAW_RATE;

		if (main_state->dirty == false)
			return;

		unsigned int i = 0;
		for (i = 0; i < 3; i++) {
			WINDOW *outer_w = windows[i].outer_w;
			WINDOW *inner_w = windows[i].inner_w;

			wmove(outer_w, 0, 0);
			wmove(inner_w, 0, 0);

			if (main_state->current_window_idx == i) {
				box(outer_w, '*', '*');
			} else {
				box(outer_w, 0, 0);
			}

			windows[i].drawable_func(&windows[i], main_state);
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
