#include <avr/io.h>
#include <util/delay.h>

#define s1 bit_is_set(PINB, 4)
#define s2 bit_is_set(PINB, 3)
#define s3 bit_is_set(PINB, 2)
#define s4 bit_is_set(PINB, 1)
#define s5 bit_is_set(PINB, 0)
#define TURN 125
#define INCH 90

int path[][7] = {};
int shortestPath[] = {};
int counter = 0;
int junctionCounter = 0;
int dryRun = 1;
int aligned = 1;
int finalCounter = 0;
	
//straight -> 1
//left -> 2
//right -> 4
//u-turn -> 8

int sum(int ar[])
{
	int i, sum = 0;
	for (i = 0; ar[i] != 0; ++i)
		sum += ar[i];
	return sum;
}

void right()
{
	if (counter)
		if (path[junctionCounter][counter-1] != 8)
			junctionCounter++;
	path[junctionCounter][counter] = 4;
	counter++;
	PORTC = 0b00000110;
	_delay_ms(TURN);
    PORTC = 0b00000101;
	_delay_ms(INCH*2);
}

void left()
{
	if (counter)
		if (path[junctionCounter][counter-1] != 8)
			junctionCounter++;
	path[junctionCounter][counter] = 2;
	counter++;
	PORTC = 0b00001001;
	_delay_ms(TURN);
	PORTC = 0b00000101;
	_delay_ms(INCH*2);
}

void straight()
{
	if (counter)
		if (path[junctionCounter][counter-1] != 8)
			junctionCounter++;
	path[junctionCounter][counter] = 1;
	counter++;
	PORTC = 0b00000101;
	_delay_ms(INCH*2);
}

void uTurn()
{
	path[junctionCounter][counter] = 8;
	counter++;
	PORTC = 0b00000110;
	_delay_ms(TURN*2);
	PORTC = 0b00000101;
	_delay_ms(INCH*2);
}

void shortestPathCalculator()
{
	int i;
	for (i = 0; i <= junctionCounter; ++i)
	{
		if ((sum(path[i]) == 2) || (sum(path[i]) == 13) || (sum(path[i] == 28)))
			shortestPath[i] = 2;
		else if ((sum(path[i]) == 1) || (sum(path[i]) == 16))
			shortestPath[i] = 1;
		else if (sum(path[i]) == 4)
			shortestPath[i] = 4;
	}
}

int endCheck()
{
	PORTC = 0b00000101;
	_delay_ms(INCH);
	if (s1&&s2&&s3&&s4&&s5)
	{
		return 1;
	}
	else
	{
		PORTC = 0b00001010;
		_delay_ms(INCH);
		return 0;
	}
}

int main (void)
{
	DDRC = 0b11111111;
	DDRB = 0b00000000;
	while (1)
	{	
		if (dryRun && aligned)
		{
			if (!s1&&s2&&s3&&!s4&&s5)
				PORTC = 0b00000101;
			else if (s1&&s2&&s3&&s4&&s5)
			{
				if (endCheck())
				{
					PORTC = 0b00010000;
					shortestPathCalculator();
					dryRun = 0;
				}
				else
					right();
			}
			else if ((s2&&!s3&&!s4&&!s5) ||(!s1&&!s2&&s3&&!s5))
				aligned = 0;
			else if (!s1&&s2&&s3&&!s4&&!s5)
				PORTC = 0b00000101;
				_delay_ms(INCH);
				if (s1&&s2&&s3&&s4)
					right();
				else if (s1&&s2&&s3&&!s4)
					right();
				else if (!s1&&s2&&s3&&s4)
					left();
				else if (!s1&&s2&&s3&&!s4&&!s5)
					uTurn();
		}
		else if (!dryRun && aligned)
		{
			if (!s1&&s2&&s3&&!s4&&s5)
				PORTC = 0b00000101;
			else if ((s1&&s2&&s3&&!s4&&s5) || (!s1&&s2&&s3&&s4&&s5) || (s1&&s2&&s3&&s4&&s5) || (s1&&s2&&s3&&!s4&&!s5) || (!s1&&s2&&s3&&s4&&s5) || (s1&&s2&&s3&&s4&&!s5))
			{
				if (shortestPath[finalCounter] == 1)
				{
					straight();
					finalCounter++;
				}
				else if (shortestPath[finalCounter] == 2)
				{
					left();
					finalCounter++;
				}
				else if (shortestPath[finalCounter] == 4)
				{
					right();
					finalCounter++;
				}
				else
					PORTC = 0b00010000;
			}
		}
		else if (!aligned)
		{
			if (s2&&!s3&&!s4&&!s5)
				PORTC = 0b00000100;
			else if (!s1&&!s2&&s3&&!s5)
				PORTC = 0b00000001;
			else if (s2&&s3&&s5)
				aligned = 1;
		}
	}
}
