#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include "basketball.h"
#include "football.h"
#include "colors.h"
#include "menu.h"



int main(void) {
    int num;
	int Result[3]={0,0,0};
	int Stat, SCORE=0;
	int Money=0, Max_money=999999;
	while(1)
	{
      num=menu(Result,Money);
	  Stat=Result[num];
	  
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
	  if (Max_money - SCORE > Money) 
	    Money +=SCORE;
	  else 
	    Money = Max_money;
	}
	return 0;
}
