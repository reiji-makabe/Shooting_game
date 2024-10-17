// The game progress is handled in a concurrent manner using multithreading
// Screen structure contains all the information
// See the header file for its structure
#include "shooting.h"

pthread_mutex_t	g_mutex;

int	main(void) {
	pthread_t	draw_screen_thread, player_input_thread, move_object_thread, read_panel_thread;
	t_screen	*screen;
	int			is_normal;
	#ifdef DEBUG
		pthread_setname_np(pthread_self(), "main");
	#endif


	// init
	screen = screen_init();
	if (screen == NULL) {
		return 1;
	}
	is_normal = pthread_mutex_init(&g_mutex, NULL);
	if (is_normal != 0) {
		perror("Failed to initialized mutex");
		exit(1);
	}

	// Generating a random number generator
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	long long milisec = tv.tv_sec * 1000L + tv.tv_usec / 1000;
	srand((unsigned int)milisec);

	#ifdef WIN
			enable_virtual_terminal_processing();
	#endif
	// clear screen
	printf("\033[2J\033[H");
	printf("\033[?25l");
	system("cls");

	// ToDo 難易度設定、ヘルプ画面
	main_menu(screen);
	for (int i=0; i<3; ++i) {
		system("cls");
		printf("\033[%d;%dH%d", SCREEN_H / 2, SCREEN_W / 2, i + 1);
		sleep_for_milliseconds(1000);
	}

	// main loop
	// Each thread is in an infinite loop
	pthread_create(&player_input_thread, NULL, player_input, screen);
	pthread_create(&move_object_thread, NULL, move_object, screen);
	pthread_create(&draw_screen_thread, NULL, draw_screen, screen);
	pthread_create(&read_panel_thread, NULL, read_panel, screen);
	pthread_join(player_input_thread, NULL);
	pthread_join(move_object_thread, NULL);
	pthread_join(draw_screen_thread, NULL);
	pthread_join(read_panel_thread, NULL);

	// Score Display
	system("cls");
	screen->score += screen->player.hp * 100;
	printf("\33[%d;%dHscore:%d", SCREEN_H / 2, SCREEN_W / 2 - 8, screen->score);

	// cleanup
	pthread_mutex_destroy(&g_mutex);
	free_t_screen(screen);
	printf("\033[?25h");
	fflush(stdin);
	exit(0);
}