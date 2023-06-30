#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "inputs.h"
char buffer[100];
static int index = -1;

char *getCommand(FILE *fp1)
{
	char c;
	char *arr;
	int i;
	if((c=getNextChar(fp1))!= EOF)
		setNextChar(c);
	else
		return NULL;
	arr = (char *)calloc(commanSize,sizeof(char));
	if(arr)
	{
		i = 0;
		while((c = getNextChar(fp1)) != EOF && c != '\n' && i<(commanSize-1))
		{
			arr[i] = c;
			i++;
		}
		arr[i] = '\0';
		if(i <= (commanSize-1) && c!= EOF && c != '\n')
		{
			setNextChar(c);
			while((c = getNextChar(fp1)) != EOF && c != '\n')
				;
			if(c == EOF)			
				setNextChar(c);			
			return "Syntax error, too much long command.\0";
		}
		else
			return arr;

	}
	else
	{
		return "Dynamic allocatio error, can not create space.\0";
	}	
}

char getNextChar(FILE *fp1)
{
	if(index>-1)
	{
		return buffer[index--];
	}
	return getc(fp1);
}

void setNextChar(char c)
{
	buffer[++index] = c;
}

int skipSpace(char *command, int from)
{
	char c;
	while((c = command[(from)]) != '\0' && isspace(c))
		from++;
	return from;
}

int indexAfterComma(char *command, int from)
{
	from = skipSpace(command,from);
	if(command[from] == ',')
	{
		from++;
		return from;
	}
	else
	{
		return -1;
	}
}

int checkEndOfCommand(char *command, int from)
{
	from = skipSpace(command,from);
	return (command[from] != '\0')? 1 : 0;	
}

int endOfWord(char *command, int from)
{
	char c;
	while((c = command[(from)]) != '\0' && !isspace(c) && (c != ','))
		from++;
	return from;
}

