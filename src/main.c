// vim: noet ts=4 sw=4
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

#include "state.h"

#define KEY_ESCAPE 27

void init(struct app_state_t *main_state) {
	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);

	int max_row, max_col;
	getmaxyx(stdscr, max_row, max_col);

	const int window_col = max_col / 3;
	/* LEFT */
	main_state->left_w_outer = newwin(max_row, window_col, 0, 0);
	box(main_state->left_w_outer, 0, 0);
	wrefresh(main_state->left_w_outer);

	main_state->left_w_i = newwin(max_row - 2, window_col - 2, 1, 1);
	wrefresh(main_state->left_w_i);
	/* ----- */

	/* MIDDLE */
	main_state->middle_w_outer = newwin(max_row, window_col, 0, window_col);
	box(main_state->middle_w_outer, 0, 0);
	wrefresh(main_state->middle_w_outer);

	main_state->middle_w_i = newwin(max_row - 2, window_col - 2, 1, window_col + 1);
	wrefresh(main_state->middle_w_i);
	/* ----- */

	/* RIGHT */
	main_state->right_w_outer = newwin(max_row, window_col, 0, window_col * 2);
	box(main_state->right_w_outer, 0, 0);
	wrefresh(main_state->right_w_outer);

	main_state->right_w_i = newwin(max_row - 2, window_col - 2, 1, window_col * 2 + 1);
	wrefresh(main_state->right_w_i);
	/* ----- */
}

void cleanup() {
	endwin();
}

void update(struct app_state_t *main_state) {
	int ch = getchar();

	if (ch == KEY_ESCAPE) {
		main_state->should_exit = true;
	} else if (ch == '\t') {
		main_state->current_window_idx += 1;
		main_state->current_window_idx = main_state->current_window_idx % 3;
	}
}

void draw(const struct app_state_t *main_state) {
	//refresh();
	WINDOW *windows[][2] = {
		{main_state->left_w_outer, main_state->left_w_i},
		{main_state->middle_w_outer, main_state->middle_w_i},
		{main_state->right_w_outer, main_state->right_w_i},
	};
	unsigned int i = 0;
	for (i = 0; i < 3; i++) {
		WINDOW *outer_w = windows[i][0];
		WINDOW *inner_w = windows[i][1];

		wmove(outer_w, 0, 0);
		wmove(inner_w, 0, 0);

		if (main_state->current_window_idx == i) {
			box(outer_w, '*', '*');

			wrefresh(inner_w);
			wrefresh(outer_w);
		} else {
			box(outer_w, 0, 0);
			wrefresh(outer_w);
			wrefresh(inner_w);
		}
	}
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	struct app_state_t main_state = {0};

	init(&main_state);
	draw(&main_state);

	while (!main_state.should_exit) {
		update(&main_state);
		draw(&main_state);
	}

	cleanup();

	return 0;
}
