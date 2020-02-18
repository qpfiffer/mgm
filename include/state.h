// vim: noet ts=4 sw=4
#include <stdbool.h>

#include <ncurses.h>

struct app_state_t {
    bool should_exit;

    WINDOW *left_w_outer, *middle_w_outer, *right_w_outer;
    WINDOW *left_w_i, *middle_w_i, *right_w_i;
};
