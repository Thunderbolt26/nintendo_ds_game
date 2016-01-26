#include "basketball.h"

void initBall(Ball *sprite, u8* gfx, OamState *oam)
{
    int i;
	sprite->sprite_gfx_mem = oamAllocateGfx(oam, SpriteSize_32x32, SpriteColorFormat_256Color);
	for(i=0;i<4;i++)
	dmaCopy(gfx+i*8*64, sprite->sprite_gfx_mem+i*4*32, 32*8);
	
}



void initBasket(Basket *sprite, u8* gfx, OamState *oam)
{
	sprite->sprite_gfx_mem = oamAllocateGfx(oam, SpriteSize_64x32, SpriteColorFormat_256Color);
	dmaCopy(gfx+64*32, sprite->sprite_gfx_mem, 64*32);
}


void timerCallBack()
{
   TIME--;
}

int is_capture(Ball ball_main, int x, int y)
{
  int x0 = ball_main.x + BALL_RADIUS;
  int y0 = ball_main.y + BALL_RADIUS;
  if(x==0 && y==0)
    return 0;
  if((x0-x)*(x0-x) + (y0-y)*(y0-y) <= BALL_RADIUS*BALL_RADIUS)
    return 1;
  else return 0;
}


int rebound_from_ring(Ball ball)
{
  Point basket_point[]={{182,104},{181,103},{180,102},{181,101},{182,101},{183,102},{183,103}};
  int i=0;
  for(i=0;i<7;i++)
  if(is_capture(ball, basket_point[i].x,basket_point[i].y)==1)
    return 1;
  return 0;
}

TURN ball_vs_basket2(Ball ball, Basket basket)
{
   Point basket_point[]={{225,102},{219,100},{226,100},{240,84},{240,40}};
   int size=5;
   int x = ball.x + 16;
   int y = ball.y + 16;
   TURN z={ball.vx,ball.vy};
   if((x - y <= basket_point[size-1].x - basket_point[size-1].y) && (x + y <= basket_point[size-2].x + basket_point[size-2].y) && (x>=basket_point[size-1].x-16) && (x<=basket_point[size-1].x))
   {
      z.vx = -ball.vx-1;
	  z.vy = ball.vy;
   }
   if((x - y > basket_point[size-1].x - basket_point[size-1].y) && (y<=basket_point[size-1].y)&& (y>=basket_point[size-1].y+16))
   {
      z.vx = ball.vx;
	  z.vy = -ball.vy;
   }
   if((x - y >= basket_point[1].x - basket_point[1].y) && (x + y <= basket_point[size-2].x + basket_point[size-2].y) && (y>=basket_point[2].y-16) && (y<=basket_point[2].y))
   {
      z.vx = ball.vx;
	  z.vy = -ball.vy;
   }
   if((x - y < basket_point[1].x - basket_point[1].y) && (x < basket_point[1].x) && (ball.vx >= 0)&&(x >= basket_point[1].x - 16) )
   {
       z.vx = -ball.vx+1;
	   z.vy = ball.vy;
	   return z;
   }
   return z;
}

TURN ball_vs_net(Ball ball, Basket basket)
{
   Point basket_point[]={{182,104},{182,130},{240,130}};
   int x = ball.x + 16;
   int y = ball.y + 16;
   TURN z={ball.vx,ball.vy};
   if((x + y >= basket_point[0].x + basket_point[0].y) && (x + y <= basket_point[1].x + basket_point[1].y) && (x>=basket_point[1].x-16) && (x<=basket_point[1].x))
   {
      z.vx = -ball.vx;
	  z.vy = ball.vy;
	  return z;
   }
   if((x + y >= basket_point[1].x + basket_point[1].y) && (x - y <= basket_point[2].x - basket_point[2].y) && (y<=basket_point[1].y+16) && (y>=basket_point[1].y))
   {
      z.vx = ball.vx;
	  z.vy = -ball.vy;
	  return z;
   }
   return z;
}

TURN ball_vs_basket1(Ball ball, Basket basket)
{
  Point basket_point[]={{182,104},{181,103},{180,102},{181,101},{182,101},{183,101},{182,101},{183,102},{183,103}};
  int x = ball.x + 16;
  int y = ball.y + 16;
  TURN z={ball.vx,ball.vy};
  
  if((x-y < basket_point[2].x - basket_point[2].y) && (x + y >= basket_point[2].x + basket_point[2].y) && (x + y < basket_point[0].x + basket_point[0].y) )
  {
    z.vx = ball.vy;
	z.vy = ball.vx;
	
  }
  if((x-y < basket_point[2].x - basket_point[2].y) && (x + y < basket_point[2].x + basket_point[2].y) && (ball.vx >=0))
  {
    z.vx = -ball.vx-1;
	z.vy = ball.vy;
	
  }
  if((x-y >= basket_point[2].x - basket_point[2].y) && (x + y < basket_point[2].x + basket_point[2].y) && (x-y <= basket_point[3].x - basket_point[3].y))
  {
     z.vx = -ball.vy;
	 z.vy = -ball.vx;
	
  }
  if((y<basket_point[2].y) && (x-y > basket_point[3].x - basket_point[3].y) && (x + y < basket_point[6].x + basket_point[6].y) && (ball.vy >=0))
  {
     z.vy = -ball.vy;
	  z.vx = ball.vx;
	  
  }
  if((x-y >= basket_point[6].x - basket_point[6].y) && (x + y >= basket_point[6].x + basket_point[6].y) && (x + y <= basket_point[7].x + basket_point[7].y))
  {
      z.vx = ball.vy;
	  z.vy = ball.vx;
	 
  }
  if((x + y > basket_point[7].x + basket_point[7].y) && (x > basket_point[7].x) && (ball.vx <= 0))
  {
      z.vx = -ball.vx+1;
	  z.vy = ball.vy;
	 
  }
  return z;
}


int is_point(Ball ball, Basket basket)
{
   if(basket.x +2 < ball.x && ball.x < basket.x + 24 && ball.y + 16 > basket.y && ball.y + 16 <=130)
      return 1;
   else return 0;
}

void ballreset(Ball *ball)
{
   ball->x=11;
   ball->y=149;
   ball->vx=0;
   ball->vy=0;
   oamSet(&oamMain, 1, ball->x, ball->y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
			ball->sprite_gfx_mem, -1, false, false, false, false, false);
   swiWaitForVBlank();
   oamUpdate(&oamMain);
}
int basketball(int REC)
{
	int is_touch=0, flag=0, prev_touch=0,exit=1; /*flags*/
	int dx,dy;
	int i=0;
	int pos_score=218, pos_time=33, pos_rec=33;
	int SCORE=0, RECORD=REC;
	Ball ball_main={11,149};
	Ball ball_sub;
	Basket basket={180,99};
	Num Numbers[10];
	Num Points[3]={{pos_score,10,0,0},{pos_score-12,10,0,1},{pos_score-24,10,0,1}};
	Num Record[3]={{pos_rec+24,10,0,0},{pos_rec+12,10,0,1},{pos_rec,10,0,1}};
	Num Timer[3]={{pos_time+29,35,0,0},{pos_time+17,35,0,0},{pos_time,35,0,0}};
	
	ball_main.vx=0;
	ball_main.vy=0;
	ball_sub.vx=0;
	ball_sub.vy=0;
	ball_main.hide=1;
	ball_sub.hide=0;
	TIME=60;
	TURN z;
	touchPosition touch;
	
	powerOn(POWER_ALL_2D);
	lcdMainOnBottom(); /* Place the main screen on the bottom physical screen */
	videoSetMode(MODE_5_2D | /* Enable BG3 for display*/
                 DISPLAY_SPR_ACTIVE | /* Enable sprites for display*/
                 DISPLAY_SPR_1D );
    videoSetModeSub(MODE_5_2D);
	
	
    bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0); 

	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG);
	
	decompress(basketball_subBitmap, BG_GFX_SUB,  LZ77Vram);
	decompress(basketball_mainBitmap, BG_GFX,  LZ77Vram);
	
	oamInit(&oamMain, SpriteMapping_1D_128, false);
	oamInit(&oamSub, SpriteMapping_1D_128, false);
	
	initNum(Numbers, (u8*)SpritesTiles,&oamSub);
	initBall(&ball_main, (u8*)SpritesTiles,&oamMain);
	initBasket(&basket, (u8*)SpritesTiles, &oamSub);
	initBall(&ball_sub, (u8*)SpritesTiles,&oamSub);
	
	
	dmaCopy(SpritesPal, SPRITE_PALETTE, 512);
	dmaCopy(SpritesPal, SPRITE_PALETTE_SUB, 512);
		
	
	timerStart(0, ClockDivider_1024, TIMER_FREQ_1024(1), timerCallBack);
	while(exit) {
		swiWaitForVBlank();
		scanKeys();
		touchRead(&touch);
		
		prev_touch = is_touch;
		

		/*control out of area*/
		if(ball_main.x <=-BALL_DIAMETER || ball_main.x >=LENGTH)
		{
		  
		  ballreset(&ball_main);
		  continue;
		}
		/*is_touch - ?*/
		if(touch.px==0 && touch.py==0)
		  is_touch=0;
		else {is_touch=1; };
		  
		if(is_touch==0)
          flag=0;
		/*is_capture - ?*/
		if(is_capture(ball_main, touch.px, touch.py) && (prev_touch==0))
		{
		   dx = ball_main.x  - touch.px;
	       dy = ball_main.y  - touch.py;
		   flag=1;
		}
		/*control ball by touchpad*/   
		
		if(flag)
		{
		   ball_main.vx=touch.px + dx - ball_main.x;
		   ball_main.vy=touch.py + dy - ball_main.y;
		   if(ball_main.vx>26)
		     ball_main.vx=26;
		   if(ball_main.vy>16)
		     ball_main.vy=16;
		   if(ball_main.vx<-26)
		     ball_main.vx=-26;
		   if(ball_main.vy<-26)
		     ball_main.vy=-26;
		   ball_main.x= touch.px + dx;
		   ball_main.y= touch.py + dy;
		}
		else /*control ball by gravity*/ 
        {
		   ball_main.x=ball_main.x+ball_main.vx;
		   if(ball_main.y+ball_main.vy < HEIGHT - BALL_DIAMETER)
		   {
		     ball_main.y=ball_main.y+ball_main.vy;
			 ball_main.vy = ball_main.vy+1;
		   }
		   else 
		   {
		     ball_main.y=HEIGHT - BALL_DIAMETER;
			 ball_main.vy=-ball_main.vy/2;
		   }
		   
		}
		ball_sub.x = ball_main.x;
		/*ball in sub or main screen?*/ 
		if(-BALL_DIAMETER <ball_main.y+HEIGHT && ball_main.y+HEIGHT< HEIGHT + BALL_DIAMETER)
		{
		  ball_sub.y = ball_main.y+HEIGHT;
		  ball_sub.hide=0;
		}
		else 
		  ball_sub.hide=1;
		

		if(ball_main.y<-BALL_DIAMETER)
			ball_main.hide=1;
		else 
			ball_main.hide=0;
		
		/*ball vs basket*/ 
		if(ball_sub.hide==0)
		{
		  ball_sub.vx = ball_main.vx;
		  ball_sub.vy = ball_main.vy;
		  z = ball_vs_basket2(ball_sub, basket);
		  ball_sub.vx=z.vx;
          ball_sub.vy = z.vy;
		  z = ball_vs_net(ball_sub, basket);
		  ball_sub.vx=z.vx;
          ball_sub.vy = z.vy;
		  
		  if(rebound_from_ring(ball_sub)==1)
		  {
		      z=ball_vs_basket1(ball_sub, basket);
		      ball_sub.vx=z.vx;
              ball_sub.vy = z.vy;
		  }
		  ball_main.vx = ball_sub.vx;
		  ball_main.vy = ball_sub.vy;
		}
		if(is_point(ball_sub, basket)==1 && ball_sub.hide==0)
		{
		   SCORE+=10;
		   ballreset(&ball_main);  
		}

		/*update screen*/
		prepare_digits(SCORE,Points,Numbers,3);
		prepare_timer(TIME,Timer,Numbers);
		prepare_digits(RECORD,Record,Numbers,3);
		exit=TIME;
		for(i=0;i<3;i++)
		oamSet(&oamSub, i+2, Points[i].x, Points[i].y,  0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
			Points[i].sprite_gfx_mem, -1, 0, Points[i].hide, 0, 0, 0);	  
	    for(i=0;i<3;i++)
		oamSet(&oamSub, i+5, Timer[i].x, Timer[i].y,  0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
			Timer[i].sprite_gfx_mem, -1, 0, Timer[i].hide, 0, 0, 0);
		for(i=0;i<3;i++)
		oamSet(&oamSub, i+8, Record[i].x, Record[i].y,  0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
			Record[i].sprite_gfx_mem, -1, 0, Record[i].hide, 0, 0, 0);
			
		oamSet(&oamMain, 1, ball_main.x, ball_main.y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
			ball_main.sprite_gfx_mem, -1, 0, ball_main.hide, 0, 0, 0);
		oamSet(&oamSub, 1, ball_sub.x, ball_sub.y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
			ball_sub.sprite_gfx_mem, -1, 0, ball_sub.hide, 0, 0, 0);
		oamSet(&oamSub, 0, basket.x, basket.y, 0, 0, SpriteSize_64x32, SpriteColorFormat_256Color, 
			basket.sprite_gfx_mem, -1, 0, 0, 0, 0, 0);	
			
		
			
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
		
	}
    wait_click();
	oamClear(&oamMain,0,128);
	oamClear(&oamSub,0,128);
	oamUpdate(&oamMain);
	oamUpdate(&oamSub);
	timerStop(0);
	return SCORE; 
}

