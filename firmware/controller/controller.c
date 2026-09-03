#include <18f4550.h>
#fuses INTRC,NOWDT,NOPROTECT,NOLVP,CPUDIV1,PLL1
#use delay(clock=8M)
#use rs232(rcv=pin_c7, xmit=pin_c6, baud=9600, bits=8, parity=n)
#include <string.h>
#include <stdlib.h>

#BYTE TRISA = 0xF92
#BYTE TRISC = 0xF94//ENTRADAS/SALIDAS
#BYTE PORTA = 0xF80
#BIT A2=0xF80.0
#BYTE PORTC = 0xF82
#BIT C0 = 0xF82.0

char M[10]={""};
char v0[]="@ORIGEN";
char v1[]="@NORTE";
char v2[]="@ESTE";
char v3[]="@SUR";
char v4[]="@OESTE";

char v9[]="@TURBO";

char X1[]="@FAST1",X2[]="@FAST2",X3[]="@FAST3",X4[]="@FAST4",X5[]="@FAST5",X6[]="@FAST6",X7[]="@FAST7",X8[]="@FAST8",X9[]="@FASTX";

void imprimir();

#BYTE ADCON0 = 0xFC2 //REGISTROS PARA ADC
#BYTE ADCON1 = 0xFC1
#BYTE ADCON2 = 0xFC0
#BYTE ADRESL = 0xFC3//Registro bajo del resultado A/D
#BYTE ADRESH = 0xFC4//Registro alto del resultado A/D

#BIT  ADCXD  = 0xFC2.2// AN0 Y AN1
#BIT  ADCXD2  = 0xFC2.3// AN2

int32 adc1, adc2, adc3, vel;//adc

void main()
         {
   
         TRISA = 0x07;//0b00000011;// AN1 AN0 ENTRADAS
         TRISC = 0x81;
   
         ADCON0 = 0b00000011;
         //BIT5-2 Bits selectores del canal analógico        (AN0) 0000 = canal 0 (AN0) 
         //BIT1 Bit A/D del estado de la conversión          1 = conversión A/D en marcha
         //BIT0 Bit de permiso del A/D                       1 = módulo A/D permitido
         ADCON1 = 0b00001100;
         //BIT5 Bit de configuración de la tensión de referencia   0 = VSS 0V
         //BIT4 Bit de configuración de la tensión de referencia   1 = VREF+ (AN3) 5V
         //BIT3-0 Bits de configuración de los puertos             A/D: ANO=A 
         ADCON2 = 0b10001000;
         //BIT7 Bit selecto del formato A/D del resultado       1 = derecho justificado 
         //BIT5-3 Bits selectores del tiempo de adquisición     001 = 2 TAD
         //BIT2-0 Bits selectores del reloj de conversión       000= FOSC/2
         delay_us(10);
  
         While(true)
         {
            //ADCON0 |= 0b00000010;      //1 = conversión A/D en marcha
            //ADCXD=0;//ANO
            //ADCXD2=0;
            ADCON0 = 0b00000011;
            delay_us(10);
            adc1=ADRESL;
            adc1 |= ((int32)ADRESH<<8);
            //printf("ADC1: %lu\n\r",adc1);
            //CCPR1H=adc1;
            //CCPR1L=adc1>>2;
         
            //ADCON0 |= 0b00000010;      //1 = conversión A/D en marcha
            //ADCXD=1;//AN1
            //ADCXD2=0;
            ADCON0 = 0b00000111;
            delay_us(10);
            adc2=ADRESL;
            adc2 |= ((int32)ADRESH<<8);
            //printf("ADC2: %lu\n\r\n\r",adc2);
            //CCPR1H=adc2;
            //CCPR2L=adc2>>2;
            
            //ADCON0 |= 0b00000010;      //1 = conversión A/D en marcha
            //ADCXD=0;//AN1
            //ADCXD2=1;//AN1
            
            ADCON0 = 0b00001011;
            delay_us(10);
            adc3=ADRESL;
            adc3 |= ((int32)ADRESH<<8);
            
            if(adc3 != vel)
            {
               vel=adc3;
               
               if(adc3 <100)
               {
                  strcpy(M,X1);
                  imprimir();
               }
            
               else if(100<=adc3 && adc3 <200)
               {
                  strcpy(M,X2);
                  imprimir();
               }
            
               else if(200<=adc3 && adc3 <300)
               {
                  strcpy(M,X3);
                  imprimir();
               }
            
               else if(300<=adc3 && adc3 <400)
               {
                  strcpy(M,X4);
                  imprimir();
               }
            
               else if(400<=adc3 && adc3 <500)
               {
                  strcpy(M,X5);
                  imprimir();
               }
            
               else if(500<=adc3 && adc3 <600)
               {
                  strcpy(M,X6);
                  imprimir();
               }
            
               else if(600<=adc3 && adc3 <700)
               {
                  strcpy(M,X7);
                  imprimir();
               }
            
               else if(700<=adc3 && adc3 <800)
               {
                  strcpy(M,X8);
                  imprimir();
               }
            
               else if(900<=adc3)
               {
                  strcpy(M,X9);
                  imprimir();
               }
            }
            
            else if((200<=adc1 && adc1 <=730) && (650<=adc2))//ESTE
            {
               memset(M,0x00,10);
               strcpy(M,v2);
               imprimir();
            }
            else if((200<=adc1 && adc1 <=730) && (adc2 <=350))//OESTE
            {
               memset(M,0x00,10);
               strcpy(M,v4);
               imprimir();
            }
            
            else if(730<=adc1)//NORTE
            {
               memset(M,0x00,10);
               strcpy(M,v1);
               imprimir();
            }
            else if(adc1 <=200)//SUR
            {
               memset(M,0x00,10);
               strcpy(M,v3);
               imprimir();
            }

            else
            {
               memset(M,0x00,10);
               strcpy(M,v0);
               imprimir();
            }
         }
      }
      
void imprimir()
{
   for(int i=0; i<10; i++)
      {
         printf("%c",M[i]);
         delay_us(10);
      }
         delay_ms(10);
      
      printf("\r\n");
      
      //printf("%lu\t\t%lu\r\n",adc1,adc2);
      //delay_ms(200);
}
