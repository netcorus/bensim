
#include "menulinux03.h"

#define HLT 0b1000000000000000  // Halt clock
#define MI  0b0100000000000000  // Memory address register in
#define RI  0b0010000000000000  // RAM data in
#define RO  0b0001000000000000  // RAM data out
#define IO  0b0000100000000000  // Instruction register out
#define II  0b0000010000000000  // Instruction register in
#define AI  0b0000001000000000  // A register in
#define AO  0b0000000100000000  // A register out
#define EO  0b0000000010000000  // ALU out
#define SU  0b0000000001000000  // ALU subtract
#define BI  0b0000000000100000  // B register in
#define OI  0b0000000000010000  // Output register in
#define CE  0b0000000000001000  // Program counter enable
#define CO  0b0000000000000100  // Program counter out
#define J   0b0000000000000010  // Jump (program counter in)

#define NOP 0b00000000              //
#define LDA 0b00010000				//
#define ADD 0b00100000				//
#define SUB 0b00110000				//
#define STA 0b01000000				//
#define LDI 0b01010000				//
#define JMP 0b01100000				//
//#define     0b0111000
//#define     0b1000000
//#define     0b1001000
//#define     0b1010000
//#define     0b1011000
//#define     0b1100000
//#define     0b1101000
#define OUT 0b11100000			//
#define ALT 0b11110000			//
void mysleep (int sleepms);
int writea ();//write istruction on file program the virtual EEPROM A,B
int reada ();//read istruction on file program the virtual EEPROM A,B
void printContents();//print local
void program ();//write instruction  on local
int  decimalToBinary(int  decimalnum);
int microm(char regname[],int  data );
int wmemory (int adrs,uint8_t data);//RI
int rmemory (int adrs);//RO
int printlocal();//manage microistruzio
int exec();
int split16b(int data);
int split8b (int data);
void main_menu();
int steps(int va,int vb);
int nutoarray(int y,int number);
int head ();
int foot();
int readf();// read file prog
int writef();
int insertf();
int readmem();// read istruction value and memory and print them
int readprog();//read memory prog
int convertprogram(char insr[],int num);
int printdata();
void setcolor (int ForgC,int BackC);
int getdir ();
void FindFirstFile_dir(char strFile[10]);
void title ();
#ifdef _WIN32

#else
int file_select(const struct dirent *entry);// select file

#endif

int stop = 0;
int split [2];
uint8_t memory [16];
int local [256]= {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
uint8_t rega; //register a
uint8_t regb; //register b
uint8_t regsum;  //alu register
uint8_t regmem; //memory reg
uint8_t reginst; //instruction reg
uint8_t adrs ;
uint8_t bus ;
uint8_t datac ;
uint16_t instructions ;
int nstep; //steps counter
int ncount; // counter
int fcount;//file counter
int nprog = 0; //prog counter
int displayout;
int buffern =0;
int count=0;// cont of simulator program
char proginst [16][4] = {
	"LDA",
	"ADD",
	"OUT",
	"ALT",
	"NOP",
	"NOP",
	"NOP",
	"NOP",
	"NOP",
	"NOP",
	"NOP",
	"NOP",
	"NOP",
	"NOP",
	"MEM",
	"MEM",
};
int  progval [16]={14,15,0,0,0,0,0,0,0,0,0,0,0,0,23,34};
char file []="def.prg";
FILE * my_file;
char files [15][30];// array files name
char menulist [20][20];

struct threeNum
{
uint8_t n0, n1, n2, n3 ,n4 ,n5, n6 ,n7 ;
};
/*
int prog [] = {
    LDI|3, // 0
    STA|15, // 1
    LDI|0, // 2
    ADD|15, // 3
    OUT|0, // 4
    JMP|3, // 5
    NOP|0, // 6
    NOP|0, // 7
    NOP|0, // 8
    NOP|0, // 9
    NOP|0, // 10
    NOP|0, // 11
    NOP|0, // 12
    NOP|0, // 13
        0, // 14
        1, // 15
};
*/
uint16_t data[] = {
  MI|CO,  RO|II|CE,  0,      0,      0,         0, 0, 0,   // 0000 - NOP
  MI|CO,  RO|II|CE,  IO|MI,  RO|AI,  0,         0, 0, 0,   // 0001 - LDA
  MI|CO,  RO|II|CE,  IO|MI,  RO|BI,  EO|AI,     0, 0, 0,   // 0010 - ADD
  MI|CO,  RO|II|CE,  IO|MI,  RO|BI,  EO|AI|SU,  0, 0, 0,   // 0011 - SUB
  MI|CO,  RO|II|CE,  IO|MI,  AO|RI,  0,         0, 0, 0,   // 0100 - STA
  MI|CO,  RO|II|CE,  IO|AI,  0,      0,         0, 0, 0,   // 0101 - LDI
  MI|CO,  RO|II|CE,  IO|J,   0,      0,         0, 0, 0,   // 0110 - JMP
  MI|CO,  RO|II|CE,  0,      0,      0,         0, 0, 0,   // 0111
  MI|CO,  RO|II|CE,  0,      0,      0,         0, 0, 0,   // 1000
  MI|CO,  RO|II|CE,  0,      0,      0,         0, 0, 0,   // 1001
  MI|CO,  RO|II|CE,  0,      0,      0,         0, 0, 0,   // 1010
  MI|CO,  RO|II|CE,  0,      0,      0,         0, 0, 0,   // 1011
  MI|CO,  RO|II|CE,  0,      0,      0,         0, 0, 0,   // 1100
  MI|CO,  RO|II|CE,  0,      0,      0,         0, 0, 0,   // 1101
  MI|CO,  RO|II|CE,  AO|OI,  0,      0,         0, 0, 0,   // 1110 - OUT
  MI|CO,  RO|II|CE,  HLT,    0,      0,         0, 0, 0,   // 1111 - ALT
};


int main ()
{
    //program (); //write instruction  on local if no file eeprom
	setcolor(7,1);
	title ();
	readf(); // read prog from file and write on memory
	reada ();// read eeprom from file
	
	main_menu (); // menu chose


}

void title ()
{
sprintf (headf[0],"   ____________________________________________________________________________ \n");
sprintf (headf[1]," /|                                                                            |\n");
sprintf (headf[2],"| |   BEN COMPUTER SIMULATOR BY NETCORUS   VER 8_12wl                          |\n");
sprintf (headf[3],"| |____________________________________________________________________________|\n");

}

int exec(){
		nprog=0;
        nstep=0;
        displayout=0;
        rega=0;
        regb=0;
        regsum=0;
		ncount=0;
		stop=0;
   int busl=0;
   ncount=0;
    printlocal(); //display graph
    	while (stop != 1){
        // separe  intruction from data for read eeprom
        busl = rmemory (ncount); //read program instruction and write on busl local
        adrs = (busl-split8b(busl))>>1;// separe the address for read eeprom
        // end

    for (int x=0;x<= 4;x++)
    {
        nstep = x ;
        printlocal(); //display graph
        mysleep(2000);
        //for (int x=0 ;x==100000;x++){for (int Y=0 ;Y==100000;Y++){}}
    }
     nprog = ncount;
 }
//chosech();
return 0;
}

int steps(int va,int vb)
{
char namestepa [8][4] = {"hl","mi","ri","ro","io","ii","ai","ao",};
char namestepb [8][4] = {"eo","su","bi","oi","ce","co","j ","xx",};
int valuea [8] ={0,0,0,0,0,0,0,0,};
int valueb [8] ={0,0,0,0,0,0,0,0,};
int y=7 ;

for (int x=0;x<=7;x++)
{
    valuea [x]=nutoarray((y-x),(decimalToBinary(va)));
    valueb [x]=nutoarray((y-x),(decimalToBinary(vb)));
}
printf ("| | ");

for (int x=0;x<=7;x++)
{
       setcolor(7,1);
       printf("[%s]",namestepa[x]);

}
for (int x=0;x<=7;x++)
{
       setcolor(7,1); // 
       printf("[%s]",namestepb[x]);
       setcolor(7,1);

}
  printf("           |\n");
  printf ("| | ");

for (int x=0;x<=7;x++)
{
    if (valuea [x]==1)
    {
       setcolor(0,3);
       printf("[ 1]");
       setcolor(7,1);
    }
    else
    {
        setcolor(7,0);
        printf("[ -]");
        setcolor(7,1);
    }
}
 //   printf("                           |");
 //   printf ("| | ");
for (int x=0;x<=7;x++)
{
   if (valueb [x]==1)
    {
        setcolor(0,3);
       printf("[ 1]");
       setcolor(7,1);
    }
    else
    {
        setcolor(7,0);
        printf("[ -]");
        setcolor(7,1);
    }
}
    printf("           |\n");


//___________________________________________________________________________//

    if (valuea[0]==1) {stop = 1;} //NTL alt clock
    if (valuea[4]==1) {datac = split8b(reginst);bus = datac;}//IO Instruction register out 4bit
    if (valueb[5]==1) {bus=ncount;}//CO Program counter out
    if (valuea[1]==1) {regmem = bus ;}//MI Memory address register in
    if (valuea[7]==1) {bus = rega ;}//AO A register out
    if (valuea[2]==1) {memory[regmem]=bus;} //RI RAM data in
    if (valuea[3]==1) {bus =memory[regmem];}//RO RAM data out
    if (valuea[5]==1) {reginst = bus;}//II Instruction register in
    if (valueb[1]==1) {regb=regb*-1;}//SU ALU
    if (valueb[0]==1) {regsum = rega+regb;bus = regsum;}//EO ALU out
    if (valuea[6]==1) {rega = bus ;}//AI A register in
    if (valueb[2]==1) {regb = bus ;}//BI B register in
    if (valueb[3]==1) {displayout=regsum;}//0I Output register in
    if (valueb[4]==1) {ncount++;}//CE Program counter enable
    if (valueb[6]==1) {ncount = bus;}//J Jump (program counter in)
    if (valueb[7]==1) {}//XX
return 0;
}


void main_menu()
{
	int  int_choice = 0;
	fcount=0;
	 sprintf (menulist[fcount],"%s","Run simulator");fcount++;//0
	 sprintf (menulist[fcount],"%s","Load program");fcount++;//1
	 sprintf (menulist[fcount],"%s","Show  Microcode");fcount++;//2
	 sprintf (menulist[fcount],"%s","Show  EEprom");fcount++;//3
	 sprintf (menulist[fcount],"%s","Write EEprom");fcount++;//4
	 sprintf (menulist[fcount],"%s","Insert Program");fcount++;//5
	 sprintf (menulist[fcount],"%s","Show program");fcount++;//6
	 sprintf (menulist[fcount],"%s","Exit");fcount++;//7
	 sprintf (menulist[fcount],"%s","-------------");fcount++;//8
	 sprintf (menulist[fcount],"%s","-------------");fcount++;//9

		while((int_choice = menu(7,1,fcount)) != 10){
			
		switch (int_choice)
		{
			case 0:
	            exec();  // start istructions
	            getch();
	            main_menu();
				break;
			case 1:
				cls ();
                getdir ();
                system("cls");
                readf(); // read prog from file and write on memory
                readmem();
				getch();
				main_menu();
				break;
			case 2:
                cls ();
				head();
                printdata();
                foot();
                getch();
                main_menu();
				break;
			case 3:
				cls ();
				head();
				printContents();
				foot();
				getch();
                main_menu();
				break;
			case 4:
                cls ();
                program (); //write instruction  on local
                mysleep(5);
                writea (); //write istruction on file program the virtual EEPROM A,B
                reada ();// read eeprom
                printContents();
                getch();
                main_menu();
			        break;
			case 5:
                    cls ();
                    insertf();
                    writef();
                    readf();
                    readmem();
                   getch();
					main_menu();
			       break;
			case 6:
                cls ();
                    readmem();
                    getch();
                main_menu();
                    break;
			case 7:
                //	debug_hex_file(file_name);
                exit(0);
			        break;
			case 8:
			        //printf("Wrong choice. Enter Again");
			        exit(0);
           	        break;
           	        
           	case 9:
			        //printf("Wrong choice. Enter Again");
			        exit(0);
           	        break;

			default:
			//printf("Wrong choice. Enter Again");
			        break;
		}
	}
}






//write eeprom on screen from local[]
void printContents() {
  for (int base = 0; base <= 255; base += 16) {
    uint16_t data[16];

    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset]= local [base+offset] ;
    }
    //printf ("%03x %02x", base , data[base]);
    char buf[100];
    sprintf(buf,"| | %03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x                   |",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    printf ("%s \n",buf);
  }
}


//write instruction  on local
void program() {

  // Program data bytes
  printf("Programming EEPROM \n");

  // Program the 8 high-order bits of microcode into the first 128 bytes of EEPROM
  for (int address = 0; address < sizeof(data)/sizeof(data[0]); address += 1) {
	  //int local [255];
	  local[address] = data [address]>>8;
      printf("%03x : %04x ",address, data[address]);
      printf ("%02x\n",local[address]);
    }

  // Program the 8 low-order bits of microcode into the second 128 bytes of EEPROM
  for (int address = 0; address < sizeof(data)/sizeof(data[0]); address += 1) {
	  //int local [255];
      printf("%03x : %04x ", address + 128, data[address]);
      local[address+128] = data [address] & 0x00ff;
      printf("%02x\n", local[address+128]);
  }
printf (" done \n");
printf (" local contents \n");
//printContents ();

}


int writea()
{
   uint_fast8_t count=0;
   int n;
   struct threeNum num;
   FILE *fptr;

   if ((fptr = fopen("program.bin","wb")) == NULL){
       printf("Error! opening file");

       // Program exits if the file pointer returns NULL.
       exit(1);
   }

   for( n = 0; n <= 31; ++n)
   {
	  num.n0 = local [count] ;
	  num.n1 = local [count +1];
      num.n2 = local [count +2];
      num.n3 = local [count +3];
      num.n4 = local [count +4];
      num.n5 = local [count +5];
      num.n6 = local [count +6];
      num.n7 = local [count +7];
      count =  count +8 ;
      fwrite(&num, sizeof(struct threeNum), 1, fptr);

   }
   fclose(fptr);

   return 0;
}

//read istruction on file program the virtual EEPROM A,B
int reada()
{
   uint_fast8_t count=0;
   int n;
   struct threeNum num;

   //printf (" EEPROM contents \n");

   FILE *fptr;

   if ((fptr = fopen("program.bin","rb")) == NULL){
       printf("Error! opening file");
                cls();
                program (); //write instruction  on local
               // sleep(5);
                writea (); //write istruction on file program the virtual EEPROM A,B
                reada ();// read eeprom
                printContents();
               // sleep(1);
                main_menu ();
       // Program exits if the file pointer returns NULL.
       exit(1);
   }

   for(n = 1; n <= 32; ++n)
   {
      fread(&num, sizeof(struct threeNum), 1, fptr);

		 local[count]   = num.n0;
		 local[count+1] = num.n1;
		 local[count+2] = num.n2;
		 local[count+3] = num.n3;
		 local[count+4] = num.n4;
		 local[count+5] = num.n5;
		 local[count+7] = num.n6;
		 local[count+8] = num.n7;

	  count =count+8 ;
   }
   fclose(fptr);

   return 0;
}

int wmemory (int adrs,uint8_t data) // RI
{
    memory [adrs]= data ;

    //printf ("          ADR   VAL  \n");
    //printf ("Wmemory : %03x   %02x * ",adrs,memory [adrs]);

return 0 ;
	}

int rmemory (int adrs) // RO
{
     uint8_t data = memory [adrs] ;
    //printf ("          ADR   VAL  \n");
	//printf ("Rmemory : %03x   %02x * ",adrs,memory [adrs]);

return data ;
	}

int printlocal()

{
    char status [6];
    if (stop == 0){strcpy (status,"RUN ");}
    if (stop == 1){strcpy (status,"STOP");}
    cls ();
    head ();
   // printf ("| |                                                                            |\n");
    printf ("| |  MEMORY       >> ");
    for (int x=0;x<=15;x++)
        {printf (" %02x",memory[x]);}
    printf ("          |\n");
    printf ("| |  MICROINSTR   >> ");
    for (int x=0;x<=7;x++)
        {printf (" %02x%02x",local[adrs +x],local[adrs+x+128]);}
    printf ("                  |\n");
   // printf ("\n| |                                                                            |\n");
    printf ("| |  PROG : %02d %s %2d          STEP : %d %02x%02x ",nprog,proginst[nprog],progval[nprog],nstep,local[adrs+nstep],local [adrs+nstep+128]);
    printf (" %08d  %08d               |\n"  ,decimalToBinary  (local[adrs +  nstep]),decimalToBinary(local[adrs+128+nstep]));
    printf ("| |  MEM 14 = %03d MEM 15 = %03d  >>> DISPLAY [%03d] [%s]                       |\n",memory[14],memory[15],displayout,status);
    printf ("| |  __________________________________________________________                |\n");
    microm ("BUS       ",bus);
    microm ("REG  A    ",rega);
    microm ("REG  B    ",regb);
    microm ("REG SUMM  ",regsum);
    microm ("REG MEM   ",regmem);
    microm ("REG ISTR  ",reginst);
    microm ("DATA ISTR ",datac);
    printf ("| | |__________________________________________________________|               |\n");
   // printf("| |____________________________________________________________________________|\n");
    printf ("| |                                                                            |\n");
    steps((local[adrs +nstep]),(local[adrs +128+nstep]));// read eeprom and process data
    printf ("| |                                                                            |\n");
    printf ("| |____________________________________________________________________________|\n");
    printf ("|/____________________________________________________________________________/ \n");
    return 0;
}

int  decimalToBinary(int  decimalnum)
{
    uint64_t binarynum = 0;
    int rem, temp = 1;

    while (decimalnum!=0)
    {
        rem = decimalnum%2;
        decimalnum = decimalnum / 2;
        binarynum = binarynum + rem*temp;
        temp = temp * 10;
    }
    //printf ("%ld ", binarynum);
    return binarynum;
}




int split16b (int  data)
{

    split [0]  = data >>8;
    split [1] = data & 0x00ff; ;

    printf("data16 : %04x \n",data);
    printf ("local16 : %02x %02x\n",split [0] ,split[1] );

    return 0;
}

int split8b (int  data)
{
int splitl = 0;
    split [0]  = data >>4;
    //split [1] = data & 0x0f;
    splitl = data & 0x0f;
    //printf("data8 : %02x \n",data);
    //printf ("local8 : %01x %01x\n",split [0] ,split[1] );

    return splitl;
}

int microm(char regname[],int  data )
{

char str[10];
sprintf(str,"%08d",decimalToBinary(data));

//printf("| |  _____________________________________________________                     |\n");
printf("| | | %s ", regname);
for(int x=0;x<=7;x++)
{
if (str[x]== '0'){
printf("-(");
setcolor(4,0);
printf("%c",str[x]);
setcolor(7,1);
printf(")-");
}

else {
printf("-(");
setcolor(0,2);
printf("%c",str[x]);
setcolor(7,1);
printf(")-");
}
setcolor(7,1);
}
printf(" %03d ",data);
printf(" |               |\n");
//printf("| | |_____________________________________________________|                    |\n");
return 0;
}

int nutoarray(int y,int number)
{

    int c=0;
    int n=number;
    int numberarray[8] = {0,0,0,0,0,0,0,0,};
    while (n !=0)
    {
        numberarray[c] = n % 10;
        n /= 10;
        c++;
    }
     //printf("%d ",numberarray[y]);
     return numberarray[y];
}
int head ()
{
	for (int x =0 ;x<=3 ;x++ )
{
	printf ("%s",headf[x]);
}
/*
printf ("   ____________________________________________________________________________ \n");
printf (" /|                                                                            |\n");
printf ("| |   BEN COMPUTER SIMULATOR BY NETCORUS   VER 8_12wl                          |\n");
printf ("| |____________________________________________________________________________|\n");
*/

return 0;
}

int foot ()
{
    //printf("| |____________________________________________________________________________|\n");
    //printf("| |                                                                            |\n");
    //printf("| |                                                                            |\n");
    printf("| |____________________________________________________________________________|\n");
    printf("|/____________________________________________________________________________/ \n");
return 0;
}



int insertf()
{
    char chr[3]={0,0,0,};
    int value;
    
    printf("File name ?");
    scanf ("%s",file);
    for (int x=0;x<=15;x++)
{
cls ();

	printf("File : %s \n", file);
	
	for (int x=0;x<=15;x++)
		{
			printf     ("%d %3s %d\n",x,proginst[x],progval[x]);
		}
printf ("\n insert instruction %d : ",x);
scanf ("%s",chr);

for (int z=0;z<=2;z++)
{
    proginst [x][z] =chr[z];
    //printf ("\n chr : %d %c \n",z,chr[z]);

}
 printf ("proginst %d: %s \n",x,proginst[x]);
 for(int y=0;y<=2;y++){chr[y]=0;}

 printf ("insert value %d : ",x);
 scanf ("%d",&value);
 progval[x]=value;
 printf ("value : %d \n",value);

value=0;
}
return 0;
}

int writef () // write to file
{
my_file = fopen (file,"w");
for (int x=0;x<=15;x++)
{
    fprintf(my_file,"%3s %2d\n",proginst[x],progval[x]);
    printf     ("%d %3s %d\n",x,proginst[x],progval[x]);
}
fclose (my_file);
for (int x=0;x<= 15;x++) { wmemory (x,convertprogram(proginst[x],x));} //program on memory computer
return 0;
}

// read file prog
int readf ()
{
char chr[7]={0,0,0,0,0,0,0};
char chs;
int y=0;

if ((my_file = fopen(file,"r")) == NULL){
printf("Program not found please write new program\n");
printf("Program not found \n");
printf(" 1 write new program\n");
printf(" 2 default  program\n");
chs = getchar();
//chs = chosech ();
if (chs == '1'){insertf();writef();readf();readmem();}
if (chs == '2')          {}

writef();
getchar();getchar();
readf();
readmem();
reada ();// read eeprom from file
main_menu ();

}

//my_file = fopen (file,"r");
while (1)
{
for (int x=0;x<=6;x++)
{
    chr[x] = fgetc (my_file);
    //printf ("chr : %d %c \n",x,chr[x]);
}
if (feof(my_file) ){break;}
for (int x=0;x<=2;x++)
{
    //printf ("chr : %d %c \n",x,chr[x]);
    proginst [y][x] = chr[x];
}
if (chr[4]<='0')
{
    progval[y] = (chr[5]-'0');//unita
}
else
{
    progval[y] = ((chr[4]-'0')*10)+(chr[5]-'0');//decine+unita
}

//printf("progval %d : %d \n",y,progval[y]);
y++;


}
fclose (my_file);
for (int x=0;x<= 15;x++) { wmemory (x,convertprogram(proginst[x],x));} //program on memory computer

memory [14]=progval[14];
memory [15]=progval[15];
return 0;
}

int readprog() //read memory prog
{
    printf("\n Program \n");
    for  (int x =0;x<=15;x++)
{
	printf ("%2d %02x \n",x,memory[x]);
}
return 0;
}

int readmem()// read istruction value and memory and print them
{
    //char filel[6];
    //strcpy (filel,file);
    head();
    printf("| |    PROGRAM    %s                                        \n",file);
    for  (int x =0;x<=15;x++)
{
	printf("| | %2d %s %2d  %02x %08d                                                     |\n"
	,x,proginst[x],progval[x],memory[x],decimalToBinary(memory[x]));
}
foot();
return 0;
}

int convertprogram(char istr[],int num) // convert program to exadecimal number
{
 int convert=0;
if (strcmp(istr,"NOP") ==0)
{
    //printf ("LDI  %d",LDI);
    convert=NOP+progval[num];
}
if (strcmp(istr,"LDA") ==0)
{
    //printf ("LDI  %d",LDI);
    convert=LDA+progval[num];
}
if (strcmp(istr,"ADD") ==0)
{
    //printf ("LDI  %d",LDI);
    convert=ADD+progval[num];
}
if (strcmp(istr,"SUB") ==0)
{
    //printf ("LDI  %d",LDI);
    convert=SUB+progval[num];
}
if (strcmp(istr,"STA") ==0)
{
    //printf ("LDI  %d",LDI);
    convert=STA+progval[num];
}
if (strcmp(istr,"LDI") ==0)
{
    //printf ("LDI  %d",LDI);
    convert=LDI+progval[num];
}
if (strcmp(istr,"JMP") ==0)
{
    //printf ("LDI  %d",LDI);
    convert=JMP+progval[num];
}
if (strcmp(istr,"OUT") ==0)
{
    //printf ("LDI  %d",LDI);
    convert=OUT+progval[num];
}
if (strcmp(istr,"ALT") ==0)
{
    //printf ("LDI  %d",LDI);
    convert=ALT+progval[num];
}
//#define     0b0111000
//#define     0b1000000
//#define     0b1001000
//#define     0b1010000
//#define     0b1011000
//#define     0b1100000
//#define     0b1101000
return convert;
}

int printdata() //show eeprom from file
{
    //printf("\n");
    for (int x=0 ;x<=127;x+=8)
    {
       printf("| | %02x",x);
       for (int y=0 ;y<=7;y+=1)
       {
           printf(" %04x ",data[x+y]);
       }
        printf("                         |\n");
    }
    return 0;
}



int getdir ()
{
	int a=0;
	fcount=0;
	#ifdef  _WIN32
     
    char strPath[10] = "*.*";
    FindFirstFile_dir(strPath);
    for (int x =0 ;x<fcount ;x++ )
        {
           strcpy(menulist[x],files[x]);
        }
    a = menu(7,1,fcount);
    strcpy(file,files[a]);
    
 #else
		buffern=0;
        count=0;
  struct dirent **eps;
  int n;
  n = scandir ("./", &eps, file_select, alphasort);
  if (n >= 0)
    {
      int cnt;
      for (cnt = 0; cnt < n; ++cnt)
      {
        sprintf (files[cnt] ,"%s",eps[cnt]->d_name);
        //sprintf (buffer[buffern],"%d %s ",buffern,eps[cnt]->d_name);
        buffern++;
        count++;
        fcount = count ; // fcount is number of menu entry
	  }	
	    for (int x =0 ;x<n ;x++ )
        {
           strcpy(menulist[x],files[x]);
        }
    a = menu(7,1,fcount);
    strcpy(file,files[a]);
    }
  else
  {
	      perror ("Couldn't open the directory");
  }
 #endif   
return 0;
}

#ifdef _WIN32

#else
int file_select(const struct dirent *entry)
{
	
	
	char *ptr;
	
   if ((strcmp(entry->d_name, ".")==0 ||
   (strcmp(entry->d_name, ".."))==0))
   {
	   return (0);
	}
   ptr =strrchr (entry->d_name, '.');
   if ((ptr != NULL)&&
   ((strcmp(ptr,".prg")==0)))
   return (1);
   else
   return 0;
  
   
}
 #endif


void FindFirstFile_dir(char strFile[10])
{
	
	#ifdef _WIN32
    WIN32_FIND_DATAA    find;
    HANDLE    hFile;

    //printf("%s \n",strFile);
    int    nCount = 0;
    char filesl [15][30];// array files name
    char *ptr;
    fcount=0;

    if((hFile=FindFirstFile(strFile,&find)) != INVALID_HANDLE_VALUE)
    {
        //printf("L %d %s \n",nCount,find.cFileName);
        sprintf (filesl[nCount] ,"%s", find.cFileName);
        //printf("S %d %s \n",nCount,filesl[nCount]);
        nCount++;

        while(FindNextFile(hFile,&find))
        {
            //printf("L %d %s \n",nCount,find.cFileName);
            sprintf (filesl[nCount] ,"%s",find.cFileName);
            //printf("S %d %s \n",nCount,filesl[nCount]);
            nCount++;
        }
        FindClose(hFile);        // must close  the handle
        //for (int x=0;x <= nCount-1;x++){printf("filesL %d  %s \n",x,filesl[x]);}
    }
    else {printf("%s: file(s) not found\n",strFile);}

    for (int x=0;x <= nCount;x++){
            ptr =strrchr (filesl[x], '.');
           if ((strcmp(filesl[x], ".")==0 ||
            (strcmp(filesl[x], ".."))==0) ){
       //printf("deleted %d %s \n",x,filesl[x]);
     }
     else {

        if ((ptr != NULL)&&
        ((strcmp(ptr,".prg")==0))){
        sprintf (files[fcount] ,"%s",filesl[x]);
        //printf("loc %d %s \n",fcount,files[fcount]);
        fcount++;
        }
     }

    }
     #else
     //fcount--; //return 0;
     #endif
}


 



void mysleep (int sleepms)
{
	
	sleep(sleepms/1000);
}
