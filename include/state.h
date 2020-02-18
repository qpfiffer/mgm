// vim: noet ts=4 sw=4
#include <stdbool.h>

#include <ncurses.h>

struct app_state_t {
    bool should_exit;

    WINDOW *left_w, *middle_w, *right_w;
};
