// vim: noet ts=4 sw=4
#include "draw.h"
#include "state.h"

#define BLINK_TICK_COUNT 20

void init_state(struct app_state_t *state) {
	state->current_time = time(NULL);
	state->last_time_triggered_redraw = 0;
	state->last_update_time = 0;
	state->last_draw_time = 0;
	state->dirty = true;
}

void update_state(struct app_state_t *state) {
	/* Has the time turned over sufficiently to redraw? */
	state->current_time = time(NULL);
	if (!state->last_time_triggered_redraw ||
			state->current_time - state->last_time_triggered_redraw >= 1) {
		state->last_time_triggered_redraw = state->current_time;
		state->dirty = true;
	}

	/* Cursor flashing stuff: */
	if (state->insert_mode_on) {
		state->cursor_ticks_advanced -= 1;
		if (state->cursor_ticks_advanced <= 0) {
			state->cursor_ticks_advanced = BLINK_TICK_COUNT;
			state->cursor_flash_show = !state->cursor_flash_show;
			state->dirty = true;
		}
	}

	/* Update the state of every drawable_t (window) */
	unsigned int i = 0;
	for (i = 0; i < 3; i++) {
		struct drawable_t *i_drawable = &state->windows[i];
		bool is_focused = (int)i == state->current_window_idx ? true : false;
		i_drawable->update_state_func(i_drawable, state, is_focused);
	}
}

void exit_insert_mode(struct app_state_t *state,
		struct drawable_t *current_drawable) {
	(void)current_drawable;
	state->insert_mode_on = false;
	state->cursor_ticks_advanced = BLINK_TICK_COUNT;
	state->dirty = true;
}

void enter_insert_mode(struct app_state_t *state,
		struct drawable_t *current_drawable) {
	state->insert_mode_on = true;
	state->cursor_ticks_advanced = BLINK_TICK_COUNT;
	state->dirty = true;

	/* Are we creating a new entry, or editing an existing one? */
	if (current_drawable->entries) {
		if (current_drawable->highlighted_idx == (int64_t)current_drawable->entries->count) {
			/* New entry at bottom */
			struct entry_t new_entry = {
				.id = 0,
				.text = NULL,
				.text_len = 0
			};
			vector_append(current_drawable->entries, &new_entry, sizeof(new_entry));
		}
	} else {
		vector *entries = vector_new(sizeof(struct entry_t), 8);
		current_drawable->entries = entries;
		struct entry_t new_entry = {
			.id = 0,
			.text = NULL,
			.text_len = 0
		};
		vector_append(entries, &new_entry, sizeof(new_entry));
	}
}

void update_state_with_keypress_insert_mode(struct app_state_t *state,
		struct drawable_t *current_drawable,
		const vector *key_presses,
		const int ch,
		bool *escape_triggered,
		bool *square_thingie_triggered) {
	(void)escape_triggered;
	(void)square_thingie_triggered;
	(void)current_drawable;
	switch (ch) {
		case KEY_ESCAPE:
			*escape_triggered = true;
			if (key_presses->count <= 1)
				exit_insert_mode(state, current_drawable);
			break;
		default:
			/* Show the key if we pressed one. */
			state->dirty = true;
			break;
	}
}

void update_state_with_keypress_command_mode(struct app_state_t *state,
		struct drawable_t *current_drawable,
		const vector *key_presses,
		const int ch,
		bool *escape_triggered,
		bool *square_thingie_triggered) {
	switch (ch) {
		case KEY_ESCAPE:
			*escape_triggered = true;
			if (key_presses->count <= 1) {
				state->should_exit = true;
			}
			break;
		case '[':
			if (*escape_triggered) {
				*square_thingie_triggered = true;
			}
			break;
		case 'Z':
			if (*escape_triggered && *square_thingie_triggered) {
				state->current_window_idx -= 1;
				if (state->current_window_idx < 0)
					state->current_window_idx = 2;
				state->dirty = true;
			}
			break;
		case 'i':
			enter_insert_mode(state, current_drawable);
			break;
		case '\t':
		case 'l':
		case 67: /* Right */
			state->current_window_idx += 1;
			state->current_window_idx = state->current_window_idx % 3;
			state->dirty = true;
			break;
		case 'h':
		case 68: /* Left */
			state->current_window_idx -= 1;
			if (state->current_window_idx < 0)
				state->current_window_idx = 2;
			state->dirty = true;
			break;
		case 'k':
		case 65: /* Up Arrow */
			current_drawable->highlighted_idx -= 1;
			if (current_drawable->ensure_highlight_correct)
				current_drawable->ensure_highlight_correct(current_drawable);
			state->dirty = true;
			break;
		case 'j':
		case 66: /* Down Arrow */
			current_drawable->highlighted_idx += 1;
			if (current_drawable->ensure_highlight_correct)
				current_drawable->ensure_highlight_correct(current_drawable);
			state->dirty = true;
			break;
		default:
			/* Show the key if we pressed one. */
			state->dirty = true;
			break;
	}
}

void update_state_with_keypress(struct app_state_t *state, const vector *key_presses) {
	struct drawable_t *current_drawable = &state->windows[state->current_window_idx];
	unsigned int i = 0;
	memset(state->last_key_pressed, '\0', sizeof(state->last_key_pressed));

	int print_iter = 0;
	bool escape_triggered = false;
	bool square_thingie_triggered = false;

	for (i = 0; i < key_presses->count; i++) {
		const int *_ch = (int *)vector_get(key_presses, i);
		const int ch = *_ch;

		print_iter += snprintf(state->last_key_pressed + print_iter, sizeof(state->last_key_pressed) - print_iter, " %i,", ch);
		if (state->insert_mode_on) {
			update_state_with_keypress_insert_mode(state,
					current_drawable,
					key_presses,
					ch,
					&escape_triggered,
					&square_thingie_triggered);
		} else {
			update_state_with_keypress_command_mode(state,
					current_drawable,
					key_presses,
					ch,
					&escape_triggered,
					&square_thingie_triggered);
		}
	}
}

void cleanup_entries(struct drawable_t *self) {
	if (self->entries) {
		unsigned int i = 0;
		for (i = 0; i < self->entries->count; i++) {
			const struct entry_t *entry = vector_get(self->entries, i);
			free(entry->text);
		}
		vector_free(self->entries);
		self->entries = NULL;
	}
}

void ensure_left_highlight_correct(struct drawable_t *self) {
	if (self->entries && self->entries->count > 0) {
		if (self->highlighted_idx > (int64_t)self->entries->count ||
			self->highlighted_idx < 0) {
			self->highlighted_idx = (self->highlighted_idx % self->entries->count - 1) % self->entries->count + 1;
		}
	} else {
		if (self->highlighted_idx >= 1 ||
				self->highlighted_idx < 0) {
			self->highlighted_idx = 0;
		}
	}
}

void update_left(struct drawable_t *self, const struct app_state_t *main_state, const bool is_focused) {
	(void)is_focused;
	if (self->should_reload_entries) {
		cleanup_entries(self);

		vector *entries = vector_new(sizeof(struct entry_t), 8);
		sqlite3_stmt *statement = NULL;
		const char query[] = "SELECT id, parent_id, data "
							 "FROM entry "
							 "WHERE date(created_at_date, 'localtime') = date('now', 'localtime') "
							 "ORDER BY id;";
		const char *tail = NULL;
		sqlite3_prepare_v3(main_state->database,
				query,
				sizeof(query),
				0,
				&statement,
				&tail);

		while (sqlite3_step(statement) == SQLITE_ROW) {
			struct entry_t new_entry = {0};
			const char *text = (char *)sqlite3_column_text(statement, 2);
			new_entry.text_len = sqlite3_column_bytes(statement, 2);

			new_entry.text = calloc(new_entry.text_len, sizeof(char));
			strncpy(new_entry.text, text, new_entry.text_len);
			vector_append(entries, &new_entry, sizeof(struct entry_t));
		}

		sqlite3_finalize(statement);

		self->entries = entries;
		self->should_reload_entries = false;
	}
}

void update_middle(struct drawable_t *self, const struct app_state_t *main_state, const bool is_focused) {
	(void)self;
	(void)main_state;
	(void)is_focused;
}

void update_right(struct drawable_t *self, const struct app_state_t *main_state, const bool is_focused) {
	(void)self;
	(void)main_state;
	(void)is_focused;
}
