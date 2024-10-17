// A file that controls the creation and management of objects
// Objects are bullets, enemies, items, and everything else
// Items are to be implemented as bullets.(May be sized for individual items
#include "shooting.h"

void	copy_obj(t_object *dest, const t_object *src) {
	dest->x = src->x;
	dest->y = src->y;
	dest->x_speed = src->x_speed;
	dest->y_speed = src->y_speed;
	dest->c = src->c;
	dest->hp = src->hp;
	memcpy(dest->color, src->color, SIZE_COLOR);
}

void	create_enemy(t_screen *screen, t_object *obj) {
	t_object	*tmp_e;

	// if (current number of enemy == 0) calloc
	// else if (current + 1 enemy >= array size) realloc
	if (screen->now_size_e == 0) {
		screen->enemy = (t_object *)calloc((size_t)screen->now_size_e + OBJ_GROWTH_SIZE, sizeof(t_object));
		screen->size_enemy += OBJ_GROWTH_SIZE;
	}
	else if (screen->now_size_e == screen->size_enemy) {
		return;
		tmp_e = (t_object *)realloc(screen->enemy, (size_t)(screen->size_enemy + OBJ_GROWTH_SIZE) * sizeof(t_object));
		if (tmp_e == NULL) {
			perror("Failed to allocate memory on create_enemy");
			exit(1);
		}
		screen->enemy = tmp_e;
		screen->size_enemy += OBJ_GROWTH_SIZE;
	}
	
	// Assignment
	copy_obj(&screen->enemy[screen->now_size_e], obj);
	// Increment size
	screen->now_size_e += 1;
}

void	create_bullet(t_screen *screen, t_object *obj) {
	t_object	*tmp_b;

	// if (current number of bullet == 0) calloc
	// else if (current + 1 bullet >= array size) realloc
	if (screen->now_size_b == 0) {
		screen->bullet = (t_object *)calloc((size_t)screen->now_size_b + OBJ_GROWTH_SIZE, sizeof(t_object));
		screen->size_bullet += OBJ_GROWTH_SIZE;
	}
	else if (screen->now_size_b == screen->size_bullet) {
		return;
		tmp_b = (t_object *)realloc(screen->bullet, (size_t)(screen->size_bullet + OBJ_GROWTH_SIZE) * sizeof(t_object));
		if (tmp_b == NULL) {
			perror("Failed to allocate memory on create_bullet");
			exit(1);
		}
		screen->bullet = tmp_b;
		screen->size_bullet += OBJ_GROWTH_SIZE;
	}
	// Assignment
	copy_obj(&screen->bullet[screen->now_size_b], obj);
	// Increment size
	screen->now_size_b += 1;
}

void	free_object(t_screen *screen) {
	free(screen->bullet);
	free(screen->enemy);
}

void	delete_enemy(t_screen *screen, int index) {
	for (int i=0; i<screen->now_size_e; ++i) {
		if (i < index) {
			continue;
		} else {
			if (i + 1 != screen->size_enemy) {
				copy_obj(&screen->enemy[i], &screen->enemy[i + 1]);
			}
		}
	}
	screen->now_size_e -= 1;
}

void	delete_bullet(t_screen *screen, int index) {
	for (int i=0; i<screen->now_size_b; ++i) {
		if (i < index) {
			continue;
		} else {
			if (i + 1 != screen->size_bullet) {
				copy_obj(&screen->bullet[i], &screen->bullet[i + 1]);
			}
		}
	}
	screen->now_size_b -= 1;
}

static void	move_enemy(t_screen *screen) {

	// Enemy of 't' track the player
	for (int i=0; i<screen->now_size_e; ++i) {
		if (screen->enemy[i].c == 't' || screen->enemy[i].c == 'T') {
			if (screen->player.x > screen->enemy[i].x) {
				if (screen->enemy[i].x_speed < 0) {
					screen->enemy[i].x_speed *= -1;
				}
			} else if (screen->player.x < screen->enemy[i].x) {
				if (screen->enemy[i].x_speed > 0) {
					screen->enemy[i].x_speed *= -1;
				}
			}
		}
		screen->enemy[i].y += screen->enemy[i].y_speed;
		screen->enemy[i].x += screen->enemy[i].x_speed;
	}
}

static void	move_bullet(t_screen *screen) {

	for (int i=0; i<screen->now_size_b; ++i) {
		// Enemy red bullets track the player
		if (screen->bullet[i].c == '+' && !memcmp(screen->bullet[i].color, RED, SIZE_COLOR)) {
			if (screen->player.x > screen->bullet[i].x) {
				if (screen->bullet[i].x_speed < 0) {
					screen->bullet[i].x_speed *= -1;
				}
			} else if (screen->player.x < screen->bullet[i].x) {
				if (screen->bullet[i].x_speed > 0) {
					screen->bullet[i].x_speed *= -1;
				}
			}
		}
		screen->bullet[i].y += screen->bullet[i].y_speed;
		screen->bullet[i].x += screen->bullet[i].x_speed;
	}
}

static void	set_speed_reach_player(t_object *enemy, t_object *player,t_object *set_bul, double second) {
	set_bul->y_speed = second;
	set_bul->x_speed = (player->x - enemy->x) / second;
}

static void	enemy_action(t_screen *screen) {
	int	ran;
	t_object	bullet;
	static int	count = 0;
	
	// 適当な固定値でよくない ToDo:ゲームデザイン上最適な行動回数(考える必要ある?)
	if (++count < 10) {
		return;
	} else {
		count = 0;
	}
	ran = rand() % 7;
	bullet.c = '+';
	bullet.hp = 1;
	for (int i=0; i<screen->now_size_e; ++i) {
		if (screen->enemy[i].c == '@' || screen->enemy[i].c == 'd' || screen->enemy[i].c == 'D') {
			bullet.x = screen->enemy[i].x;
			bullet.y = screen->enemy[i].y + 1;
			set_speed_reach_player(&screen->enemy[i], &screen->player, &bullet, 0.5);
			switch (ran) {
			case 0:
			case 1:
			case 2:
				memcpy(bullet.color, WHITE, SIZE_COLOR);
				create_bullet(screen, &bullet);
				break;
			case 3:
				memcpy(bullet.color, RED, SIZE_COLOR);
				bullet.hp = 3;
				create_bullet(screen, &bullet);
				break;
			case 4:
				memcpy(bullet.color, BLUE, SIZE_COLOR);
				bullet.x_speed = -5;
				create_bullet(screen, &bullet);
				bullet.x_speed = 5;
				create_bullet(screen, &bullet);
				break;
			case 5:
				memcpy(bullet.color, YELLOW, SIZE_COLOR);
				create_bullet(screen, &bullet);
				bullet.x_speed = -1;
				create_bullet(screen, &bullet);
				bullet.x_speed = 1;
				create_bullet(screen, &bullet);
				break;
			case 6:
				bullet.hp = 2;
				memcpy(bullet.color, GREEN, SIZE_COLOR);
				create_bullet(screen, &bullet);
				break;
			default:
				break;
			}
		}
	}
}

void	*move_object(void *arg) {
	t_screen *screen = (t_screen *)arg;

	#ifdef DEBUG
		pthread_setname_np(pthread_self(), "move_object");
	#endif
	while (1) {
		pthread_mutex_lock(&g_mutex);
		if (screen->is_game_active == 0) {
			pthread_mutex_unlock(&g_mutex);
			return NULL;
		}
		move_enemy(screen);
		enemy_action(screen);
		move_bullet(screen);
		detection_all(screen);
		pthread_mutex_unlock(&g_mutex);

		sleep_for_milliseconds(INTERVAL_MOVE_OBJ);
	}
	return NULL;
}