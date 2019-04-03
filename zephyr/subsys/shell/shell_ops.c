/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "shell_ops.h"

void shell_op_cursor_vert_move(const struct shell *shell, s32_t delta)
{
	if (delta != 0) {
		shell_raw_fprintf(shell->fprintf_ctx, "\033[%d%c",
				delta > 0 ? delta : -delta,
				delta > 0 ? 'A' : 'B');
	}
}

void shell_op_cursor_horiz_move(const struct shell *shell, s32_t delta)
{
	if (delta != 0) {
		shell_raw_fprintf(shell->fprintf_ctx, "\033[%d%c",
				delta > 0 ? delta : -delta,
				delta > 0 ? 'C' : 'D');
	}
}

/* Function returns true if command length is equal to multiplicity of terminal
 * width.
 */
static inline bool full_line_cmd(const struct shell *shell)
{
	return ((shell->ctx->cmd_buff_len + shell_strlen(shell->prompt))
			% shell->ctx->vt100_ctx.cons.terminal_wid == 0);
}

/* Function returns true if cursor is at beginning of an empty line. */
bool shell_cursor_in_empty_line(const struct shell *shell)
{
	return ((shell->ctx->cmd_buff_pos + shell_strlen(shell->prompt))
			% shell->ctx->vt100_ctx.cons.terminal_wid == 0);
}

void shell_op_cond_next_line(const struct shell *shell)
{
	if (shell_cursor_in_empty_line(shell) || full_line_cmd(shell)) {
		cursor_next_line_move(shell);
	}
}

void shell_op_cursor_position_synchronize(const struct shell *shell)
{
	struct shell_multiline_cons *cons = &shell->ctx->vt100_ctx.cons;
	bool last_line;

	shell_multiline_data_calc(cons, shell->ctx->cmd_buff_pos,
				  shell->ctx->cmd_buff_len);
	last_line = (cons->cur_y == cons->cur_y_end);

	/* In case cursor reaches the bottom line of a terminal, it will
	 * be moved to the next line.
	 */
	if (full_line_cmd(shell)) {
		cursor_next_line_move(shell);
	}

	if (last_line) {
		shell_op_cursor_horiz_move(shell, cons->cur_x -
							       cons->cur_x_end);
	} else {
		shell_op_cursor_vert_move(shell, cons->cur_y_end - cons->cur_y);
		shell_op_cursor_horiz_move(shell, cons->cur_x -
							       cons->cur_x_end);
	}
}

void shell_op_cursor_move(const struct shell *shell, s16_t val)
{
	struct shell_multiline_cons *cons = &shell->ctx->vt100_ctx.cons;
	u16_t new_pos = shell->ctx->cmd_buff_pos + val;
	s32_t row_span;
	s32_t col_span;

	shell_multiline_data_calc(cons, shell->ctx->cmd_buff_pos,
				  shell->ctx->cmd_buff_len);

	/* Calculate the new cursor. */
	row_span = row_span_with_buffer_offsets_get(&shell->ctx->vt100_ctx.cons,
						    shell->ctx->cmd_buff_pos,
						    new_pos);
	col_span = column_span_with_buffer_offsets_get(
						    &shell->ctx->vt100_ctx.cons,
						    shell->ctx->cmd_buff_pos,
						    new_pos);

	shell_op_cursor_vert_move(shell, -row_span);
	shell_op_cursor_horiz_move(shell, col_span);
	shell->ctx->cmd_buff_pos = new_pos;
}

void shell_op_word_remove(const struct shell *shell)
{
	char *str = &shell->ctx->cmd_buff[shell->ctx->cmd_buff_pos - 1];
	char *str_start = &shell->ctx->cmd_buff[0];
	u16_t chars_to_delete;

	/* Line must not be empty and cursor must not be at 0 to continue. */
	if ((shell->ctx->cmd_buff_len == 0) ||
	    (shell->ctx->cmd_buff_pos == 0)) {
		return;
	}

	/* Start at the current position. */
	chars_to_delete = 0;

	/* Look back for all spaces then for non-spaces. */
	while ((str >= str_start) && (*str == ' ')) {
		++chars_to_delete;
		--str;
	}

	while ((str >= str_start) && (*str != ' ')) {
		++chars_to_delete;
		--str;
	}

	/* Manage the buffer. */
	memmove(str + 1, str + 1 + chars_to_delete,
		shell->ctx->cmd_buff_len - chars_to_delete);
	shell->ctx->cmd_buff_len -= chars_to_delete;
	shell->ctx->cmd_buff[shell->ctx->cmd_buff_len] = '\0';

	/* Update display. */
	shell_op_cursor_move(shell, -chars_to_delete);
	cursor_save(shell);
	shell_fprintf(shell, SHELL_NORMAL, "%s", str + 1);
	clear_eos(shell);
	cursor_restore(shell);
}

void shell_op_cursor_home_move(const struct shell *shell)
{
	shell_op_cursor_move(shell, -shell->ctx->cmd_buff_pos);
}

void shell_op_cursor_end_move(const struct shell *shell)
{
	shell_op_cursor_move(shell, shell->ctx->cmd_buff_len -
						shell->ctx->cmd_buff_pos);
}


void shell_op_left_arrow(const struct shell *shell)
{
	if (shell->ctx->cmd_buff_pos > 0) {
		shell_op_cursor_move(shell, -1);
	}
}

void shell_op_right_arrow(const struct shell *shell)
{
	if (shell->ctx->cmd_buff_pos < shell->ctx->cmd_buff_len) {
		shell_op_cursor_move(shell, 1);
	}
}

static void reprint_from_cursor(const struct shell *shell, u16_t diff,
				bool data_removed)
{
	/* Clear eos is needed only when newly printed command is shorter than
	 * previously printed command. This can happen when delete or backspace
	 * was called.
	 *
	 * Such condition is useful for Bluetooth devices to save number of
	 * bytes transmitted between terminal and device.
	 */
	if (data_removed) {
		clear_eos(shell);
	}

	shell_fprintf(shell, SHELL_NORMAL, "%s",
			&shell->ctx->cmd_buff[shell->ctx->cmd_buff_pos]);
	shell->ctx->cmd_buff_pos = shell->ctx->cmd_buff_len;

	if (full_line_cmd(shell)) {
		if (((data_removed) && (diff > 0)) || (!data_removed)) {
			cursor_next_line_move(shell);
		}
	}

	shell_op_cursor_move(shell, -diff);
}

static void data_insert(const struct shell *shell, const char *data, u16_t len)
{
	u16_t after = shell->ctx->cmd_buff_len - shell->ctx->cmd_buff_pos;
	char *curr_pos = &shell->ctx->cmd_buff[shell->ctx->cmd_buff_pos];

	if ((shell->ctx->cmd_buff_len + len) >= CONFIG_SHELL_CMD_BUFF_SIZE) {
		return;
	}

	memmove(curr_pos + len, curr_pos, after);
	memcpy(curr_pos, data, len);
	shell->ctx->cmd_buff_len += len;
	shell->ctx->cmd_buff[shell->ctx->cmd_buff_len] = '\0';

	if (!flag_echo_is_set(shell)) {
		shell->ctx->cmd_buff_pos += len;
		return;
	}

	reprint_from_cursor(shell, after, false);
}

void char_replace(const struct shell *shell, char data)
{
	shell->ctx->cmd_buff[shell->ctx->cmd_buff_pos++] = data;
	shell_raw_fprintf(shell->fprintf_ctx, "%c", data);
	if (shell_cursor_in_empty_line(shell)) {
		cursor_next_line_move(shell);
	}
}

void shell_op_char_insert(const struct shell *shell, char data)
{
	if (shell->ctx->internal.flags.insert_mode &&
		(shell->ctx->cmd_buff_len != shell->ctx->cmd_buff_pos)) {
		char_replace(shell, data);
	} else {
		data_insert(shell, &data, 1);
	}
}

void shell_op_char_backspace(const struct shell *shell)
{
	if ((shell->ctx->cmd_buff_len == 0) ||
	    (shell->ctx->cmd_buff_pos == 0)) {
		return;
	}

	shell_op_cursor_move(shell, -1);
	shell_op_char_delete(shell);
}

void shell_op_char_delete(const struct shell *shell)
{
	u16_t diff = shell->ctx->cmd_buff_len - shell->ctx->cmd_buff_pos;
	char *str = &shell->ctx->cmd_buff[shell->ctx->cmd_buff_pos];

	if (diff == 0) {
		return;
	}

	memmove(str, str + 1, diff);
	--shell->ctx->cmd_buff_len;
	reprint_from_cursor(shell, --diff, true);
}

void shell_op_completion_insert(const struct shell *shell,
				const char *compl,
				u16_t compl_len)
{
	data_insert(shell, compl, compl_len);
}
