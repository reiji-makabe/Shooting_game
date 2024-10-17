// A file dealing with the functions involved in processing the input and changing the player.
// Shooting bullets, bombs, taking items and changing weapons...
// As for the HP reduction, it is in the hit detection.
#include "shooting.h"

static void	get_key_input(int *key_status) {
	int	loop;

	loop = 0;
	memset(key_status, 0, sizeof(int) * (E_KEY_END + 1));
	while (1) {
		if (GetKeyState(' ') & 0x8000 || GetKeyState('\r') & 0x8000 || GetKeyState('\n') & 0x8000) {
			key_status[E_KEY_SHOT] = 1;
		}
		if (GetKeyState('A') & 0x8000 || GetKeyState('H') & 0x8000) {
			key_status[E_KEY_LEFT] = 1;
		}
		if (GetKeyState('D') & 0x8000 || GetKeyState('L') & 0x8000) {
			key_status[E_KEY_RIGHT] = 1;
		}
		if (GetKeyState('W') & 0x8000 || GetKeyState('K') & 0x8000) {
			key_status[E_KEY_UP] = 1;
		}
		if (GetKeyState('S') & 0x8000 || GetKeyState('J') & 0x8000) {
			key_status[E_KEY_DOWN] = 1;
		}
		if (loop > TIME_PER_FRAME) {
			return;
		}
		++loop;
	}
	sleep_for_milliseconds(1);
}

static void	handle_key_input(t_screen *screen, int *key_status) {
	t_object bullet;

	if (key_status[E_KEY_SHOT]) {
		bullet.y = screen->player.y - 1;
		bullet.x = screen->player.x;
		bullet.y_speed = -2;
		bullet.x_speed = 0;
		bullet.c = '*';
		bullet.hp = 0;
		if (!memcmp(screen->player.color, WHITE, SIZE_COLOR)) {
			memcpy(bullet.color, WHITE, SIZE_COLOR);
			create_bullet(screen, &bullet);
		} else if (!memcmp(screen->player.color, RED, SIZE_COLOR)) {
			memcpy(bullet.color, RED, SIZE_COLOR);
			bullet.hp = 3;
			create_bullet(screen, &bullet);
		} else if (!memcmp(screen->player.color, BLUE, SIZE_COLOR)) {
			memcpy(bullet.color, BLUE, SIZE_COLOR);
			bullet.x_speed = -5;
			create_bullet(screen, &bullet);
			bullet.x_speed = 5;
			create_bullet(screen, &bullet);
		} else if (!memcmp(screen->player.color, YELLOW, SIZE_COLOR)) {
			memcpy(bullet.color, YELLOW, SIZE_COLOR);
			create_bullet(screen, &bullet);
			bullet.x_speed = -1;
			create_bullet(screen, &bullet);
			bullet.x_speed = 1;
			create_bullet(screen, &bullet);
		} else if (!memcmp(screen->player.color, GREEN, SIZE_COLOR)) {
			bullet.hp = 2;
			memcpy(bullet.color, GREEN, SIZE_COLOR);
			create_bullet(screen, &bullet);
		}
		key_status[E_KEY_SHOT] = 0;
	}
	if (key_status[E_KEY_LEFT]) {
		if (screen->player.x > 0) {
			screen->player.x -= 0.5;
		}
		key_status[E_KEY_LEFT] = 0;
	}
	if (key_status[E_KEY_RIGHT]) {
		if (screen->player.x < SCREEN_W - 1) {
			screen->player.x += 0.5;
		}
		key_status[E_KEY_RIGHT] = 0;
	}
	if (key_status[E_KEY_UP]) {
		if (screen->player.y > 0) {
			screen->player.y -= 0.5;
		}
		key_status[E_KEY_UP] = 0;
	}
	if (key_status[E_KEY_DOWN]) {
		if (screen->player.y < SCREEN_H - 1) {
			screen->player.y += 0.5;
		}
		key_status[E_KEY_DOWN] = 0;
	}
}

void	*player_input(void *arg) {
	t_screen	*screen;
	int			key_status[E_KEY_END];

	screen = (t_screen *)arg;
	#ifdef DEBUG
		pthread_setname_np(pthread_self(), "player_input");
	#endif
	while (1) {
		pthread_mutex_lock(&g_mutex);
		if (screen->is_game_active == 0) {
			pthread_mutex_unlock(&g_mutex);
			return NULL;
		}
		get_key_input(key_status);
		// IO handle
		handle_key_input(screen, key_status);
		// stream fulsh
		// ToDo2:Unix環境だと非標準
		pthread_mutex_unlock(&g_mutex);
		sleep_for_milliseconds(TIME_PER_FRAME);
	}
	free(key_status);
	return NULL;
}