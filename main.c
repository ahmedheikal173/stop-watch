/*
 * proju.c
 *
 * Created: 9/15/2022 10:08:45 PM
 * Author : Ahmed heikal
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char units = 0;
unsigned char tens = 0;
unsigned char hundreds = 0;
unsigned char thousands = 0;
unsigned char tenThousands = 0;

unsigned char firstTime = 0;
//unsigned char tick = 0;
void init_delay(void)
{
	TCCR1B = 0;
	TCNT1 = 0;
	OCR1A = 800;
	//	DDRB |= (1<<PB3);
	TIMSK |= (1<<OCIE1A);
	TCCR1B |= (1<<WGM12) | (1<<CS12) | (1<<CS10);
}


void init_Interrupt(void)
{
	DDRD  &= ~(1<<PD2);
	PORTD |= (1<<PD2);

	DDRD  &= ~(1<<PD3);
	PORTD &= ~(1<<PD3);

	DDRB  &= ~(1<<PB2);
	PORTB |= (1<<PB2);

	GICR |= (1<<INT0) | (1<<INT1) | (1<<INT2);
	MCUCR |= (1<<ISC01) | (1<<ISC10) | (1<<ISC11);
	MCUCSR |= ~(1<<ISC2);


}
ISR(INT0_vect)
{
	units = 0;
	hundreds = 0;
	tens = 0;
	thousands = 0;
}

ISR(INT1_vect)
{
	if(firstTime == 0)
	{
		firstTime++;
	}
	else
	{
		TCCR1B = 0;
		TIFR |= (1<<OCF1B);
	}

}
ISR(INT2_vect)
{
	init_delay();
}
ISR(TIMER1_COMPA_vect)
{
	if(units == 9)
	{
		units = 0;
		if(tens == 5)
		{
			tens = 0;
			if(hundreds == 9)
			{
				hundreds = 0;
				if(thousands == 5)
				{
					thousands = 0;
					tenThousands++;
				}
				else
				{
					thousands++;
				}
			}
			else
			{
				hundreds++;
			}
		}
		else
		{
			tens++;
		}
	}
	else
	{
		units++;
	}
}

void Init(void)
{
	DDRC |= (0x0F);
	PORTC = 0;

	DDRA |= (1<<PA1) | (1<<PA2) | (1<<PA3) | (1<<PA4) | (1<<PA5) | (1<<PA6);
	PORTA &= (0x81);
}
int main(void)
{
	Init();
	init_delay();
	init_Interrupt();
	sei();
	while (1)
	{

		PORTA = (1<<PA6);
		//PORTA &= ~(1<<PA1);
		PORTC = units;
		_delay_ms(5);


		PORTA = (1<<PA5);
		//PORTA &= ~(1<<PA6);
		PORTC = tens;
		_delay_ms(5);


		PORTA = (1<<PA4);
		//PORTA &= ~(1<<PA5);
		PORTC = hundreds;
		_delay_ms(5);


		PORTA = (1<<PA3);
		//PORTA &= ~(1<<PA4);
		PORTC = thousands;
		_delay_ms(5);


		PORTA = (1<<PA2);
		//PORTA &= ~(1<<PA3);
		PORTC = tenThousands;
		_delay_ms(5);


		PORTA = (1<<PA1);
		//PORTA &= ~(1<<PA2);
		PORTC = 0;
		_delay_ms(5);
	}
}

