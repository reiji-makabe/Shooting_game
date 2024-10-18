// A file dealing with functions related to drawing, 
// such as changing cells and buffers and console mode.
#include "shooting.h"

static void rewrite_buf_with_cell(t_screen *screen) {
	int	n = 0;
	char	tmp_obj_buf[6] = {'\0'};
	char	tmp_back_buf[6] = {'\0'};

	for (int i=0; i<SCREEN_H; ++i) {
		for (int j=0; j<SCREEN_W; ++j) {
			// If the color is the same as the previous color, skip writing to the buffer
			if (memcmp(tmp_obj_buf, screen->cells[i][j].obj_color, SIZE_COLOR)) {
				// copy to buffer
				memcpy(&screen->buffer[n], screen->cells[i][j].obj_color, SIZE_COLOR);
				// copy to tmp
				memcpy(tmp_obj_buf, screen->cells[i][j].obj_color, SIZE_COLOR);
				n += SIZE_COLOR;
			}
			// If the color is the same as the previous color, skip writing to the buffer
			if (!memcmp(tmp_back_buf, screen->cells[i][j].back_color, SIZE_COLOR)) {
				// copy to buffer
				memcpy(&screen->buffer[n], screen->cells[i][j].back_color, SIZE_COLOR);
				// copy to tmp
				memcpy(tmp_back_buf, screen->cells[i][j].back_color, SIZE_COLOR);
				n += SIZE_COLOR;
			}
			screen->buffer[n] = screen->cells[i][j].object;
			n++;
		}
		screen->buffer[n] = '\n';
		n++;
	}
	screen->buffer[n] = '\0';
	screen->buffer[(SCREEN_H * (SCREEN_W + 1)) * (SIZE_COLOR * 2 + 1)] = '\0';
}

static void	draw_object_to_cell(t_screen *screen) {
	int	y;
	int	x;
	for (int i=0; i<screen->now_size_b; ++i) {
		// write screen->bullet.c to cell
		y = (int)(screen->bullet[i].y + 0.5);
		x = (int)(screen->bullet[i].x + 0.5);
		if (y < SCREEN_H - 1 && y > -1 && x < SCREEN_W && x > -1) {
			screen->cells[y][x].object = screen->bullet[i].c;
			memcpy(screen->cells[y][x].obj_color, screen->bullet[i].color, SIZE_COLOR);
		}
	}
	for (int i=0; i<screen->now_size_e; ++i) {
		y = (int)(screen->enemy[i].y + 0.5);
		x = (int)(screen->enemy[i].x + 0.5);
		if (y < SCREEN_H - 1 && y > -1 && x < SCREEN_W && x > -1) {
			screen->cells[y][x].object = screen->enemy[i].c;
			memcpy(screen->cells[y][x].obj_color, screen->enemy[i].color, SIZE_COLOR);
		}
	}
	y = (int)(screen->player.y + 0.5);
	x = (int)(screen->player.x + 0.5);
	screen->cells[y][x].object = screen->player.c;
	memcpy(screen->cells[y][x].obj_color, screen->player.color, SIZE_COLOR);
}

void	*draw_screen(void *arg) {
	t_screen *screen = (t_screen *)arg;

	#ifdef DEBUG
		pthread_setname_np(pthread_self(), "draw_screen");
	#endif
	while (1) {
		pthread_mutex_lock(&g_mutex);
		if (screen->is_game_active == 0) {
				pthread_mutex_unlock(&g_mutex);
			return NULL;
		}
		initialized_cell(screen);
		draw_object_to_cell(screen);
		rewrite_buf_with_cell(screen);
		// Move the cursor position to the upper left
		printf("\033[Hscore:%d\n%sHP:%d   \n", screen->score, screen->buffer, screen->player.hp);
		pthread_mutex_unlock(&g_mutex);

		// wait fps
		sleep_for_milliseconds(TIME_PER_FRAME);
	}
	return NULL;
}

void	initialized_cell(t_screen *screen) {
	for (int i=0; i<SCREEN_H; ++i) {
		for (int j=0; j<SCREEN_W; ++j) {
			memcpy(screen->cells[i][j].obj_color , WHITE, SIZE_COLOR);
			memcpy(screen->cells[i][j].back_color , WHITE, SIZE_COLOR);
			screen->cells[i][j].object = ' ';
		}
	}
}

#ifdef WIN
void enable_virtual_terminal_processing(void) {
	// Get stdout handle
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Error: Invalid handle.\n");
		return;
	}

	// Get now console mode
	DWORD dwOriginalMode;
	if (!GetConsoleMode(hOut, &dwOriginalMode)) {
		fprintf(stderr, "Error: Unable to get console mode.\n");
		return;
	}

	// enable VIRTUAL_TERMINAL_PROCESSING
	DWORD dwMode = dwOriginalMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode)) {
		fprintf(stderr, "Error: Unable to set console mode.\n");
		return;
	}

	// Function to restore console mode at exit
	void restore_console_mode(void) {
		SetConsoleMode(hOut, dwOriginalMode);
	}

	// At exit, return conlose previous mode
	atexit(restore_console_mode);
}
#endif