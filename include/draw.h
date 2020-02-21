// vim: noet ts=4 sw=4
#include "app.h"

struct app_state_t;

struct drawable_t {
	WINDOW *outer_w;
	WINDOW *inner_w;
	void (*drawable_func)(const struct drawable_t *self,
						  const struct app_state_t *main_state,
						  const bool is_focused);

	unsigned int highlighted_idx;
};

void draw_left_items(const struct drawable_t *self, const struct app_state_t *main_state, const bool is_focused);
void draw_middle_items(const struct drawable_t *self, const struct app_state_t *main_state, const bool is_focused);
void draw_right_items(const struct drawable_t *self, const struct app_state_t *main_state, const bool is_focused);
