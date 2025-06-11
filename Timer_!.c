#include <LPC213X.H>

void delay()
{
	T0TCR = (1<<4);
	T0MR0 = 300000;
	T0MCR = (3<<0);
	T0TCR = (1<<0);
	while(!(T0IR & (1<<0)));
	T0IR = (1<<0);
	T0TCR = (0<<0);
}

int main()
{
	IODIR0 |= (1<<4);
	
	while (1)
	{
		IOSET0 = (1<<4);
		delay();
		IOCLR0 = (1<<4);
		delay();
	}
}
