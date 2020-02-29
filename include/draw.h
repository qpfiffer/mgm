// vim: noet ts=4 sw=4
#pragma once
#include "app.h"
#include "vector.h"

struct app_state_t;

struct entry_t {
	uint64_t id;
	char *text;
	size_t text_len;
};

struct drawable_t {
	WINDOW *outer_w;
	WINDOW *inner_w;
	void (*drawable_func)(const struct drawable_t *self,
						  const struct app_state_t *main_state,
						  const bool is_focused);
	void (*update_state_func)(struct drawable_t *self,
							  const struct app_state_t *main_state,
							  const bool is_focused);

	/* Currently highlighted item */
	unsigned int highlighted_idx;

	/* Entries to display in this window */
	vector *entries;

	/* Whether or not we should reload entries */
	bool should_reload_entries;
};

void draw_left_items(const struct drawable_t *self, const struct app_state_t *main_state, const bool is_focused);
void draw_middle_items(const struct drawable_t *self, const struct app_state_t *main_state, const bool is_focused);
void draw_right_items(const struct drawable_t *self, const struct app_state_t *main_state, const bool is_focused);
