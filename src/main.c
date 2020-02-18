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
	main_state->left_w = newwin(max_row, window_col, 0, 0);
	box(main_state->left_w, 0, 0);
	wrefresh(main_state->left_w);

	main_state->middle_w = newwin(max_row, window_col, 0, window_col);
	box(main_state->middle_w, 0, 0);
	wrefresh(main_state->middle_w);

	main_state->right_w = newwin(max_row, window_col, 0, window_col * 2);
	box(main_state->right_w, 0, 0);
	wrefresh(main_state->right_w);
}

void cleanup() {
	endwin();
}

void update(struct app_state_t *main_state) {
	int ch = getchar();

	if (ch == KEY_ESCAPE) {
		main_state->should_exit = true;
	}
}

void draw(const struct app_state_t *main_state) {
	//refresh();
	wrefresh(main_state->left_w);
	wrefresh(main_state->middle_w);
	wrefresh(main_state->right_w);
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
