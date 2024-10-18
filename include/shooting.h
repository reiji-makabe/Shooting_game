#ifndef SHOOTING_H
#define SHOOTING_H

#define SCREEN_H 20
#define SCREEN_W 60
#define FPS 60
#define TIME_PER_FRAME (1000 / FPS)
#define OBJ_MOVE_PER_SECOND 0.1
#define INTERVAL_MOVE_OBJ (int)(OBJ_MOVE_PER_SECOND * 1000)
#define OBJ_GROWTH_SIZE 256 // reallocを避けている よくないが長時間動かすとエラーが起きてしまうため
#define SIZE_COLOR 5

#define BLACK		"\033[30m"
#define RED			"\033[31m"
#define GREEN		"\033[32m"
#define YELLOW		"\033[33m"
#define BLUE		"\033[34m"
#define MAGENTA		"\033[35m"
#define CYAN		"\033[36m"
#define WHITE		"\033[37m"

#define B_BLACK		"\033[40m"
#define B_RED		"\033[41m"
#define B_GREEN		"\033[42m"
#define B_YELLOW	"\033[43m"
#define B_BLUE		"\033[44m"
#define B_MAGENTA	"\033[45m"
#define B_CYAN		"\033[46m"
#define B_WHITE		"\033[47m"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <dirent.h> // only panel.c
//#ifdef WIN
#include <windows.h>
#include <conio.h>
//#endif

typedef enum E_KEYS{
	E_KEY_SHOT,
	E_KEY_RIGHT,
	E_KEY_LEFT,
	E_KEY_UP,
	E_KEY_DOWN,
	E_KEY_END
} E_KEYS;

typedef struct	s_cell {
	char	object;
	char	obj_color[5];
	char	back_color[5];
}t_cell;

typedef struct	t_object {
	double		y;
	double		x;
	double		y_speed;
	double		x_speed;
	char		c;
	char		color[5];
	int			hp;
}t_object;

typedef struct s_panel {
	char 			**buf;
	int				difficulty;
	char			difficulty_name[10];
	DIR				*dir;
	struct dirent	*entry;
	FILE			*file;
	int				lines_read;
	int				apply_lines;
}t_panel;

typedef struct	s_screen {
	t_cell		**cells;	
	char		*buffer;
	int			size_enemy;
	int			size_bullet;
	int			now_size_e;
	int			now_size_b;
	t_object	*enemy;
	t_object	*bullet;
	t_object	player;
	int			is_game_active;
//	int			bomb_remain;
	t_panel		*panels;
	int			score;
//	int			max_panel_num;
}t_screen;
// ex. now_size == 2, -> enemy[0], enemy[1];

// global_variable
extern pthread_mutex_t	g_mutex;

// Menu
void	main_menu(t_screen *screen);

// Thread management functions
void	*player_input(void *arg);
void	*draw_screen(void *arg);
void	*move_object(void *arg);
void	*read_panel(void *arr);

// Screen manipulate functions
// Initialize the screen 
t_screen	*screen_init(void);
// Fill the screen cell with space
void	initialized_cell(t_screen *screen);
// Cursor visibility control
void	hide_cursor(void);
// Free screen
void	free_t_screen(t_screen *screen);
#ifdef WIN
// Enable escape sequence for windows
void enable_virtual_terminal_processing(void);
#endif

// Object manipulate functions
// Create object
void	create_enemy(t_screen *screen, t_object *obj);
void	create_bullet(t_screen *screen, t_object *obj);
// Copy obj
void	copy_obj(t_object *dest, const t_object *src);
// Delete object
void	delete_enemy(t_screen *screen, int index);
void	delete_bullet(t_screen *screen, int index);
// Free memory
void	free_object(t_screen *screen);

// Hit-detection functions
void	detection_all(t_screen *screen);

// Control time
int		get_current_time_ms(void);
void	sleep_for_milliseconds(int milliseconds);

// Utils
void	char_to_obj(t_screen *screen, char c, int y, int x);

#endif