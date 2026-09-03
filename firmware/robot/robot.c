#include <18f2550.h>
#fuses INTRC,NOWDT,NOPROTECT,NOLVP,CPUDIV1,PLL1
#use delay(clock=8M)
#use rs232(rcv=pin_c7, xmit=pin_c6, baud=9600, bits=8, parity=n)
#include <string.h>

#BYTE TRISB = 0xF93
#BYTE TRISC = 0xF94//ENTRADAS/SALIDAS

#BYTE PORTB = 0xF81

#BYTE CCP1CON=0xFBD //REGISTRO DE CONTROL ESTÁNDAR DE CCP1
#BYTE CCP2CON=0xFBA //REGISTRO DE CONTROL ESTÁNDAR DE CCP2
#BYTE CCPR1H=0xFBF
#BYTE CCPR1L=0xFBE 
#BYTE CCPR2H=0xFBC 
#BYTE CCPR2L=0xFBB
#BYTE PR2=0xFCB //El periodo se especifica escribiendo el registro PR2

#BYTE T2CON =0xFCA//permitir el Timer2
#BYTE TMR2=0xFCC//valor del prescaler TMR2

//Periodo _ PWM = [(PR + • 4]1)2 •TOSC • (Valor _ del _ presaler _ de _ TMR )
//PWM = (PR2 +1)*4*Tosc*Prescaler
//1/1KHz = (PR2 +1)*4*(1/4MHz)*4
//PR2=249   PR2=F9

//CCPRXL=PWM/Tosc*Prescaler
//(1/1KHz)/(1/4MHz)*4 =1
//1000->100% 

char PM[10];
char b=0x00;
int bu=0x00, vel=0;
int bandera=0;

char v0[]="@ORIGEN";
char v1[]="@NORTE";
char v2[]="@ESTE";
char v3[]="@SUR";
char v4[]="@OESTE";

char v9[]="@TURBO";

char X1[]="@FAST1",X2[]="@FAST2",X3[]="@FAST3",X4[]="@FAST4",X5[]="@FAST5",X6[]="@FAST6",X7[]="@FAST7",X8[]="@FAST8",X9[]="@FASTX";

#int_rda
void rcv_data()
{
   if(kbhit())
   {
      b=getc();
      
      if(b=='@')
      {
         bu=0;
         bandera=1;
      }
      if(bu>=10)
      {
         memset(PM,'\0',10);
         bu=0;
      }
      else
      PM[bu++]=b;
   }
}

void c_mueve();

void main()
{
   enable_interrupts(int_rda);
   enable_interrupts(global);
   
   //PR2=0xF9;
   PR2=249;
   
   TRISB = 0X00;
   TRISC = 0x80;
   
   T2CON = 0x05;//T2CON = 0b00000101 //NO POSTSCALE / SI TIMER2 / PRESCALER DE  4
   
   CCP1CON = 0x0C;//0b00001100;11xx = modo PWM
   CCP2CON = 0x0C;//11xx = modo PWM
   
   TMR2 =4;//PRESCALER DE 4
   
   while(true)
   {
      c_mueve();
      CCPR1L=vel;
      CCPR2L=vel;
       /*
         if(bandera==1)
         {
         for(int i=0; i<=10; i++)
         {
            printf("%c",PM[i]);
            delay_ms(1);
         }
         bandera=0;
         delay_us(100);
         printf("\r\n");
         }*/
         
   }
}

void c_mueve()
{
   if(strstr(PM,v1)!=NULL)//NORTE
   {
      PORTB=0x15;//1001
   }
   
   else if(strstr(PM,v2)!=NULL)//ESTE
   {
      PORTB=0x16;//1010
   }
   
   else if(strstr(PM,v3)!=NULL)//SUR
   {
      PORTB=0x0E;//0110
   }
   else if(strstr(PM,v4)!=NULL)//OESTE
   {
      PORTB=0x0D;//0101
   }
   else if(strstr(PM,v9)!=NULL)//TURBO
   {
      vel=249;
   }
   
   else if(strstr(PM,X1)!=NULL)//TURBO
   {
      vel=0;
   }
   
   else if(strstr(PM,X2)!=NULL)//TURBO
   {
      vel=25;
   }
   
   else if(strstr(PM,X3)!=NULL)//TURBO
   {
      vel=50;
   }
   
   else if(strstr(PM,X4)!=NULL)//TURBO
   {
      vel=75;
   }
   
   else if(strstr(PM,X5)!=NULL)//TURBO
   {
      vel=100;
   }
   
   else if(strstr(PM,X6)!=NULL)//TURBO
   {
      vel=125;
   }
   
   else if(strstr(PM,X7)!=NULL)//TURBO
   {
      vel=150;
   }
   
   else if(strstr(PM,X8)!=NULL)//TURBO
   {
      vel=175;
   }
   
   else if(strstr(PM,X9)!=NULL)//TURBO
   {
      vel=249;
   }
   
   else if(strstr(PM,v0)!=NULL)//NADA
   {
      PORTB=0x00;
   }
}










