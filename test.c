#include <stdio.h>
#include <windows.h>

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

#include <string.h>

int	main(void) {
	char	*s = "\033[33m";
	int		len = strlen(s);
	while (1) {
		if (GetAsyncKeyState('A') & 0x8000) {
			printf("Aキーが押されました\n");
		}
	}
	printf("%stest\n", s);
	printf("%d\n", len);
}
/*

		t_object	obj[4];
		obj[0].y = 0;
		obj[0].x = SCREEN_W / 2;
		obj[0].y_speed = 0;
		obj[0].x_speed = 0;
		obj[0].c = 'O';
		obj[0].hp = 1;
		obj[0].x_speed = 0.5;
		memcpy(obj[0].color, BLACK, SIZE_COLOR);

		copy_obj(&obj[1], &obj[0]);
		copy_obj(&obj[2], &obj[0]);
		copy_obj(&obj[3], &obj[0]);
		obj[1].x -= 4;
		obj[1].c = 'x';
		obj[1].y_speed = 0.1;
		memcpy(obj[1].color, BLUE, SIZE_COLOR);
		obj[2].x += 4;
		obj[2].c = 'x';
		memcpy(obj[2].color, GREEN, SIZE_COLOR);
		obj[3].c = 'x';
		obj[3].y += 5;
		memcpy(obj[3].color, YELLOW, SIZE_COLOR);
		pthread_mutex_lock(&g_mutex);
		create_enemy(screen, &obj[0]);
		create_bullet(screen, &obj[1]);
		create_bullet(screen, &obj[2]);
		pthread_mutex_unlock(&g_mutex);
		*/