#include "menu.h"

int is_basket(touchPosition touch)
{
  if(touch.px>=102 && touch.py>=74 && touch.py<=145 && touch.px<=158)
  return 1;
  return 0;
}

int is_football(touchPosition touch)
{
  if(touch.px>=102+84 && touch.py>=74 && touch.py<=145 && touch.px<=158+84)
  return 1;
  return 0;
}

int is_color(touchPosition touch)
{
  if(touch.px>=102-84 && touch.py>=74 && touch.py<=145 && touch.px<=158-84)
  return 1;
  return 0;
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

int is_play(touchPosition touch)
{
  if(touch.px>=212 && touch.py>=161 && touch.py<=190 && touch.px<=245)
  return 1;
  return 0;
}

int is_man(touchPosition touch)
{
  if(touch.px>=133 && touch.py>=172 && touch.py<=188 && touch.px<=164)
  return 1;
  return 0;
}
void initNumber(Num *sprite, u8* gfx, OamState *oam, Point num)
{
    int i;
	sprite->sprite_gfx_mem = oamAllocateGfx(oam, SpriteSize_16x16, SpriteColorFormat_256Color);
	for(i=0;i<2;i++)
	dmaCopy(gfx+i*8*64 + 64*64 + num.x*8*16 + num.y*16*64, sprite->sprite_gfx_mem+i*4*16, 8*16);
}

void initNumbers(Num *sprites, u8* gfx, OamState *oam)
{
    Point num = {0,0};
	int i=0;
	for(num.y=0; num.y < 3; num.y++)
	  for(num.x=0; num.x < 4 || (num.y == 2 && num.x < 2); num.x++)
      {
	     initNumber(&(sprites[i]),gfx,oam,num);
		 sprites[i].n=i;
	     i++;
	  }
}

void init_border(Border *sprite, u8* gfx, OamState *oam)
{
    sprite->sprite_gfx_mem_top = oamAllocateGfx(oam, SpriteSize_64x64, SpriteColorFormat_256Color);
	sprite->sprite_gfx_mem_bot=oamAllocateGfx(oam, SpriteSize_64x64, SpriteColorFormat_256Color);
	dmaCopy(gfx, sprite->sprite_gfx_mem_top, 64*64);
	dmaCopy(gfx+128*64, sprite->sprite_gfx_mem_bot, 64*64);
}

void manual(int num)
{
   touchPosition touch;
   
   oamClear(&oamMain,0,128);
   oamClear(&oamSub,0,128);
   oamUpdate(&oamMain);
   oamUpdate(&oamSub);
   switch(num)
	  {
	    case 0:
		decompress(help_colorsBitmap, BG_GFX,  LZ77Vram);
		break;
	    case 1:
		decompress(help_basketballBitmap, BG_GFX,  LZ77Vram);
		break;
		case 2:
		decompress(help_footballBitmap, BG_GFX,  LZ77Vram);
		break;
	  }
	while(1)
	{
	  touchRead(&touch);
	  if(is_continue(touch)==1) break;
	}
    decompress(Menu_screenBitmap, BG_GFX,  LZ77Vram);
}

int menu(int *Result, int MONEY)
{
    int pos_rec=231, pos_mon=98;
	int RECORD=Result[1];
	int i=0;
	int num_game=1,prev_touch = 1,is_touch=0;
    Num Numbers[10];
	Num Record[3]={{pos_rec,45,0,0},{pos_rec-12,45,0,1},{pos_rec-24,45,0,1}};
	Num Money[6]={{pos_mon,45,0,0},{pos_mon-12,45,0,1},{pos_mon-24,45,0,1},{pos_mon-36,45,0,0},{pos_mon-48,45,0,1},{pos_mon-60,45,0,1}};
	Border BORDER = {98,70};
	touchPosition touch;
	
    powerOn(POWER_ALL_2D);
	lcdMainOnBottom(); /* Place the main screen on the bottom physical screen*/
	videoSetMode(MODE_5_2D | /* Enable BG3 for display*/
                 DISPLAY_SPR_ACTIVE | /* Enable sprites for display*/
                 DISPLAY_SPR_1D );
    videoSetModeSub(MODE_5_2D);
	
	
    bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0); 

	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG);
	
	decompress(Menu_screenBitmap, BG_GFX,  LZ77Vram);
	decompress(Menu_screen_subBitmap, BG_GFX_SUB,  LZ77Vram);
	
	oamInit(&oamMain, SpriteMapping_1D_128, false);
	oamInit(&oamSub, SpriteMapping_1D_128, false);
	
	initNumbers(Numbers, (u8*)menu_spritesTiles,&oamMain);
	init_border(&BORDER, (u8*)menu_spritesTiles,&oamMain);
	
	dmaCopy(menu_spritesPal, SPRITE_PALETTE, 512);
	dmaCopy(menu_spritesPal, SPRITE_PALETTE_SUB, 512);
	
	while(1)
	{
	   touchRead(&touch);
	   scanKeys();
	   
	   
	   if (keysDown()&KEY_RIGHT) {if(BORDER.x!=98+84){BORDER.x+=84;num_game++;}}
       if (keysDown()&KEY_LEFT) {if(BORDER.x!=98-84){BORDER.x-=84;num_game--;}}
	   
	   prev_touch = is_touch;
		if(touch.px==0 && touch.py==0)
		  is_touch=0;
		else 
		  is_touch=1;
	   if(prev_touch==0)
	   {
	     if(is_basket(touch)==1) {BORDER.x=98;num_game=1;}
	     if(is_football(touch)==1) {BORDER.x=98+84;num_game=2;} 
         if(is_color(touch)==1) {BORDER.x=98-84;num_game=0;} 
		 if(is_play(touch)==1 || keysDown()&KEY_A) break;
	     if(is_man(touch)==1 || keysDown()&KEY_B) {manual(num_game);}
	   }
	   RECORD=Result[num_game];
	   prepare_digits(RECORD,Record,Numbers,3);
	   prepare_digits(MONEY, Money, Numbers,6);
	   
	   for(i=0;i<6;i++)
		oamSet(&oamMain, i+5, Money[i].x, Money[i].y,  0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
			Money[i].sprite_gfx_mem, -1, 0, Money[i].hide, 0, 0, 0);
	   for(i=0;i<3;i++)
		oamSet(&oamMain, i+2, Record[i].x, Record[i].y,  0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
			Record[i].sprite_gfx_mem, -1, 0, Record[i].hide, 0, 0, 0);
	   oamSet(&oamMain, 0, BORDER.x, BORDER.y,  0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, 
			BORDER.sprite_gfx_mem_top, -1, 0, 0, 0, 0, 0);
	   oamSet(&oamMain, 1, BORDER.x, BORDER.y+64,  0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, 
			BORDER.sprite_gfx_mem_bot, -1, 0, 0, 0, 0, 0);
	  
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}
	oamClear(&oamMain,0,128);
	oamClear(&oamSub,0,128);
	oamUpdate(&oamMain);
	oamUpdate(&oamSub);
	return num_game;
}
