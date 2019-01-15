#include<avr/io.h>
#include<util/delay.h>

void init_adc()
{
ADCSRA=ADCSRA|0b00000001<<ADEN;				//Ennable ADC by accessing "ADCSRA" register's "ADEN" bit (7th bit)
ADCSRA=ADCSRA|0B00000001<<ADPS2;			//Calculate division factor and accordingly make the necessary pins high 
ADMUX=ADMUX|0b00000001<<REFS0;				//Set reference voltage to 5V
}

unsigned int read_adc()
{
int p=0b00000001;
ADMUX=ADMUX|p;								//MUX0 is set HIGH in order to take input from pin 39 "ADC1"
ADCSRA=ADCSRA|(0b00000001<<ADSC);			//To start conversion, ADSC is set high
while((ADCSRA&(0b00000001<<ADIF))==0)		//ADIF becomes HIGH when conversion ends
{}
ADCSRA=ADCSRA|(0b00000001<<ADIF);			//Reset ADIF bit (ATmega 16 is so designed that once ADIF is set HIGH, it becomes LOW!)
return(ADC);								//Get the output from ADC
}

void main()
{
DDRB=0b11111111;							//Set Part B as output
init_adc();									//Call init_adc function
_delay_ms(500);
unsigned int a;
unsigned int p;
unsigned int T;
unsigned int TON;
unsigned int TOFF;
while(1)
{
p=read_adc();
a=(p/40);									//Scaling down (40 is an optimum tested value, however can be changed for tuning)
T=25.6;										//Dividing 1024 (2^10 quantization levels) by 40 we get scaled down total time period
TOFF=a;
TON=T-TOFF;
if(p<250)									//Digital output with input = LDR voltage when exposed to ambient light
{
PORTB=0b00001010;
_delay_ms(TON);
PORTB=0b00000000;
_delay_ms(TOFF);
}
else
PORTB=0b00000000;
}
}