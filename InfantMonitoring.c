#include<reg51.h>
#include <stdio.h>

//#define dataport P2;    // LCD Data pins = P2
#define adc_input P0;    // Data pins of ADC =P0
 
sbit PIR= P1^0;  //P1.0 – PIR sensor
sbit SOUND = P1^1; //P1.1 - sound sensor (LM393) to detect cry

sbit LED=  P3 ^7; //P3.7 - LED to show temp not in range


//LCD
sbit rs = P3^0;    //RS=P3.0
sbit rw = P3^1;   //R/W =P3.1
sbit e = P3^2;    //Enable= P3.2

 
//ADC 0804
sbit rd= P3^4;  //Read
sbit wr= P3^3;   //Write or Start conversion
sbit intr= P3^5;  //Interrupt or End of conversion


//FUNCTION DEFINITIONS

void delay();    // Delay
void lcd_cmd(unsigned char);   //command write
void lcd_data(unsigned char); //data write 
void lcd_data_string(unsigned char*); // writing the string
void lcd_init();   // initialisation ..01,06,etc
void adc_conv();   // adc commands
void adc_read();  // read data after conversion
void lcd_data_adc(unsigned int);
void check_tmp(unsigned int);

int num[10];

unsigned int value;
unsigned int motion;

void main(void)
{
	
	 //INPUTS
		PIR=1;
		SOUND=1; 
		P0 = 0xFF ;   //p0=input 
	
		//OUTPUTS
		LED=0;
		rs=0;
		rw=0;
		e=0;	
		P2 = 0x00 ;    //p2= output		
		//P3 = 0x00;
	
	while(1)
	{
		

		lcd_init();
		//lcd_data_string("TEMPERATURE=");
		adc_conv();
		adc_read();		
		motion=PIR;
		check_tmp(value);
		lcd_data('C');
		lcd_data(' ');
		
			
		delay();
 
        if(PIR == 0) 
					{
            //lcd_cmd(0xc0);
            //lcd_data_string("No Motion");
            
        } 
				else
					{
            //lcd_cmd(0xc0);
            lcd_data_string("Motion");
						        }
					
				
				
					if(SOUND == 0) 
					{
            //lcd_cmd(0xc0);
            //lcd_data_string("");
						
            
        } 
				else
					{
            lcd_cmd(0xc0);
            lcd_data_string("CRY");
						
        }
				
				delay();

				
				
				
    }
}


void check_tmp(unsigned int i)
{ 
	int p;
	int k;
	int tmp;
	
	k=0;
	
	while(i>0)
	{
		num[k]=i%10;
		i=i/10;
		k++;
	}
	k--;
	
	tmp=0;
	
	for (p=k;p>=0;p--)
	{
		tmp= (tmp*10)+(num[p]);
	}

	
		if (tmp >= 37 && tmp <= 40)
			{ 
					//printf ("LED OFF\n");
				
					LED = 0;
				delay();
			}

			else
			{
				//printf ("LED ON\n");
				
				LED=1;
				delay();
				delay();

			}
		
			
}


void lcd_init()  //LCD initialisation
{
lcd_cmd(0x01); //clear screen
delay();
lcd_cmd(0x06);  //increment cursor
delay();
lcd_cmd(0x0e); //Display on , cursor blinking off
delay();
lcd_cmd(0x38); //use two lines of lcd
delay();
lcd_cmd(0x80);//force cursor to beginning of first line
delay();
}

void lcd_cmd(unsigned char item)   // command write
{
P2 = item;   
rs= 0;    //RS=0
rw=0;  //R/W=1
e=1;    //Enable goes from high to low
delay();
e=0;
return;
}

void delay()   //DELAY FUNCTION
{
    unsigned int i;
    for(i=0;i<20000;i++);
}

void lcd_data(unsigned char item)  //Data write 
{
			P2 = item;  
			rs= 1;  // RS=1 implies write Data
			rw=0;  
			e=1;
			delay();
			e=0;
}

void lcd_data_string(char *x)  //STRING TO WRITE
{
    while(*x!='\0')
    {
        lcd_data(*x);
        delay();
        x++;
    }
}


void adc_conv()   //ADC 
{                      
	wr = 0;     //WR=0 initially
	delay();                     
	wr = 1;         //  WR=1 means start of conversion           
	while(intr);  //checking if intr=0
	delay();
	intr=1;    // change intr to 1 to indicate conversion has ended
}



void adc_read()
{
		                 
		rd = 0;
		delay();
		value=adc_input;
		delay();
		rd=1; 
		
		lcd_data_adc(value);
		
		//return value;
}





void lcd_data_adc(unsigned int i)   //Converting digital Data to ASCII to read to the LCD
{
	int p;
	int k=0;
	while(i>0)
	{
		num[k]=i%10;
		i=i/10;
		k++;
	}
	k--;
	
	for (p=k;p>=0;p--)
	{
		P2 = (num[p]+48);
		rw = 0;
		rs = 1;
		e = 1;
		delay();
		e = 0;
	}
}

