#include "template.h"
#include "coulours_main.h"
#include "coulours_sub.h"
#include "Coulours_Sprites.h"

#define  NUMBER_OF_SQUARES 4
#define  NUMBER_OF_SETS 9
#define  NUMBER_OF_TRUE_SQUARES 4
#define  NUMBER_OF_ALL_SQUARES 40
#define  NUMBER_OF_ALL_NOT_DIGIT 50

typedef struct
{
	int x;
	int y;
	u16* sprite_gfx_mem;
}COLOR;

typedef struct
{
	int x;
	int y;
	int n;
	int hide;
	u16* sprite_gfx_mem;
}SQUARE;

typedef struct
{
	int x;
	int y;
	int pos;
	u16* sprite_gfx_mem;
}SMALL;


int colors(int Record);