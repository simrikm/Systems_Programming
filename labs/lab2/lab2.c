#include <stdio.h>
#include <string.h>

#define STRMAX 1024

int toInt(char c)
{
	switch(c)
	{
	case 'I': return 1;
	case 'V': return 5;
	case 'X': return 10;
	case 'L': return 50;
	case 'C': return 100;
	case 'D': return 500;
	case 'M': return 1000;
	default: return -1;
	}
}

int main(/*int argc,char *argc[]*/)
{
	char roman[STRMAX];	
	while(fgets(roman, STRMAX, stdin))
	{
		sscanf("%s",roman);
		int len = strlen(roman)-2;
		int val = 0, max = 0;
		for(int i = len; i >= 0; --i)
		{
			int temp = toInt(roman[i]);
			if(temp >= max)
			{
				val += temp;
				max = temp;
			}
			else val -= temp;
		}
		printf("%s => %d\n", roman, val);
	}
	return 0;
}
