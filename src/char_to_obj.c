#include <shooting.h>

void	char_to_obj(t_screen *screen, char c, int y, int x) {
	t_object	obj;
	obj.c = c;
	obj.x = x;
	obj.y = y;
	obj.x_speed = 0;
	obj.y_speed = 0;
	obj.hp = 1;
	memcpy(obj.color, WHITE, SIZE_COLOR);

	if (c == 'x') {
		obj.y_speed = 0.5;
		memcpy(obj.color, RED, SIZE_COLOR);
		create_bullet(screen, &obj);
	} else if (c == 'X') {
		obj.c = 'x';
		obj.y_speed = 0.5;
		memcpy(obj.color, BLUE, SIZE_COLOR);
		create_bullet(screen, &obj);
	} else if (c == 'z') {
		obj.c = 'x';
		obj.y_speed = 0.5;
		memcpy(obj.color, YELLOW, SIZE_COLOR);
		create_bullet(screen, &obj);
	} else if (c == 'Z') {
		obj.c = 'x';
		obj.y_speed = 0.5;
		memcpy(obj.color, GREEN, SIZE_COLOR);
		create_bullet(screen, &obj);
	} else if (c == 'o' || c == 'c' || c == 'd') {
		obj.y_speed = 0.5;
		obj.x_speed = rand() % 7 - 5; 
		memcpy(obj.color, WHITE, SIZE_COLOR);
		create_enemy(screen, &obj);
	} else if (c == 'O' || c == 'T' || c == 'D') {
		obj.y_speed = 1;
		obj.hp = 3;
		obj.x_speed = rand() % 7 - 5; 
		memcpy(obj.color, WHITE, SIZE_COLOR);
		create_enemy(screen, &obj);
	} else if (c == '@') {
		obj.hp = 50;
		obj.x_speed = 1;
		create_enemy(screen, &obj);
	} else {
		return;
	}
}