#include <avr/io.h>
#include <util/delay.h>

#define s1 bit_is_set(PINB, 4)
#define s2 bit_is_set(PINB, 3)
#define s3 bit_is_set(PINB, 2)
#define s4 bit_is_set(PINB, 1)
#define s5 bit_is_set(PINB, 0)
#define a1 bit_is_set(PINB, 5)
#define a2 bit_is_set(PINB, 6)
#define TURN 125
#define INCH 95

int shortestPath[] = {};
int counter = 0;
int sum = 0;
int prev = 0;
int finalCounter = 0;
	
//straight -> 1
//left -> 2
//right -> 4
//u-turn -> 8

void right(int i)
{
	PORTC = 0b00000110;
	while (1)
	{
		if ((s2||s3)&&s5)
		{	
			PORTC = 0b00000101;
			_delay_ms(INCH);
			PORTC = 0b00000000;
			break;
		}
	}
	if (i)
	{
		if ((prev == 1) || (prev == 2) || (prev == 4))
		{
			if ((sum == 2) || (sum == 13) || (sum == 28)))
				shortestPath[counter] = 2;
			else if ((sum == 1) || (sum == 16))
				shortestPath[i] = 1;
			else if (sum == 4)
				shortestPath[i] = 4;
			counter++;
			prev = 4;
			sum = 4;
		}
		else
		{
			prev = 4;
			sum += 4;
		}
	}
}

void left(int i)
{
	PORTC = 0b00001001;
	while (1)
	{
		if ((s2||s3)&&s5)
		{	
			PORTC = 0b00000101;
			_delay_ms(INCH);
			PORTC = 0b00000000;
			break;
		}
	}
	if (1)
	{
		if ((prev == 1) || (prev == 2) || (prev == 4))
		{
			if ((sum == 2) || (sum == 13) || (sum == 28)))
				shortestPath[counter] = 2;
			else if ((sum == 1) || (sum == 16))
				shortestPath[i] = 1;
			else if (sum == 4)
				shortestPath[i] = 4;
			counter++;
			prev = 2;
			sum = 2;
		}
		else
		{
			prev = 2;
			sum += 2;
		}
	}
}

void straight(int i)
{
	PORTC = 0b00000101;
	if (i)
	{
		if ((prev == 1) || (prev == 2) || (prev == 4))
		{
			if ((sum == 2) || (sum == 13) || (sum == 28)))
				shortestPath[counter] = 2;
			else if ((sum == 1) || (sum == 16))
				shortestPath[i] = 1;
			else if (sum == 4)
				shortestPath[i] = 4;
			counter++;
			prev = 1;
			sum = 1;
		}
		else
		{
			prev = 1;
			sum += 1;
		}
	}
}

void uTurn(int i)
{
	while (1)
	{
		PORTC = 0b00001010;
		if (s2||s3)
			break;
	}
	while (1)
	{
		PORTC = 0b00000110;
		if ((s2||s3)&&s5)
			break;
	}
	if (i)
	{
		prev = 8;
		sum += 8;
	}
}

void tJunction()
{
	while (1)
	{
		if (!s1&&!s2&&!s3&&!s4&&!s5)
		{
			uTurn(1);
			break;
		}
		else if (s1&&(s2||s3)&&!s5)
		{
			right(1);
			break;
		}
		else if ((s2||s3)&&s4&&!s5)
		{
			left(1);
			break;
		}
	}
}

void alignment()
{
	if (a1)
	{
		PORTC = 0b00000110;
		while(1)
			if (s5)
			{
				PORTC = 0b00000101;
				break;
			}
	}
	else if (a2)
	{
		PORTC = 0b00001001;
		while(1)
			if (s5)
			{
				PORTC = 0b00000101;
				break;
			}
	}
}

void dryRun()
{
	while (1)
	{
		if (!s1&&(s2||s3)&&!s4&&s5)
			PORTC = 0b00000101;
		else if (s1&&(s2||s3))
			right(1);
		else if ((s2||s3)&&s4)
			left(1);
		else if (!s1&&(s2||s3)&&!s4&&!s5)
		{
			PORTC = 0b00000101;
			tJunction();
		}
		else if (s1&&s2&&s3&&s4&&s5&&a1&&a2)
		{
			PORTC = 0b00010000;
			break;
		}
		else if ((a1||a2)&&!s5)
		{
			alignment();
		}
	}
}

int main ()
{
	DDRC = 0b11111111;
	DDRB = 0b00000000;
	while (1)
	{	
		dryRun();
		_delay_ms(10000);
		if (!s1&&(s2||s3)&&!s4&&s5)
			PORTC = 0b00000101;
		else if ((s1&&s2&&s3&&!s4&&s5) || (!s1&&s2&&s3&&s4&&s5) || (s1&&s2&&s3&&s4&&s5) || (s1&&s2&&s3&&!s4&&!s5) || (!s1&&s2&&s3&&s4&&s5) || (s1&&s2&&s3&&s4&&!s5))
		{
			if (shortestPath[finalCounter] == 1)
			{
				straight(0);
				finalCounter++;
			}
			else if (shortestPath[finalCounter] == 2)
			{
				left(0);
				finalCounter++;
			}
			else if (shortestPath[finalCounter] == 4)
			{
				right(0);
				finalCounter++;
			}
			else
				PORTC = 0b00010000;
		}
	}
}