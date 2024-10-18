// File dealing with the panel structure
// It handles reading and scrolling the contents of the panel
// Writing to cell buffers is handled by draw.c
// Panels also handle the stage, home screen, help screen, etc.

#include "shooting.h"

void	free_double_char_arr(char **arr, char *last) {
	char	*current = arr[0];
	char	*target;
	int		i = 0;

	while (current != last) {
		target = current;
		current = arr[++i];
		free(target);
	}
	free(last);
	free(arr);
}

static int	init_panel(t_panel *panel) {
	DIR	*dir;

	panel->buf = (char **)calloc(SCREEN_H, sizeof(char *));
	if (panel->buf == NULL) {
		perror("Failed to allocate memory on panel buf");
		return 1;
	}
	for (int i=0; i<SCREEN_H; ++i) {
		panel->buf[i] = (char *)calloc(SCREEN_W + 2, sizeof(char));
		if (panel->buf[i] == NULL) {
			perror("Failed to allocate memory on panel buf");
			free_double_char_arr(panel->buf, panel->buf[i - 1]);
			return 1;
		}
	}
	dir = panel->dir;
	if (panel->difficulty == 0) {
		strcpy(panel->difficulty_name, "easy");
		dir = opendir("./stages/easy");
	}
	else if (panel->difficulty == 1) {
		strcpy(panel->difficulty_name, "normal");
		dir = opendir("./stages/normal");
	}
	else if (panel->difficulty == 2) {
		strcpy(panel->difficulty_name, "hard");
		dir = opendir("./stages/hard");
	}
	else if (panel->difficulty == 3) {
		strcpy(panel->difficulty_name, "lunatic");
		dir = opendir("./stages/lunatic");
	}
	else {
		dir = NULL;
	}
	if (dir == NULL) {
		perror("Could not open directory");
		free(panel->buf);
		return 1;
	}
	panel->file = NULL;
	panel->entry = NULL;
	panel->lines_read = 0;
	panel->apply_lines = 0;
	panel->dir = dir;
	return 0;
}

static int	read_panel_sustain(t_panel *panel) {
	char	buffer[SCREEN_W + 2];

	// If the first file is not opened, open the next file
	if (panel->file == NULL) {
		struct dirent	*tmpen;
		tmpen = readdir(panel->dir);
		while (tmpen->d_name[0] == '.' && (tmpen->d_name[1] == '\0' || tmpen->d_name[1] == '.')) {
			tmpen = readdir(panel->dir);
		}
		if (tmpen == NULL) {
			// The file is no longer there
			return 1;
		} else {
			panel->entry = tmpen;
			char	filepath[256];
			snprintf(filepath, sizeof(filepath) + 30, "./stages/%s/%s", panel->difficulty_name, panel->entry->d_name);
			panel->file = fopen(filepath, "r");
			if (panel->file == NULL) {
				perror("Could not open file");
			}
		}
		// Reset the number of lines read as a new file is opened
		panel->lines_read = 0;
	}

	// The first call processes SCREEN_H rows
	while (panel->lines_read < SCREEN_H) {
		if (fgets(buffer, sizeof(buffer), panel->file) != NULL) {
			strncpy(panel->buf[panel->lines_read], buffer, SCREEN_W + 2);
			++panel->lines_read;
		} else {
			fclose(panel->file);
			panel->file = NULL;
			return 1;
		}
		if (panel->lines_read == SCREEN_H) {
			return 0;
		}
	}
	if (panel->lines_read >= SCREEN_H) {
		// Shift existing buffer before reading a new line
		for (int i=1; i<SCREEN_H; ++i) {
			strcpy(panel->buf[i - 1], panel->buf[i]);
		}
		if (fgets(buffer, sizeof(buffer), panel->file) != NULL) {
			strncpy(panel->buf[SCREEN_H - 1], buffer, SCREEN_W + 2);
			++panel->lines_read;
		} else {
			fclose(panel->file);
			panel->file = NULL;
			return 1;
		}
	}
	return 0;
}

static void	panel_to_object(t_screen *screen) {
	int	lines;

	lines = screen->panels->apply_lines;
	// The first call processes SCREEN_H rows
	while (lines < SCREEN_H) {
		for (int i=0; i<SCREEN_W; ++i) {
			char_to_obj(screen, screen->panels->buf[lines][i], SCREEN_H - lines - 1, i);
		}
		++screen->panels->apply_lines;
		lines = screen->panels->apply_lines;
		if (lines == SCREEN_H) {
			return;
		}
	}

	for (int i=0; i<SCREEN_W; ++i) {
		char_to_obj(screen, screen->panels->buf[SCREEN_H - 1][i], 0, i);
	}
	++screen->panels->apply_lines;
	lines = screen->panels->apply_lines;
	return;
}

void	*read_panel(void *arr) {
	t_screen	*screen;
	int			tic = FPS / 2;

	screen = (t_screen *)arr;
	if (init_panel(screen->panels)) {
		pthread_mutex_lock(&g_mutex);
		screen->is_game_active = 0;
		pthread_mutex_unlock(&g_mutex);
		return NULL;
	}
	#ifdef DEBUG
		pthread_setname_np(pthread_self(), "read_panel");
	#endif
	while (1) {
		pthread_mutex_lock(&g_mutex);
		if (screen->is_game_active == 0) {
			pthread_mutex_unlock(&g_mutex);
			return NULL;
		}
		if (++tic >= FPS / 2) {
			if (read_panel_sustain(screen->panels) == 1) {
				free_double_char_arr(screen->panels->buf, screen->panels->buf[SCREEN_H - 1]);
				pthread_mutex_unlock(&g_mutex);
				return NULL;
			}
			panel_to_object(screen);
			tic = 0;
		}
		pthread_mutex_unlock(&g_mutex);

		sleep_for_milliseconds(TIME_PER_FRAME);
	}
	return NULL;
}