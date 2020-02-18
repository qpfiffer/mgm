// vim: noet ts=4 sw=4
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

#include "state.h"

void init() {
	initscr();
	noecho();
	curs_set(FALSE);
}

void cleanup() {
	endwin();
}

void update(struct app_state_t *main_state) {
	int ch = 0;
	ch = getch();

	const char str[] = "Last key pressed: %i";
	char *buf = malloc(128);
	snprintf(buf, 128, str, ch);
	mvprintw(0, 0, buf);

	if (ch == KEY_EXIT) {
		main_state->should_exit = true;
	}
	free(buf);
}

void draw() {
	refresh();
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	struct app_state_t main_state = {0};

	init();

	while (!main_state.should_exit) {
		update(&main_state);
		draw();
	}

	cleanup();

	return 0;
}
