#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include "basketball.h"
#include "football.h"
#include "colors.h"
#include "menu.h"

int help_rat()
{
   touchPosition touch;
   int back=0;
   oamClear(&oamMain,0,128);
   oamClear(&oamSub,0,128);
   oamUpdate(&oamMain);
   oamUpdate(&oamSub);
   
   decompress(help_ratingBitmap, BG_GFX,  LZ77Vram);

   while(1)
   {
	  touchRead(&touch);
	  if(is_continue(touch)==1) break;
	  if(is_back(touch)==1) 
	  {
	    back=1;
		break;
	  }
   }
   return back;
}


int main(void){
    int num;
	int back=0;
	int Result[3]={0,0,0};
	int Stat=0, SCORE=0;
	int Money=0, Max_money=999999;
	
	while(1)
	{
      num=menu(Result,Money,1);
	  
	  if(num<=2)
	  {
	    Stat=Result[num];
		SCORE=0;
	    switch(num)
	    {
	      case 0:
		    SCORE = colors(Stat);
		    if (SCORE > Result[0]) Result[0]=SCORE;
		  break;
	      case 1:
		    SCORE = basketball(Stat);
		    if (SCORE > Result[1]) Result[1]=SCORE;
		  break;
		  case 2:
		    SCORE = football(Stat);
		    if (SCORE > Result[2]) Result[2]=SCORE;
		  break;
	    }
		Money = Money + SCORE;
	  }
	  else
	  { 
	     back = help_rat();
		 if(back==1) continue;
		 rating(Result);
		 break;
	  } 
	}
	
	while(1)
	{
      num=menu(Result,Money,0);
	  Stat=Result[num];
	  SCORE=0;
	  switch(num)
	  {
	      case 0:
		    SCORE = colors(Stat);
		    if (SCORE > Result[0]) Result[0]=SCORE;
		  break;
	      case 1:
		    SCORE = basketball(Stat);
		    if (SCORE > Result[1]) Result[1]=SCORE;
		  break;
		  case 2:
		    SCORE = football(Stat);
		    if (SCORE > Result[2]) Result[2]=SCORE;
		  break;
		  default:
		  break;
	  }
		Money = Money + SCORE;
	}
	return 0;
}
