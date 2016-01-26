#ifndef TEMPLATE_H
#define TEMPLATE_H
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#define SPRITE_DMA_CHANNEL 3
#define LENGTH 256
#define HEIGHT 192
typedef struct
{
	int x;
	int y;
}Point;

typedef struct
{
	int x;
	int y;
	int n;
	int hide;
	u16* sprite_gfx_mem;
	int gfx_frame;
}Num;

typedef struct
{
	int x;
	int y;
    int vx;
	int vy;
    int hide;
	double a;
	u16* sprite_gfx_mem;
}Ball;


typedef struct
{
	int vx;
	int vy;
}TURN;

static const int DMA_CHANNEL = 3;

int TIME;
void wait_click();
void prepare_digits(int score, Num *points,Num *sprites, int count_digits);
void prepare_timer(int time, Num *points,Num *sprites);
void initNum0(Num *sprites, u8* gfx, OamState *oam, Point num);
void initNum(Num *sprites, u8* gfx, OamState *oam);
#endif