#include "football.h"

void initBallfoot(Ball *sprite, u8* gfx, OamState *oam)
{
	sprite->sprite_gfx_mem = oamAllocateGfx(oam, SpriteSize_64x64, SpriteColorFormat_256Color);
	dmaCopy(gfx, sprite->sprite_gfx_mem, 64*64);
	
}


int is_capture2(Ball ball_main, int x, int y)
{
  int x0 = ball_main.x + BIG_BALL_RADIUS - 6;
  int y0 = ball_main.y + BIG_BALL_RADIUS - 6;
  if(x==0 && y==0)
    return 0;
  if((x0-x)*(x0-x) + (y0-y)*(y0-y) <= BIG_BALL_RADIUS*BIG_BALL_RADIUS -36)
    return 1;
  else return 0;
}


int kick(Ball *ball, int x, int y)
{
  int sum=0;
  if(is_capture2(*ball,x,y)==0)
    return sum;
  sum = abs(ball->vx) + abs(ball->vy);
  ball->vx = (ball->x+BIG_BALL_RADIUS-x)/4;
  if (ball->y+BIG_BALL_RADIUS > y)
  ball->a = ball->vy = (int)(ball->y+BIG_BALL_RADIUS-y)/3 + 4;
  else 
  ball->a = ball->vy = (int)(ball->y+BIG_BALL_RADIUS-y)/3 - 4;
  return sum;
}

int ball_vs_area(Ball *ball)
{
    ball->x=ball->x+ball->vx;
	ball->y=ball->y+ball->vy;
	if(ball->x > LENGTH - BIG_BALL_DIAMETER)
	{
	   ball->x = LENGTH - BIG_BALL_DIAMETER;
	   ball->vx=-ball->vx/2-1;
	}
	if(ball->x < 0)
	{
	   ball->x = 0;
	   ball->vx=-ball->vx/2+1;
	}
    if(ball->y > HEIGHT - BIG_BALL_DIAMETER)
    {
	  ball->y=HEIGHT - BIG_BALL_DIAMETER;
	  ball->a = ball->vy = -ball->vy/2;
	  return 1;
    }
	else 
	{ 
	  ball->a+=0.2;
	  ball->vy=(int)ball->a;
	}
	return 0;
	
}

int football(int REC)
{
    int is_touch=1, prev_touch=1,first_touch=0,game_over=0, points=0,pos_score=218,pos_rec=33,i=0;
	int SCORE=0;
	Ball ball_main={112,126,0,0};
	Ball ball_sub={112,126};
	touchPosition touch;
	Num Numbers[10];
	Num Points[3]={{pos_score,10,0,0},{pos_score-12,10,0,1},{pos_score-24,10,0,1}};
	Num Record[3]={{pos_rec+24,10,0,0},{pos_rec+12,10,0,1},{pos_rec,10,0,1}};
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
	
	decompress(football_subBitmap, BG_GFX_SUB,  LZ77Vram);
	decompress(football_mainBitmap, BG_GFX,  LZ77Vram);
	
	oamInit(&oamMain, SpriteMapping_1D_128, false);
	oamInit(&oamSub, SpriteMapping_1D_128, false);
	
	initNum(Numbers, (u8*)football_spritesTiles,&oamSub);
	initBallfoot(&ball_main, (u8*)football_spritesTiles,&oamMain);
	initBallfoot(&ball_sub, (u8*)football_spritesTiles,&oamSub);
	
	
	dmaCopy(football_spritesPal, SPRITE_PALETTE, 512);
	dmaCopy(football_spritesPal, SPRITE_PALETTE_SUB, 512);
	
	//wait_click();
	while(1)
	{
	    scanKeys();
		touchRead(&touch);
		
		prev_touch = is_touch;
		if(touch.px==0 && touch.py==0)
		  is_touch=0;
		else 
		  is_touch=1;
		  
		if(first_touch==1)
		{
		   ball_main.a = ball_main.vy=-8;
		   first_touch=2;
		   is_touch=0;
		}
		 
		 /*ball vs touch*/
		if(prev_touch==0)
		{
		  points = kick(&ball_main,touch.px,touch.py);
		  if(first_touch==2)
		  SCORE += points;
		}
		if(first_touch==2)
		game_over = ball_vs_area(&ball_main);
		
		/*ball in sub or main screen?*/
		ball_sub.x = ball_main.x;
		if(-BIG_BALL_DIAMETER < ball_main.y+HEIGHT && ball_main.y < 0)
		{
		  ball_sub.y = ball_main.y+HEIGHT;
		  ball_sub.hide=0;
		}
		else 
		  ball_sub.hide=1;
		if(ball_main.y<-BIG_BALL_DIAMETER)
			ball_main.hide=1;
		else 
			ball_main.hide=0;
		
        /*update screen*/	
		prepare_digits(SCORE,Points,Numbers,3);
		prepare_digits(REC,Record,Numbers,3);
	    if(first_touch==2 && game_over==1)
		  break;
		  
		for(i=0;i<3;i++)
		oamSet(&oamSub, i+2, Points[i].x, Points[i].y,  0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
			Points[i].sprite_gfx_mem, -1, 0, Points[i].hide, 0, 0, 0);
		for(i=0;i<3;i++)
		oamSet(&oamSub, i+5, Record[i].x, Record[i].y,  0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
			Record[i].sprite_gfx_mem, -1, 0, Record[i].hide, 0, 0, 0);  
		oamSet(&oamMain, 1, ball_main.x, ball_main.y, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, 
			ball_main.sprite_gfx_mem, -1, 0, ball_main.hide, 0, 0, 0);
		oamSet(&oamSub, 1, ball_sub.x, ball_sub.y, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, 
			ball_sub.sprite_gfx_mem, -1, 0, ball_sub.hide, 0, 0, 0);
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
		
		if(first_touch==0)
		{
		  wait_click();
		  first_touch=1;
		}
	}
	wait_click();
	oamClear(&oamMain,0,128);
	oamClear(&oamSub,0,128);
	oamUpdate(&oamMain);
	oamUpdate(&oamSub);
	return SCORE;
}

