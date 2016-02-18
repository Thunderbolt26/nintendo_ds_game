#include "template.h"



void wait_click()
{
  int is_touch=1, prev_touch=0;
  touchPosition touch;
  while(1)
	{
	    touchRead(&touch);
		prev_touch = is_touch;
		if(touch.px==0 && touch.py==0)
		  is_touch=0;
		else {is_touch=1;}
		if(prev_touch==0 &&  is_touch==1)
		break;
	}
}

void prepare_timer(int time, Num *points,Num *sprites)
{
   
   points[2].n=time/60;
   if(points[2].n>9)
     points[2].n=9;
   points[2].sprite_gfx_mem=sprites[points[2].n].sprite_gfx_mem;
   time=time%60;
   prepare_digits(time,points,sprites,3);
   if(time<10)
   {
      points[1].n=0;
	  points[1].sprite_gfx_mem=sprites[0].sprite_gfx_mem;
   }
   points[0].hide=0;
   points[1].hide=0;
   points[2].hide=0;
}

void prepare_digits(int score, Num *points,Num *sprites, int count_digits)
{
   int i=0;
   int j=0;
   do
   {
      i=score % 10;
	  points[j].n=i;
	  points[j].hide=0;
	  points[j].sprite_gfx_mem=sprites[i].sprite_gfx_mem;
	  score = score / 10;
	  j++;
   }
   while(score!=0);
   while (j<count_digits)
   {
     points[j].hide=1;
	 j++;
   }
   return;
}

void initNum0(Num *sprite, u8* gfx, OamState *oam, Point num)
{
    int i;
	sprite->sprite_gfx_mem = oamAllocateGfx(oam, SpriteSize_16x16, SpriteColorFormat_256Color);
	for(i=0;i<2;i++)
	dmaCopy(gfx+i*8*64 + 64*64 + num.x*8*16 + num.y*16*64, sprite->sprite_gfx_mem+i*4*16, 16*8);
}
void initNum(Num *sprites, u8* gfx, OamState *oam)
{
    Point num = {0,0};
	int i=0;
	for(num.y=0; num.y < 3; num.y++)
	  for(num.x=0; num.x < 4 || (num.y == 2 && num.x < 2); num.x++)
      {
	     initNum0(&(sprites[i]),gfx,oam,num);
		 sprites[i].n=i;
	     i++;
	  }
}


int is_continue(touchPosition touch)
{
  int x= touch.px;
  int y = touch.py;
  Point border[5]={{206,169}, {206,185}, {223,162},{223,192},{239,176}};
  if(x<=border[2].x)
    if(x >= border[0].x && y >= border[0].y && y <= border[1].y)
      return 1;
	else 
      return 0;
  else
    if((x-y <= border[2].x - border[2].y) && (x+y <= border[3].x + border[3].y))
	  return 1;
	else return 0;
    	  
}

int is_back(touchPosition touch)
{
  int x= touch.px;
  int y = touch.py;
  Point border[4]={{54,169}, {54,185}, {38,169},{38,192}};
  if(x>=border[2].x)
    if(x <= border[0].x && y >= border[0].y && y <= border[1].y)
      return 1;
	else 
      return 0;
  else
    if((x-y >= border[3].x - border[3].y) && (x+y >= border[2].x + border[2].y))
	  return 1;
	else return 0;
    	  
}