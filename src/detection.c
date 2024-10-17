// File that implements hit detection of bullets, enemies, and own ship
#include "shooting.h"

static int	is_hit(t_object *one, t_object *two) {
	if ((int)(one->x + 0.5) == (int)(two->x + 0.5) &&
		(int)(one->y + 0.5) == (int)(two->y + 0.5)) {
			return 1;
		}
	return 0;
}

static void	bullet_out_of_bounds(t_screen *screen, int index) {
	// If a bullet reachs the left or right end it disappears
	// but if it is blue, reflect
	if ((int)(screen->bullet[index].x + 0.5) < 0 || (int)(screen->bullet[index].x + 0.5) > SCREEN_W - 1) {
		if (memcmp(screen->bullet[index].color, BLUE, SIZE_COLOR)) {
			delete_bullet(screen, index);
		} else {
			screen->bullet[index].x_speed *= -1;
		}
	}
	// If a bullet reachs the top or below end, it disappears
	// but if it is green, it reflect once
	if ((int)(screen->bullet[index].y + 0.5) < 0) {
		if (memcmp(screen->bullet[index].color, GREEN, SIZE_COLOR)) {
			delete_bullet(screen, index);
		} else {
			screen->bullet[index].hp -= 1;
			if (screen->bullet[index].hp <= 0) {
				delete_bullet(screen, index);
			} else {
				screen->bullet[index].y_speed *= -1;
			}
		}
	}
	if ((int)(screen->bullet[index].y + 0.5) > SCREEN_H - 1) {
		if (memcmp(screen->bullet[index].color, GREEN, SIZE_COLOR)) {
			delete_bullet(screen, index);
		} else {
			screen->bullet[index].hp -= 1;
			if (screen->bullet[index].hp <= 0) {
				delete_bullet(screen, index);
			} else {
				screen->bullet[index].y_speed *= -1;
			}
		}
	}
}

static void	enemy_out_of_bounds(t_screen *screen, int index) {
	if (screen->enemy[index].c == 'O' || screen->enemy[index].c == 'o' || screen->enemy[index].c == '@') {
		// If a enemy reachs the left or right end, flip it
		if ((int)(screen->enemy[index].x + 0.5) < 1 || (int)(screen->enemy[index].x + 0.5) > SCREEN_W - 1) {
			screen->enemy[index].x_speed *= -1;
		}
		// If a enemy reachs the top or below end, it disapperas
		if ((int)(screen->enemy[index].y + 0.5) < 0 || (int)(screen->enemy[index].y + 0.5) > SCREEN_H - 1) {
			delete_enemy(screen, index);
		}
	} else {
		if ((int)(screen->enemy[index].x + 0.5) < 1 || (int)(screen->enemy[index].x + 0.5) > SCREEN_W - 1) {
			delete_enemy(screen, index);
		}
		if ((int)(screen->enemy[index].y + 0.5) < 0 || (int)(screen->enemy[index].y + 0.5) > SCREEN_H - 1) {
			delete_enemy(screen, index);
		}
	}
}

static void	bullet_hits_enemy(t_screen *screen, int index) {
	t_object *bullet;

	bullet = &screen->bullet[index];
	if (bullet->c != '*') {
		return;
	}
	for (int i=0; i<screen->now_size_e; ++i) {
		if (is_hit(bullet, &screen->enemy[i])) {
				screen->enemy[i].hp -= 1;
				if (screen->enemy[i].hp <= 0) {
					if (screen->enemy[i].c == '@') {
						screen->is_game_active = 0;
					}
					delete_enemy(screen, i);
					screen->score += 10;
				}
				bullet->hp -= 1;
				if (bullet->hp <= 0) {
					delete_bullet(screen, index);
				}
		}
	}
}

static void	enemy_hits_player(t_screen *screen, int index) {
	t_object	*enemy;

	enemy = &screen->enemy[index];
	if (!(is_hit(enemy, &screen->player))) {
			return;
		}
	screen->player.hp -= 1;
	// Reinforcement is removed when damaged
	memcpy(screen->player.color, WHITE, SIZE_COLOR);
	if (screen->player.hp <= 0) {
		screen->is_game_active = 0;
	}
	enemy->hp -= 1;
	if (enemy->hp <= 0) {
		delete_enemy(screen, index);
		screen->score -= 10;
	}
}

// bullet contain item
static void bullet_hits_player(t_screen *screen, int index) {
	t_object	*bullet;

	bullet = &screen->bullet[index];
	if (!(is_hit(bullet, &screen->player))) {
			return;
		}
	if (bullet->c == '+') {
		screen->player.hp -= 1;
		screen->score -= 10;
		// Reinforcement is removed when damaged
		memcpy(screen->player.color, WHITE, SIZE_COLOR);
		if (screen->player.hp <= 0) {
			screen->is_game_active = 0;
		}
		bullet->hp -= 1;
		if (bullet->hp <= 0) {
			delete_bullet(screen, index);
		}
	// Items disappear when retrieved
	} else if (bullet->c == 'x') {
		memcpy(screen->player.color, bullet->color, SIZE_COLOR);
		delete_bullet(screen, index);
		screen->score += 20;
	}
}

void	detection_all(t_screen *screen) {
	int	i=0;

	while (i < screen->now_size_b || i < screen->now_size_e) {
		if (i < screen->now_size_e) {
			enemy_out_of_bounds(screen, i);
			enemy_hits_player(screen, i);
		}
		if (i < screen->now_size_b) {
			bullet_out_of_bounds(screen, i);
			bullet_hits_player(screen, i);
			if (screen->enemy != NULL) {
				bullet_hits_enemy(screen, i);
			}
		}
		++i;
	}
}