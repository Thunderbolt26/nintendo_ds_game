#include <nds.h>
#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "rating.h"

Wifi_AccessPoint* findAP(void){

    int selected = 0, i=0, count = 0, displaytop = 0, displayend, pressed = 0;
	static Wifi_AccessPoint ap;

	Wifi_ScanMode(); /*this allows us to search for APs*/
	do 
	{
	   scanKeys();
		/*find out how many APs there are in the area*/
	   count = Wifi_GetNumAP();
	   consoleClear();
       iprintf("%d APs detected\n\n", count);

	   displayend = displaytop + 10;
	   if (displayend > count) displayend = count;

		/*display the APs to the user*/
	   for(i = displaytop; i < displayend; i++) 
	   {
		  Wifi_AccessPoint ap_i;

		  Wifi_GetAPData(i, &ap_i);

		/* display the name of the AP*/
		  iprintf("%s %.29s\n  Wep:%s Sig:%i\n", 
				i == selected ? "*" : " ", 
				ap_i.ssid, 
				ap_i.flags & WFLAG_APDATA_WEP ? "Yes " : "No ",
				ap_i.rssi * 100 / 0xD0);

	   }

	   pressed = keysDown();
	   /*move the selection asterick*/
	   if(pressed & KEY_UP)
       {
		  selected--;
		  if(selected < 0) 	selected = 0;
		  if(selected<displaytop) displaytop = selected;
	   }

		if(pressed & KEY_DOWN) 
		{
			selected++;
			if(selected >= count) selected = count - 1;
			displaytop = selected - 9;
			if (displaytop<0) displaytop = 0;
		}
		swiWaitForVBlank();
	} while(!(pressed & KEY_A));

	/*user has made a choice so grab the ap and return it*/
	Wifi_GetAPData(selected, &ap);

	return &ap;
}

void keyPressed(int c){
	if(c > 0) iprintf("%c",c);
}

void getHttp(const char* request_text);


void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = (int)strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }



 void UIntegertoStr(int n,char s[])
 {
     int i=0;
     do {       
         s[i++] = n % 10 + '0';   
     } while ((n /= 10) > 0);    
	 s[i]='\0';
     reverse(s);
 }
 
char* stringRequest(int Record[], char * username)
{
   char Rec0[7], Rec1[7], Rec2[7];
   char * text1 = "GET /index2.php?username=";
   char * text2 = "&score0=";
   char * text3 = "&score1=";
   char * text4 = "&score2=";
   char * text5 = " HTTP/1.1\r\n";
   char * text6 = 
        "Host: 192.168.43.126\r\n"
        "User-Agent: Nintendo DS\r\n\r\n";
   char * req;
   
   UIntegertoStr(Record[0],Rec0);
   UIntegertoStr(Record[1],Rec1);
   UIntegertoStr(Record[2],Rec2);
   
   req = (char *)malloc(strlen(text1) + strlen(text2) + strlen(text3) + strlen(username) 
   + strlen(text4) + strlen(text5) + strlen(text6) + strlen(Rec0) + strlen(Rec1) + strlen(Rec2) + 1);
    
   strcat(req,text1);
   strcat(req,username);
   strcat(req,text2);
   strcat(req,Rec0);
   strcat(req,text3);
   strcat(req,Rec1);
   strcat(req,text4);
   strcat(req,Rec2);
   strcat(req,text5);
   strcat(req,text6);

   return req;
}
void getHttp(const char* request_text) 
{
    int my_socket;
	int res;
	int recvd_len;
    char incoming_buffer[256];
	struct sockaddr_in sain;
    struct hostent * myhost = gethostbyname("192.168.43.126");
    iprintf("Found IP Address!\n");
 
    /* Create a TCP socket*/
    
    my_socket = socket( AF_INET, SOCK_STREAM, 0 );
    iprintf("Created Socket!\n");
    
    /* Tell the socket to connect to the IP address we found, on port 80 (HTTP)*/
    sain.sin_family = AF_INET;
    sain.sin_port = htons(80);
    sain.sin_addr.s_addr= *( (unsigned long *)(myhost->h_addr_list[0]) );
    res=connect( my_socket,(struct sockaddr *)&sain, sizeof(sain) );
	if(res == SOCKET_ERROR) {
	iprintf("Error.Didn't connect to server!\n");
        return;
    }
	
    iprintf("Connected to server!\n");
    
    /*send our request*/
    send( my_socket, request_text, strlen(request_text), 0 );
    iprintf("Sent our request!\n");

    /* Print incoming data*/
    iprintf("Printing incoming data:\n");

    while( ( recvd_len = recv( my_socket, incoming_buffer, 255, 0 ) ) != 0 ) { /*if recv returns 0, the socket has been closed.*/
        if(recvd_len>0) { /*data was received*/
            incoming_buffer[recvd_len] = 0; /* null-terminate*/
            iprintf(incoming_buffer);
		}
	}

	iprintf("Other side closed connection!");

	shutdown(my_socket,0); 
    
	closesocket(my_socket); // remove the socket.
}


int rating(int Record[])  
{
	
	int status,wepmode,len, quit, pressed, change_info=0;
	/*int lic=2;*/
	Keyboard* kb;
	char wepkey[64];
	const char* request_text;
	char * username;
	char * str1= "Enter Your Name\n";
	u32 ip;
	PrintConsole pcSub;
	
	lcdMainOnTop(); 
    videoSetModeSub(MODE_5_2D);
	videoSetMode(MODE_0_2D);
	
    consoleInit(&pcSub,0, BgType_Text4bpp, BgSize_T_256x256, 4,0, true, true);
	consoleSetWindow(NULL, 0,0,32,24);
	Wifi_InitDefault(0);
	kb = keyboardInit(0,1,BgType_Text4bpp, BgSize_T_256x256, 3,1,false,true);
	kb->OnKeyPressed = keyPressed;

	while(1) 
	{
	  status = ASSOCSTATUS_DISCONNECTED;
	  consoleClear();
	  len = strlen(str1);
	  iprintf("\x1b[0;0H\x1b[K");
	  iprintf("\x1b[0;%dH%s", (32-len)/2,str1);
	  scanf("%s",username);
	  if(strcmp(username,"")==0)
		continue;
      Wifi_AccessPoint* ap = findAP();
      consoleClear();
	  iprintf("Press A to continue, B to quit, X to change info");
	  quit=0;
	  change_info=0;
	  while(1) {
			swiWaitForVBlank();
			scanKeys();
			pressed = keysDown();
			if(pressed&KEY_B) quit = 1;
			if(pressed&KEY_X) change_info=1;
			if(pressed&(KEY_A|KEY_B|KEY_X)) break;
	  }
	  if(quit) break;
	  if(change_info) continue;
      iprintf("Connecting to %s\n", ap->ssid);
      consoleClear();
		/*this tells the wifi lib to use dhcp for everything*/
	  Wifi_SetIP(0,0,0,0,0);	
	  
	  wepmode = WEPMODE_NONE;
	  if (ap->flags & WFLAG_APDATA_WEP) {
			iprintf("Enter Wep Key\n");
			while (wepmode == WEPMODE_NONE) {
				scanf("%s",wepkey);
				if (strlen(wepkey)==13) {
					wepmode = WEPMODE_128BIT;
				} else if (strlen(wepkey) == 5) {
					wepmode = WEPMODE_40BIT;
				} else {
					iprintf("Invalid key!\n");
				}
			}
	        Wifi_ConnectAP(ap, wepmode, 0, (u8*)wepkey);
		} else {
			Wifi_ConnectAP(ap, WEPMODE_NONE, 0, 0);
			
		}
		
		while(status != ASSOCSTATUS_ASSOCIATED && status != ASSOCSTATUS_CANNOTCONNECT) {

			status = Wifi_AssocStatus();
			len = strlen(ASSOCSTATUS_STRINGS[status]);
			iprintf("\x1b[0;0H\x1b[K");
			iprintf("\x1b[0;%dH%s", (32-len)/2,ASSOCSTATUS_STRINGS[status]);

			scanKeys();

			if(keysDown() & KEY_B) break;
			
			swiWaitForVBlank();
		}

		if(status == ASSOCSTATUS_ASSOCIATED) {
			ip = Wifi_GetIP();
			iprintf("\nip: [%lu.%lu.%lu.%lu]\n", (ip ) & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, (ip >> 24) & 0xFF);
			iprintf("Connected\n\n");
			request_text=stringRequest(Record,username);
            getHttp(request_text);
				swiWaitForVBlank();
			
		} else {
			iprintf("\nConnection failed!\n");
		}
        quit = 0;
		iprintf("Press A to try again, B to quit.");
		while(1) {
			swiWaitForVBlank();
			scanKeys();
			pressed = keysDown();
			if(pressed&KEY_B) quit = 1;
			if(pressed&(KEY_A|KEY_B)) break;
		}
		if(quit) break;
	}
	return 0;
}


