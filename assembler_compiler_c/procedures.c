#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "checkSyntax.h"
#include "inputs.h"
#include "symbols.h"
#include "bases.h" 
#include "procedures.h"
#define SymbolMax 30

char * countLinesNum (int actionNum, int *L, char *command, int from)
{
	int numOfLine = 1,address1,address2; /*numOfLine : the first word command*/
	from+=3;
	if(actionNum>=14)	
	{
		if(actionNum == 15)
			from+=2;
		if(checkEndOfCommand(command,from))
			return "`Syntax error, unexpected text afer action name.\0";
	}
	else
	{
		if(!checkEndOfCommand(command,from))
			return "`Syntax error, expected operate after action name.\0";
		from = skipSpace(command,from);
		address1 = addressingType(command,from);
		switch (address1)
		{
			case 0:
			if(actionNum<=3 || actionNum == 12)
				numOfLine += 1;
			else
				return "`Syntax error, Addressing number: 0, can't be in first addressing at this action.\0";
			break;
			case 1:
			numOfLine += 1;
			break;
			case 2:
			numOfLine += 2;
			break;
			case 3:
			if(actionNum != 6)
				numOfLine += 1;
			else
				return "`Syntax error, Addressing number: 3, can't be in first addressing at this action.\0";
			break;
		}
		from = endOfWord(command,from);
		if(actionNum <= 3 || actionNum == 6)
		{
			from = indexAfterComma(command,from);
			if(from == -1)
				return "`Syntax error, expected a break ',' and another command in this action.\0";
			from = skipSpace(command,from);
			address2 = addressingType(command,from);
			switch (address2)
			{
				case 0:
				if(actionNum == 1)
					numOfLine += 1;
				else
					return "`Syntax error, Addressing number: 0, can't be in second addressing at this action.\0";
				break;
				case 1:
				numOfLine += 1;
				break;
				case 2:
				numOfLine += 2;
				break;
				case 3:				
					numOfLine += (address1 == 3)? 0 : 1;				
				break;
			}
		}
		else{
			if(checkEndOfCommand(command,from))
				return "`Syntax error, unexpected text, after operate.\0";
		}
	}
	*L = numOfLine;
	return NULL;
}


int addressingType(char *command, int from)
{	
	int i=0;
	if(command[from] == '#')
		return 0;
	if(command[from] == 'r' && command[from+1] >= '0' && command[from+1] <= '7')
		return 3;
	while(command[from+i] != '\0' && !isspace(command[from+i]) && command[from+i] != '.' && command[from+i] != ',')
		i++;
	return (command[from+i] == '.')? 2:1;
}

char *binSymbolAddress(char *command, int from, int end, ptSymbol head)
{
	int cLength = end - from +1,i;
	char *bin=NULL;
	static char name[SymbolMax];
	ptSymbol temp = head;
	for(i=0;i<cLength;i++)
	{
		name[i] = command[from+i];
	}
	name[i] = '\0';
	while(temp)
	{
		if(strcmp(temp->name,name) == 0)
		{
			bin = decToBin(temp->adress);
			if(bin[0] == '%')
				return "`Dynamic allocation error.\0";
			for(i=0;i<8;i++)
				bin[i] = bin[i+2];
			if(temp->type == 2)
			{
				bin[9] = '1';
				bin[8] = '0';
			}else
			{
				bin[9] = '0';
				bin[8] = '1';
			}
			bin[7] = '|';
			return bin;
		}
		temp = temp->next;
	}
	return "`Symbol was tipe, that is not define.\0";
}

