#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "inputs.h"
#include "symbols.h"


int setNextSymbol(ptSymbol* head, char *command, int from, int length, int adress, short type)
{
	ptSymbol pt;	
	if(*head)
	{		
		pt = *head;		
		while(pt->next)
		{			
			pt = pt->next;
		}		
		pt->next = (ptSymbol)calloc(1,sizeof(nodeSymbol));		
		pt = pt->next;
	}
	else
	{		
		*head = (ptSymbol)calloc(1,sizeof(nodeSymbol));
		pt = *head;
	}	
	if(pt && (pt->name = (char *)calloc((length+1),sizeof(char))))
	{			
		pt->next = NULL;
		pt->name = strncpy(pt->name, command+from,length);
		pt->name[length] = '\0';
		pt->adress = adress;
		pt->type = type;
	}
	else
	{		
		return -2;
	}
	return 1;
}

int isSymbolExist(ptSymbol head, char *name)
{	
	while(head)
	{
		if(strcmp(head->name,name) == 0)
			return 1;
		head = head->next;		
	}
	return 0;
}

ptSymbol getSymbolIfExist(ptSymbol head, char *name)
{
	while(head)
	{
		if(strcmp(head->name,name) == 0)
			return head;
		head = head->next;		
	}
	return NULL;
}

int isSymbolListEmpty(ptSymbol head)
{
	return (head)? 0 : 1;
}

ptSymbol getNextSymbol(ptSymbol* head)
{
	ptSymbol p;
	if(!isSymbolListEmpty(*head))
	{
		p = *head;
		*head = (*head)->next;
		return p;
	}
	return NULL;
}

void deleteAllSymbols(ptSymbol* head)
{
	ptSymbol pt,next;
	if(!isSymbolListEmpty(*head))
	{
		pt = *head;
		while(pt)
		{
			next = pt->next;
			free(pt->name);
			free(pt);
			pt = next;
		}
		*head = NULL;
	}
}

int addSymbols(ptSymbol* head, char *command, int from)
{
	int start,end,count=0;	
	while(checkEndOfCommand(command, from) && from>=0)
	{
		start = skipSpace(command, from);
		end = endOfWord(command, start);
		if(end>-1)
		{						
			if(setNextSymbol(head, command, start, end - start , 0, 2) <0 )
				return -2;
			count++;
			from = indexAfterComma(command,end);				
			if((from == -1 && checkEndOfCommand(command, end)) || (from != -1 && !checkEndOfCommand(command, from)))
				return -3;			
		}
		else
		{
			return -1;
		}
	}
	return count;
}

void updateSymbolsIC(ptSymbol head, int IC)
{
	while(head)
	{
		if(head->type <= 1)
			head->adress += IC;
		head = head->next;
	}
}


