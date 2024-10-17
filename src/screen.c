// Deals with the creation and deletion of screen structures
#include "shooting.h"

// This function frees all the elements of the cells array and also frees the cells themselves
// If NULL is entered as the second argument, only the cells are free
void free_t_cells(t_cell **cells, t_cell *last) {
    t_cell	*current = cells[0];
	t_cell	*target;
	int		i = 0;

    while (current != last) {
		target = current;
		current = cells[++i];
        free(target);
	}
	free(last);
    free(cells);
}

void	free_t_screen(t_screen *screen) {
	free_t_cells(screen->cells, screen->cells[SCREEN_H - 1]);
	free(screen->buffer);
	free_object(screen);
	free(screen->panels);
	free(screen);
}

t_screen	*screen_init(void) {
	t_screen	*re;

	// create t_screen
	re = (t_screen *)calloc(1, sizeof(t_screen));
	if (re == NULL) {
		perror("Failed to allocate memory on screen_init");
		return NULL;
	}

	// create screen_cell
	re->cells = (t_cell **)calloc(SCREEN_H, sizeof(t_cell *));
	if (re->cells == NULL) {
		perror("Failed to allocate memory on screen_init");
		free(re);
		re = NULL;
		return NULL;
	}
	for (int i=0; i<SCREEN_H; ++i) {
		re->cells[i] = (t_cell *)calloc(SCREEN_W, sizeof(t_cell));
		if (re->cells[i] == NULL) {
			perror("Failed to allocate memory on screen_init");
			free_t_cells(re->cells, re->cells[i - 1]);
			free(re);
			return NULL;
		}
	}

	// create screen buffer
	re->buffer = (char *)calloc((SCREEN_H * (SCREEN_W + 1)) * (SIZE_COLOR * 2 + 1) + 1, 1);
	if (re->buffer == NULL) {
		perror("Failed to allocate memory on screen_init");
		free_t_cells(re->cells, re->cells[SCREEN_H - 1]);
		free(re);
		return NULL;
	}

	re->panels = (t_panel *)calloc(1, sizeof(t_panel));
	if (re->panels == NULL) {
		perror("Failed to allocate memory on screen_init");
		free_t_cells(re->cells, re->cells[SCREEN_H - 1]);
		free(re->buffer);
		free(re);
		return NULL;
	}

	initialized_cell(re);
	re->now_size_b = 0;
	re->now_size_e = 0;
	re->size_bullet = 0;
	re->size_enemy = 0;
	re->bullet = NULL;
	re->enemy = NULL;
	re->player.c = '@';
	memcpy(re->player.color, WHITE, SIZE_COLOR);
	re->player.y = SCREEN_H - 1;
	re->player.x = (SCREEN_W - 1) / 2;
	re->player.hp = 10;
//	re->bomb_remain = 3;
	re->is_game_active = 1;
	re->score = 0;
	return re;
}