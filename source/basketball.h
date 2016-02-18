#include "template.h"
#include "basketball_main.h"
#include "basketball_sub.h"
#include "Sprites.h"

#define BALL_RADIUS 16
#define BALL_DIAMETER 32 

typedef struct
{
	int x;
	int y;
	u16* sprite_gfx_mem;
}Basket;

int basketball(int Record);