#include "shooting.h"

void	main_menu(t_screen *screen) {
	int	flag = 1;
	char	c;


	while (flag) {
		printf("\033[%d;%dH", SCREEN_H / 2 - 3, SCREEN_W / 2 - 10);
		printf("Choose Difficulty:easy 0\n");
		printf("\033[%dCnormal 1\n", SCREEN_W / 2 + 7);
		printf("\033[%dChard 2\n", SCREEN_W / 2 + 7);
		printf("\033[%dClunatic 3\n", SCREEN_W / 2 + 7);

		c = (char)_getch();
		if (c == '1') {
			screen->panels->difficulty = 1;
			return;
		} else {
			printf("Sorry, No difficulty level other than normal is implemented.\n");
		}
	}
}