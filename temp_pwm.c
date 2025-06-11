#include <LPC213X.H>

unsigned int temp;

void msdelay(unsigned int time)
{
   unsigned int i, j;
   for(i=0;i<time;i++)
		{
			for(j=0;j<3000;j++);
		}
}

//
void lcd_data(char data)
{
	IOPIN1 = (data<<16);
	IOPIN1 |= (5<<24);
	msdelay(1);
	IOPIN1 &= ~(1<<26);
}

//
void lcd_cmd(unsigned char cmd)
{
	IOPIN1 = (cmd<<16);
	IOPIN1 |= (4<<24);
	msdelay(1);
	IOPIN1 &= ~(1<<26);
}

//
void lcd_init()
{
	lcd_cmd(0x38);
	 
	lcd_cmd(0x0C);

	lcd_cmd(0x80);
}

//
 unsigned int adc_read()
{
	unsigned int val;

	AD1CR =   (1<<2)     //CHANNEL NO.
					| (2<<8)		 //CLKDIV BY 0
					| (1<<21)		 //PDN ..TURN ON ADC
			//	| (1<<16)	   //BURST ON
					| (2<<17)		 //8-BIT RESOLUTION
					| (1<<24); 	 //start conversion
	
	while(!(AD1DR & 0x80000000));
	val=AD1GDR;
	val=(val >> 6) & 0x03FF;
	return val;
}

//
void backspace(){
	lcd_cmd(0x10);
	msdelay(1);
}
//
void pwm_irq()__irq
{
	
	if(temp>30)
 {
	switch(temp)
	{
		case 30:
		{
			PWMMR2 = 36;
			PWMLER = (1<<2);
		}
		case 35:
		{
			PWMMR2 = 36+12;
			PWMLER = (1<<2);
		}
		case 40:
		{
			PWMMR2 = 36+(12*2);
			PWMLER = (1<<2);
		}
		case 45:
		{
			PWMMR2 = 36+(12*3);
			PWMLER = (1<<2);
		}
		case 50:
		{
			PWMMR2 = 36+(12*4);
			PWMLER = (1<<2);
		}
		case 55:
		{
			PWMMR2 = 36+(12*5);
			PWMLER = (1<<2);
		}
		case 60:
		{
			PWMMR2 = 36+(12*6);
			PWMLER = (1<<2);
		}
		case 65:
		{
			PWMMR2 = 36+(12*7);
			PWMLER = (1<<2);
		}
	}
 }
	
 if(temp>60)
 {
	 switch(temp)
	{
		case 60:
		{
			PWMMR4 = 36;
			PWMLER = (1<<4);
		}
		case 65:
		{
			PWMMR4 = 36+12;
			PWMLER = (1<<4);
		}
		case 70:
		{
			PWMMR4 = 36+(12*2);
			PWMLER = (1<<4);
		}
		case 75:
		{
			PWMMR4 = 36+(12*3);
			PWMLER = (1<<4);
		}
		case 80:
		{
			PWMMR4 = 36+(12*4);
			PWMLER = (1<<4);
		}
		case 85:
		{
			PWMMR4 = 36+(12*5);
			PWMLER = (1<<4);
		}
		case 90:
		{
			PWMMR4 = 36+(12*6);
			PWMLER = (1<<4);
		}
		case 95:
		{
			PWMMR4 = 36+(12*7);
			PWMLER = (1<<4);
		}
  }
 }
 PWMIR = (1<<0);
 VICVectAddr = 0x00;
}

//
void int_init()
{
	VICIntSelect &= ~(1<<8);
	VICDefVectAddr = (unsigned)pwm_irq;
	VICIntEnable = (1<<8);
}

//
void pwm_init()
{

  PINSEL0 |= (2<<14)|(2<<16);
	PWMTCR = (1<<1);
	PWMPR = 999;
	PWMMR0 = 120;
	PWMMR2 = 36;
	PWMMR4 = 36;
	PWMMCR =(3<<0);
	PWMPCR = (1<<10)|(1<<12);
	PWMLER = (5<<0)|(1<<4);
	PWMTCR = (1<<0)|(1<<3);
}
//
int main()
{
	unsigned int i,r=0;
	unsigned char a[]="Temprature:";
	unsigned char b=' ';
  unsigned int adc_out;
	char buffer[50];
	
    IODIR1 =0xffffffff;
    PINSEL0 = 0x00300000;//  as AD1.2
    lcd_init();
		int_init();
		pwm_init();
    
		for(i=0;a[i]!='\0';i++)
	{
		lcd_data(a[i]);
	}
	
	
	    while(1) 
		{
     
      adc_out = adc_read();
			
			adc_out =  (adc_out/3);
			
			if(adc_out>20)
				adc_out = adc_out-1;
			if(adc_out>48)
				adc_out = adc_out-1;
			if(adc_out>75)
				adc_out = adc_out-1;
			if(adc_out>103)
				adc_out = adc_out-1;
			if(adc_out>131)
				adc_out = adc_out-1;
			
			temp = adc_out;
		
			
				if(r==0){
				r++;
			}
			else
			{
				for(i=0;i<5;i++)
				{
					backspace();
				}
			}
	
        buffer[0] = ((adc_out/100)%10)+48;
        buffer[1] = ((adc_out/10)%10)+48;
        buffer[2]= (adc_out%10)+48 ;
				buffer[3] = (223);
				buffer[4] = (67);
		
			for(i=0;i<5;i++)
			{
				lcd_data(buffer[i]);
			}
			
			msdelay(10);
			
			for(i=0;i<5;i++)
			{
				backspace();
			}
	
		for(i=0;i<5;i++)
		{
			lcd_data(b);
		}
		
	}
		
}

