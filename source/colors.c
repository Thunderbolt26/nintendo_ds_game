#include "colors.h"

void timerCallBack2()
{
   TIME--;
}

void initPlus0(SMALL *sprite, u8* gfx, OamState *oam)
{
    int i=0;
	sprite->sprite_gfx_mem = oamAllocateGfx(oam, SpriteSize_16x16, SpriteColorFormat_256Color);
	for(i=0;i<2;i++)
	dmaCopy(gfx+i*8*64 + 3*64*64, sprite->sprite_gfx_mem+i*4*16, 16*8);
}

void initPlus(SMALL *sprites, u8* gfx, OamState *oam)
{
    int i;
	for(i=0;i<2;i++)
    {
	     initPlus0(&(sprites[i]),gfx + i*8*32,oam);
	}
}
void initColour(COLOR *sprite, u8* gfx, OamState *oam, Point num)
{
    int i=0;
	sprite->sprite_gfx_mem = oamAllocateGfx(oam, SpriteSize_32x32, SpriteColorFormat_256Color);
	for(i=0;i<4;i++)
	dmaCopy(gfx+i*8*64+ num.x*8*32 + num.y*32*64, sprite->sprite_gfx_mem+i*4*32, 32*8);
	
}

void initColours(COLOR *sprites, u8* gfx, OamState *oam)
{
    Point num = {0,0};
	int i=0;
	for(num.y=0; num.y < 2; num.y++)
	  for(num.x=0; num.x < 2; num.x++)
      {
	     initColour(&(sprites[i]),gfx,oam,num);
		 i++;
	  }
	for(num.y=0; num.y < 2; num.y++)
	  for(num.x=0; num.x < 2; num.x++)
      {
	     initColour(&(sprites[i]),gfx+128*64,oam,num);
		 i++;
	  }
}


void initElement(SQUARE *El, Point point)
{
  El->hide=1;
  El->x=point.x;
  El->y=point.y;
  El->n=0;
  return;
}


int prepare_squares(SQUARE *El, COLOR Colours[8],int n, Point *last)
{
   El->n=n;
   El->hide=0;
   El->sprite_gfx_mem=Colours[n].sprite_gfx_mem;
   if(last->x==3) 
   {
     last->x=0;
	 last->y++;
   }
   else last->x++;
   if(last->y==9) 
     return 1;
   return 0;
}

void initCorrect(SMALL Correct_color[4], SMALL Correct_pos[4],SMALL Plus[2])
{
   int i=0;
   Point point={207,70};
   Point point_pos={207,101};
   for(i=0;i<4;i++)
   {
     Correct_color[i].pos=-1;
	 Correct_color[i].x = point.x + (i%2)*12;
	 Correct_color[i].y = point.y + (i/2)*12;
	 Correct_color[i].sprite_gfx_mem = Plus[0].sprite_gfx_mem;
	 
	 Correct_pos[i].pos=-1;
	 Correct_pos[i].x = point_pos.x + (i%2)*12;
	 Correct_pos[i].y = point_pos.y + (i/2)*12;
	 Correct_pos[i].sprite_gfx_mem = Plus[1].sprite_gfx_mem;
   }
}

int checkSet(SQUARE TrueSet[NUMBER_OF_TRUE_SQUARES], SQUARE Set[NUMBER_OF_SQUARES], SMALL Correct_color[4], SMALL Correct_pos[4])
{
   int color=0;
   int pos=0;
   int i,j,z,flag=0;
   for(i=0;i<4;i++)
   {
     Correct_color[i].pos=-1;
	 Correct_pos[i].pos=-1;
   }
   /*check_color*/
   for(i=0;i<NUMBER_OF_TRUE_SQUARES;i++)
   {
     for(j=0;j<NUMBER_OF_SQUARES;j++)
     { 
       if(TrueSet[i].n==Set[j].n)
	   {
	     if(color!=0)
	     {
	       flag=0; 
	       for(z=0;z<color;z++)
	       if(Correct_color[z].pos==j)
             flag=1;
	     }
	     if (flag==1) continue;
	     Correct_color[color].pos=j;
	     color++;
	     break;
	    }
     }
   }
   /*check_position*/
   for(i=0;i<NUMBER_OF_SQUARES;i++)
   {
     if(TrueSet[i].n==Set[i].n)
	 {
	    Correct_pos[pos].pos=i;
	    pos++;
	 }
   }
   if(pos==NUMBER_OF_SQUARES) return 1;
   return 0;
}

int read_color(touchPosition touch)
{
   int i=0;
   Point point={19,156};
   for(i=0; i < 8; i++)
   {
     if(touch.px>=point.x && touch.px<=point.x+17 && touch.py>=point.y && touch.py<=point.y+17)
	   return i;
	 point.y-=21;
   }
   return -1;
}


int colors(int REC)
{
    int i=0,j=0,pos_score=218,pos_rec=33, pos_time=24,flag=0,is_touch=0, prev_touch=0,win=0;
	int SCORE=999;int RECORD=REC;
	touchPosition touch;
    Num Numbers[10];
	Num Points[3]={{pos_score,14,0,0},{pos_score-12,14,0,1},{pos_score-24,14,0,1}};
	Num Record[3]={{pos_rec+24,14,0,0},{pos_rec+12,14,0,1},{pos_rec,14,0,1}};
	Num Timer[3]={{pos_time+29,36,0,0},{pos_time+17,36,0,0},{pos_time,36,0,0}};
	COLOR Colours[8];
	COLOR Colours_sub[8];
	SMALL Plus[2];
	SMALL Correct_color[4];
	SMALL Correct_pos[4];
	SQUARE Set[NUMBER_OF_SETS][NUMBER_OF_SQUARES];
	SQUARE TrueSet[NUMBER_OF_TRUE_SQUARES];
	Point point = {64,98}, last={0,0};
	TIME=180;
	
	
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
	
	decompress(coulours_subBitmap, BG_GFX_SUB,  LZ77Vram);
	decompress(coulours_mainBitmap, BG_GFX,  LZ77Vram);
	
	oamInit(&oamMain, SpriteMapping_1D_128, false);
	oamInit(&oamSub, SpriteMapping_1D_128, false);
	
	initNum(Numbers, (u8*)Coulours_SpritesTiles,&oamSub);
	initColours(Colours, (u8*)Coulours_SpritesTiles,&oamMain);
	initColours(Colours_sub, (u8*)Coulours_SpritesTiles,&oamSub);
	initPlus(Plus,(u8*)Coulours_SpritesTiles, &oamMain);
	initCorrect(Correct_color, Correct_pos,Plus);
	
	dmaCopy(Coulours_SpritesPal, SPRITE_PALETTE, 512);
	dmaCopy(Coulours_SpritesPal, SPRITE_PALETTE_SUB, 512);
	
    /*init Sets*/
    for(i=0;i<NUMBER_OF_SETS;i++)
    {
      for(j=0;j<NUMBER_OF_SQUARES;j++)
      {
        initElement(&(Set[i][j]),point);
	    point.x+=31;
      }
	  point.x=64;
	  point.y-=31;
	  if(i==3) {point.y=161;}
    }
	point.x=61;
	point.y=147;
	srand(time(NULL));
	for(j=0;j<NUMBER_OF_TRUE_SQUARES;j++)
      {
        initElement(&(TrueSet[j]),point);
	    point.x+=33;
		TrueSet[j].n = rand()%8;
      }
	  TrueSet[3].n = 1;
	  
	timerStart(0, ClockDivider_1024, TIMER_FREQ_1024(1), timerCallBack2);
	while(1)
	{
	    touchRead(&touch);
		prev_touch = is_touch;
		if(touch.px==0 && touch.py==0)
		  is_touch=0;
		else {is_touch=1;}
		/*check_touch*/
		if(prev_touch==0) 
		{
		  flag=read_color(touch);
		  if(flag!=-1)
		  {
		    if(last.y<NUMBER_OF_SQUARES)
		      flag=prepare_squares(&Set[last.y][last.x],Colours, flag, &last);
		    else
		      flag=prepare_squares(&Set[last.y][last.x],Colours_sub, flag, &last);
			 
		    if(last.x==0)
		      win=checkSet(TrueSet,Set[last.y-1], Correct_color, Correct_pos);
		    
		  }
        }
		
		SCORE=TIME * 999/180;
		prepare_digits(SCORE,Points,Numbers,3);
		prepare_digits(RECORD,Record,Numbers,3);
		prepare_timer(TIME,Timer,Numbers); 
		/*print_sprites*/
		for(i=0;i<4;i++)
		{
		  for(j=0;j<NUMBER_OF_SQUARES;j++)
		  oamSet(&oamMain, j + i*NUMBER_OF_SQUARES, Set[i][j].x, Set[i][j].y,  0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
			Colours[Set[i][j].n].sprite_gfx_mem, -1, 0, Set[i][j].hide, 0, 0, 0);
		}
		for(i=4;i< NUMBER_OF_SETS ;i++)
		{
		  for(j=0;j<NUMBER_OF_SQUARES;j++)
		  oamSet(&oamSub, j + i*4, Set[i][j].x, Set[i][j].y,  0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
			Colours_sub[Set[i][j].n].sprite_gfx_mem, -1, 0, Set[i][j].hide, 0, 0, 0);
		}
		for(j=0;j<NUMBER_OF_TRUE_SQUARES;j++)
		  oamSet(&oamMain, NUMBER_OF_SQUARES * NUMBER_OF_SETS + j, TrueSet[j].x, TrueSet[j].y,  0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
			Colours[TrueSet[j].n].sprite_gfx_mem, -1, 0, TrueSet[j].hide, 0, 0, 0);/*TrueSet[j].hide*/
			
		for(j=0;j<4;j++)
		{
		  if(Correct_color[j].pos!=-1)
		  oamSet(&oamMain, NUMBER_OF_ALL_SQUARES + j, Correct_color[j].x, Correct_color[j].y,  0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
			Plus[0].sprite_gfx_mem, -1, 0, 0, 0, 0, 0);
		  else oamClearSprite(&oamMain,NUMBER_OF_ALL_SQUARES+j);
		  if(Correct_pos[j].pos!=-1)
		  oamSet(&oamMain, NUMBER_OF_ALL_SQUARES + 4 + j, Correct_pos[j].x, Correct_pos[j].y,  0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
			Plus[1].sprite_gfx_mem, -1, 0, 0, 0, 0, 0);
		  else oamClearSprite(&oamMain,NUMBER_OF_ALL_SQUARES + 4 +j);
		}
			
	    for(i=0;i<3;i++)
		oamSet(&oamSub, NUMBER_OF_ALL_NOT_DIGIT+i, Points[i].x, Points[i].y,  0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
			Points[i].sprite_gfx_mem, -1, 0, Points[i].hide, 0, 0, 0);
			
		for(i=0;i<3;i++)
		oamSet(&oamSub, NUMBER_OF_ALL_NOT_DIGIT + 3+ i, Record[i].x, Record[i].y,  0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
			Record[i].sprite_gfx_mem, -1, 0, Record[i].hide, 0, 0, 0);
         
        for(i=0;i<3;i++)
		oamSet(&oamSub, NUMBER_OF_ALL_NOT_DIGIT + 6 + i, Timer[i].x, Timer[i].y,  0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
			Timer[i].sprite_gfx_mem, -1, 0, Timer[i].hide, 0, 0, 0);
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
		if(flag==1 || win==1 || SCORE==0)
		    {
		       for(j=0;j<4;j++)
		         TrueSet[j].hide=0;
			   if(win==0) SCORE=0;
		         break;
		    }
	}
	/*print true_set*/
	for(j=0;j<4;j++)
		  oamSet(&oamMain, 36 + j, TrueSet[j].x, TrueSet[j].y,  0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
			Colours[TrueSet[j].n].sprite_gfx_mem, -1, 0, TrueSet[j].hide, 0, 0, 0);
	oamUpdate(&oamMain);
	oamUpdate(&oamSub);
	
	wait_click();
	oamClear(&oamMain,0,128);
	oamClear(&oamSub,0,128);
	oamUpdate(&oamMain);
	oamUpdate(&oamSub);
	return SCORE; 
}


