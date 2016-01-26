#include "template.h"
#include "Menu_screen.h"
#include "Menu_screen_sub.h"
#include "menu_sprites.h"
#include "help_colors.h"
#include "help_basketball.h"
#include "help_football.h"

typedef struct
{
	int x;
	int y;
	int hide;
	u16* sprite_gfx_mem_top;
	u16* sprite_gfx_mem_bot;
}Border;


int menu(int *Result, int Money);

