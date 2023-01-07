#include <string.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>//STDIN_FILENO
#include <errno.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#define cls() system("cls")
#else
#include <termios.h> //termios, TCSANOW, ECHO, ICANON
#define cls() system("clear")
#define getch()  chosech()
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <time.h> 
#endif

int chosearr();// getch linux 
int chosech ();// getch linux
int menu (int cx1 , int cx2,int xcount); // menu routine
int printm (int chose,int fcount); // print menu routine
void setcolor (int ForgC,int BackC); // color routine


char menulist [20][20]; // lista menu entry
char headf [5][100];
int fcount=0; // numero entry
int c1=7; //color menu default
int c2=1; //color menu default




// corpo routine menu 
int printm (int chose, int fcount)
{
cls();
setcolor(c1,c2);
// print title
for (int x =0 ;x<=3 ;x++ )
{
	printf ("%s",headf[x]);// title array
}
//print menu
printf ("| |   _________________________________                                        |\n");
printf ("| |  /|                                |                                       |\n");
printf ("| | |/|                                |                                       |\n");
    for (int x =0 ;x<fcount ;x++ )
        {
           if (x==chose){printf("| | |/|  ");setcolor(0,7);printf("< %-15s >",menulist[x]);setcolor(c1,c2);
			   printf("           |                                       |\n");}
            else {printf("| | |/|    %-15s             |                                       |\n",menulist[x]);}

        }
printf ("| | |/|                                |                                       |\n");
printf ("| | |/|________________________________|                                       |\n");
printf ("| | |/_________________________________/                                       |\n");
printf ("| |                                                                            |\n");
printf ("| |____________________________________________________________________________|\n");
printf ("|/____________________________________________________________________________/ \n");
    
        return 0;
}
int menu (int cx1 , int cx2,int xcount)
{
    int chose = 0;
    c1=cx1;
    c2=cx2;
    fcount=xcount;
	setcolor(c1,c2); 
    printm(chose,fcount);

int ar;
    while((ar=chosearr()) != 1)
    {  
#ifdef _WIN32			        
			 if (ar== 72){if (chose > 0){chose--;printm(chose,fcount);}}//UP 72
             if (ar== 80){if (chose < fcount-1 )chose++;printm(chose,fcount);}//DOWN 80
             if (ar== 13){break;}//ENTER 13
#else	
			 if (ar== 65){if (chose > 0){chose--;printm(chose,fcount);}}//UP 72
             if (ar== 66){if (chose < fcount-1 )chose++;printm(chose,fcount);}//DOWN 80
             if (ar== 10){break;}//ENTER 10
#endif
    }  
    return chose;
}



#ifdef _WIN32
int chosearr()
{

     int a,b;
     a = getch();
    if (a==224){b=getch();}
    else if (a==13){b=a;}
return b;
}
#else
int chosearr()
{

    int a,b =0;
    a = chosech();
    if (a==91){b=chosech();}
    else if (a==10){b=a;}
    
return b;
printf("b %d",b);

}
#endif

#ifdef _WIN32
int chosech ()
{
	char c;
	    while((c=getch() )!= 'q')
	    {
			if (c == 10){break;}
		}

    return c;
}
#else
int chosech ()
{
	int c;   
    static struct termios oldt, newt;
    
    /*tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to oldt*/
    tcgetattr( STDIN_FILENO, &oldt);
    /*now the settings will be copied*/
    newt = oldt;

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    //newt.c_lflag &= ~(ICANON); 
    newt.c_lflag &= ~(ICANON | ECHO);         
    /*chose new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    /*This is your part:
    I choose 'e' to end input. Notice that EOF is also turned off
    in the non-canonical mode*/
    while((c=getchar()) == ' ') //!=
    {
	putchar(c);
	}     
                         
	//printf ("c = %c \n",c);
    /*restore the old settings*/
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);


    return c;
}
#endif 


void setcolor (int ForgC,int BackC)
{
#ifdef _WIN32 
    int wColor = ((BackC & 0x0F)<< 4)+(ForgC & 0x0F);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),wColor);
    #else
// normal color
if (ForgC ==0){printf("\x1b[30m");} //black scrittura
if (ForgC ==1){printf("\x1b[34m");} //blue scrittura
if (ForgC ==2){printf("\x1b[32m");} //green scrittura
if (ForgC ==3){printf("\x1b[36m");} //light blue scrittura
if (ForgC ==4){printf("\x1b[31m");} //red scrittura
if (ForgC ==5){printf("\x1b[35m");} //violet scrittura
if (ForgC ==6){printf("\x1b[33m");} //yellow scrittura
if (ForgC ==7){printf("\x1b[37m");} //white scrittura
//special  color
if (ForgC ==8){printf("\x1b[37m");} //gray scrittura !!!
if (ForgC ==9){printf("\x1b[34m");} //blue scrittura !!!
if (ForgC ==10){printf("\x1b[32m");} //green scrittura !!!
if (ForgC ==11){printf("\x1b[36m");}//lightblue scrittura !!!
if (ForgC ==13){printf("\x1b[32m");}//green scrittura !!!
if (ForgC ==14){printf("\x1b[33m");}//Yellow scrittura !!!
if (ForgC ==15){printf("\x1b[37m");}//white scrittura !!!
// background color
// normal color
if (BackC ==0){printf("\x1b[40m");}  //back black
if (BackC ==1){printf("\x1b[44m");}  //back blue
if (BackC ==2){printf("\x1b[42m");}  //back green
if (BackC ==3){printf("\x1b[46m");}  //back lightblue
if (BackC ==4){printf("\x1b[41m");}	 //back red	
if (BackC ==5){printf("\x1b[45m");}	 //back violet
if (BackC ==6){printf("\x1b[43m");}	 //back yellow
if (BackC ==7){printf("\x1b[47m");}	 //back white

// special color
if (BackC ==8){printf("\x1b[47m");}  //back gray !!!
if (BackC ==9){printf("\x1b[44m");}  //back blue !!!
if (BackC ==10){printf("\x1b[42m");}  //back green !!!
if (BackC ==11){printf("\x1b[46m");}  //back lightblue !!!
if (BackC ==12){printf("\x1b[41m");}	//back red	!!!
if (BackC ==13){printf("\x1b[45m");}	//back violet !!!
if (BackC ==14){printf("\x1b[43m");}	//back yellow !!!
if (BackC ==15){printf("\x1b[47m");}	//back white !!!

// 30 black ,31 red, 32 green ,33 yellow, 34 blue ,35 violet ,36 light blue,37 white   ...write
// 40 black, 41 red ,42 green ,43 yellow  44 blue,	45 viole, 46 light blue ,47 white  ... back
//backc scrittura, forgc background
    //return ;
#endif    
}
