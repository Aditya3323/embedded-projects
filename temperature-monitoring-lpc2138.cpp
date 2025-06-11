#include <LPC213X.H>

class LCDInterface 
{
public:
    virtual void sendData(char data) = 0; 
    virtual void sendCommand(unsigned char cmd) = 0;
    virtual void initialize() = 0;
    virtual ~LCDInterface() {}
};

//
class LCD : public LCDInterface 
{
public:
    virtual void sendData(char data) 
		{ 
                IOPIN1 = (data << 16);
IOPIN1 |= (5 << 24);
        delay(1);
        IOPIN1 &= ~(1 << 26);
    }

    virtual void sendCommand(unsigned char cmd) 
		{
        IOPIN1 = (cmd << 16);
        IOPIN1 |= (4 << 24);
        delay(1);
        IOPIN1 &= ~(1 << 26);
    }

    virtual void initialize() 
		{ 
        sendCommand(0x38); // 2 lines, 5x7 matrix
        sendCommand(0x0C); // Display on, cursor off
        sendCommand(0x80); // Set cursor position
    }

private:
    void delay(unsigned int time) 
		{
        for (unsigned int i = 0; i < time; i++) 
				{
            for (unsigned int j = 0; j < 3000; j++);
        }
    }
};

//
class ADC 
{
public:
    unsigned int read() 
		{
        unsigned int val;
			
      	AD1CR = (1<<2)     //CHANNEL NO.
							| (2<<8)		 //CLKDIV BY 3
							| (1<<21)		 //PDN ..TURN ON ADC
					//	| (1<<16)	   //BURST ON
							| (2<<17)		 //8-BIT RESOLUTION
							| (1<<24); 	 //start conversion
			
        while (!(AD1DR & 0x80000000));
        val = AD1GDR;
        val = (val >> 6) & 0x03FF;
        return val;
    }
};

//
class TemperatureConverter 
{
public:
    int convertToCelsius(unsigned int adcValue) 
		{
        int temperature = adcValue / 3;
			
        if (temperature > 20) 
					temperature--;
        if (temperature > 48) 
					temperature--;
        if (temperature > 75) 
					temperature--;
        if (temperature > 103) 
					temperature--;
        if (temperature > 131) 
					temperature--;
				
        return temperature;
    }
};

//
class IOControl {
public:
    void setupIO() {
        IODIR1 = 0xFFFFFFFF;  
        IODIR0 = (0x0F << 18); 
        PINSEL0 = 0x00300000;  // Set P0.10 as AD1.2
    }

    void controlOutput(int temperature) {
        if (temperature >= 40) {
            IOSET0 = (1 << 18) | (1 << 20);
        } else {
            IOCLR0 = (1 << 18) | (1 << 20);
        }

        if (temperature >= 60) {
            IOSET0 = (3 << 18) | (3 << 20);
        } else {
            IOCLR0 = (1 << 19) | (1 << 21);
        }
    }
};

//
int main() 
{
	unsigned int adcValue;
	int temperature;
	char buffer[50];
	const char message[] = "Temperature:";
	
    LCD lcd;
    ADC adc;                        
    TemperatureConverter tempConverter; 
    IOControl ioControl;            
    ioControl.setupIO();            
    
    while (1) 
		{
			lcd.initialize(); 

         adcValue = adc.read(); // Read ADC val
			
         temperature = tempConverter.convertToCelsius(adcValue); // Convert ADC to temp
			
         ioControl.controlOutput(temperature); // Control output based on temp

				for (unsigned int i = 0; message[i] != '\0'; i++) 
				{  
        lcd.sendData(message[i]);
				}
	
        buffer[0] = (temperature / 100) % 10 + '0';
        buffer[1] = (temperature / 10) % 10 + '0';
        buffer[2] = temperature % 10 + '0';
        buffer[3] = 223; // Degree symbol
        buffer[4] = 'C';
				
				for (unsigned int i = 0; buffer[i] != '\0'; i++) 
				{  
        lcd.sendData(buffer[i]);
				}	
    }
}

