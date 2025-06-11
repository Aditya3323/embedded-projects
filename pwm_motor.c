#include <LPC213X.H>

void pwm_init()
{

  PINSEL0 = (2<<14)|(2<<16);
	PWMTCR = (1<<1);
	PWMPR = 999;
	PWMMR0 = 120;
	PWMMR2 = 36;
	PWMMR4 = 36;
	PWMMCR =(2<<0);
	PWMLER = (5<<0)|(1<<4);
	PWMPCR = (1<<10)|(1<<12);
	PWMTCR = (1<<0)|(1<<3);
}



int main()
{
	IODIR0 |=(1<<10);
	pwm_init();
	
	while(1)
	{
		IOSET0 = (1<<10);
	if(PWMMR2 < (120-6))
	{
		if(PWMMR2 >= 36)
		{
			if(IOPIN0 & (1<<0))
			{
			while((IOPIN0 & (1<<0)));
			PWMMR2 = (PWMMR2+6);
			PWMLER = (5<<0);
			}
		}
	}
	if(PWMMR2 <= 120)
	{
		if(PWMMR2 >= 42)
		{
			if(IOPIN0 & (1<<1))
			{
			while((IOPIN0 & (1<<1)));
			PWMMR2 = (PWMMR2-6);
			PWMLER = (5<<0);
			}
		}
	}
	//
	if(PWMMR4 < (120-6))
	{
		if(PWMMR4 >= 36)
		{
			if(IOPIN0 & (1<<2))
			{
			while((IOPIN0 & (1<<2)));
			PWMMR4 = (PWMMR4+6);
			PWMLER = (1<<4);
			}
		}
	}
	if(PWMMR4 <= 120)
	{
		if(PWMMR4 >= 42)
		{
			if(IOPIN0 & (1<<3))
			{
			while((IOPIN0 & (1<<3)));
			PWMMR4 = (PWMMR4-6);
			PWMLER = (1<<4);
			}
		}
	}
	}
}
