#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputs.h"
#include "symbols.h"
#include "entryAndExtern.h"
#include "Code.h"
#include "checkSyntax.h"
#include "bases.h" 
#include "data.h"
#include "procedures.h"
#include "utils.h"
#define SymbolMax 30
#define ErrorJump \
flgError = ON;\
free(command);\
continue;


enum Status {OFF,ON};
/*Pointers to all lists*/
dataPointer Head_AsData = NULL;
ptSymbol Head_Symbol = NULL;
codePointer Head_Code = NULL;
ptEAEList Head_EAE = NULL;
short flgError;
char symName[commanSize];



void pass1(FILE *file)
{
	short flagSymbol = OFF;
	int index, end, INTresult, INTresult2, Sy_start, Sy_length, IC = 100, DC = 0, L = 0, lineNumber = 0;
	char *TEXTresult = NULL, *command = NULL;
	flgError = OFF;	
	while((command = getCommand(file)))
	{
		index = 0;
		L = 0;
		flagSymbol = OFF;
		lineNumber++;
		if(!checkEndOfCommand(command,index) || command[skipSpace(command,0)] == ';')
			continue;		
		if(command[index] == '`')
		{
			printf("Error: in (line: %d) %s\n",lineNumber,(command+index+1) );
			flgError = ON;			
			continue;
		}		
		index = skipSpace(command,index);
		end = checkLabel(command,index);
		if(end>0)
		{
			flagSymbol = ON;
			Sy_start = index;
			Sy_length = end - index;
			if(Sy_length>=SymbolMax)
			{
				printf("Error: in (line: %d, index: %d) Syntax error, Symbol name can't be longer than: %d.\n",lineNumber,index,SymbolMax);
				ErrorJump	
			}
			index = skipSpace(command,end+1);			
			if(!checkEndOfCommand(command,index))
			{
				printf("Error: in (line: %d, index: %d) Syntax error, expected a method name or directive after symbol.\n",lineNumber,index);
				ErrorJump
			}
		}
		if(command[index] == '.')
		{
			INTresult = checkDirective(command,index);
			if(INTresult<=0)
			{
				printf("Error: in (line: %d, index: %d) Syntax error, unknow directive name.\n",lineNumber,index);
				ErrorJump
			}
			if(INTresult<=3)
			{
				if(flagSymbol)
				{
					flagSymbol = OFF;
					strncpy(symName,(command+Sy_start),Sy_length);			
					symName[Sy_length] = '\0';
					if(isSymbolExist(Head_Symbol,symName))
					{
						printf("Error: in (line: %d, index: %d) Symbol already exist, and have been defined befor.\n",lineNumber,Sy_start);						
						ErrorJump
					}
					if(INTresult == 3)
						INTresult2 = setNextSymbol(&Head_Symbol, command, Sy_start, Sy_length, DC, 1);/*1- for sruct.*/
					else
						INTresult2 = setNextSymbol(&Head_Symbol, command, Sy_start, Sy_length, DC, 0);
					if(INTresult2 == -2)
					{
						printf("Error: in (line: %d, index: %d) Dynamic alocation error, could not add Symbol.\n",lineNumber,Sy_start);
						ErrorJump
					}
				}
				switch(INTresult)
				{
					case 1:
					INTresult2 = addData(command, index, &DC, &Head_AsData);
					if(INTresult2 < 0 && INTresult2 != -2)
					{
						printf("Error: in (line: %d, index: %d) Syntax error, after directive name. (expected: number,number..) note: all number must be between: -512:512 .\n",lineNumber,index);
						ErrorJump
					}					
					break;
					case 2:
					INTresult2 = addString(command, index, &DC, &Head_AsData);
					if(INTresult2 < 0 && INTresult2 != -2)
					{
						printf("Error: in (line: %d, index: %d) Syntax error, after directive name. (expected: \"sum_text\").\n",lineNumber,index);	
						ErrorJump						
					}					
					break;
					case 3:					
					INTresult2 = addStruct(command, index, &DC, &Head_AsData);
					if(INTresult2 < 0 && INTresult2 != -2)
					{
						printf("Error: in (line: %d, index: %d) Syntax error, after directive name. (expected: number,\"sum_text\").\n",lineNumber,index);
						ErrorJump
					}					
					break;
				}
				if(INTresult2 == -2)
				{
					printf("Error: in (line: %d, index: %d) Dynamic alocation error, could not add data.\n",lineNumber,index);					
					ErrorJump
				}
				if(checkEndOfCommand(command,INTresult2))
				{
					printf("Error: in (line: %d, index: %d) Syntax error, unexpected text after directive order.\n",lineNumber,INTresult2);
					ErrorJump
				}				
			}
			else
			{
				if(flagSymbol)
				{
					flagSymbol = OFF;
					printf("Warning in (line: %d, index: %d) unexpected Symbol in this type of directive.\n",lineNumber,Sy_start);				
				}
				if(INTresult == 5)
				{					
					index += externSize;
					if(checkEndOfCommand(command,index))
					{
						index = skipSpace(command,index);
						INTresult2 = addSymbols(&Head_Symbol, command, index);
						if(INTresult2<0)
						{
							switch(INTresult2)
							{
								case -2:
								printf("Error: in (line: %d, index: %d) Dynamic alocation error, could not add Symbol.\n",lineNumber,Sy_start);
								ErrorJump
								break;
								case -3:								
								printf("Error: in (line: %d, index: %d) Syntax error, unexpected text after adding symbols.\n",lineNumber,index);
								ErrorJump
								break;
								default :
								printf("Error: in (line: %d, index: %d) Syntax error, expected symbol name.\n",lineNumber,index);
								ErrorJump
							}							
						}
					}					
					else
					{
						printf("Error: in (line: %d, index: %d) expected symbol name afted directive: extern.\n",lineNumber,Sy_start);
						ErrorJump
					}					
				}
			}
			free(command);
			continue;
		}
		if(flagSymbol)
		{
			flagSymbol = OFF;
			strncpy(symName,(command+Sy_start),Sy_length);					
			symName[Sy_length] = '\0';
			if(isSymbolExist(Head_Symbol,symName))
			{
				printf("Error: in (line: %d, index: %d) Syntax error, Symbol already exist, and have been defined befor.\n",lineNumber,Sy_start);						
				ErrorJump
			}			
			INTresult2 = setNextSymbol(&Head_Symbol, command, Sy_start, Sy_length, IC, 3);/*3- for code Symbol.*/			
			if(INTresult2 == -2)
			{
				printf("Error: in (line: %d, index: %d) Dynamic alocation error, could not add Symbol.\n",lineNumber,Sy_start);
				ErrorJump
			}
		}
		INTresult = checkAction(command,index);
		if(INTresult<0)
		{
			printf("Error: in (line: %d, index: %d) Syntax error, unknow order name.\n",lineNumber,index);
			ErrorJump
		}
		TEXTresult = countLinesNum(INTresult, &L, command, index);
		if(TEXTresult != NULL && TEXTresult[0] == '`')
		{
			printf("Error: in (line: %d, index: %d) %s.\n",lineNumber,index,(TEXTresult+1));
			ErrorJump
		}
		IC+=L;
		free(command);
	}
	updateSymbolsIC(Head_Symbol,IC);	
}


void pass2(FILE *file)
{	
	int index, end, INTresult, INTresult2, IC = 100, lineNumber = 0;
	char *TEXTresult = NULL, *command = NULL;
	flgError = OFF;
	while((command = getCommand(file)))
	{
		index = 0;		
		lineNumber++;
		if(!checkEndOfCommand(command,index) || command[skipSpace(command,0)] == ';')
			continue;		
		if(command[index] == '`')
		{
			printf("Error: in (line: %d) %s\n",lineNumber,(command+index+1) );
			flgError = ON;			
			continue;
		}
		index = skipSpace(command,index);
		end = checkLabel(command,index);
		if(end>0)
			index = skipSpace(command,end+1);
		if(command[index] == '.')
		{
			INTresult = checkDirective(command,index);			
			if(INTresult == 4)
			{				
				index += entrySize;
				if(checkEndOfCommand(command,index))
				{					
					index = skipSpace(command,index);
					end = endOfWord(command,index);
					if((end-index) > SymbolMax)
					{
						printf("Error: in (line: %d, index: %d) Syntax error, symbol name is too long. Can't pass: %d chars.\n",lineNumber,index,SymbolMax);
						ErrorJump
					}
					TEXTresult = EAE(command,index,end-1);
					if(TEXTresult)
					{
						printf("Error: in (line: %d, index: %d) %s.\n",lineNumber,index,TEXTresult);
						ErrorJump
					}
					if(checkEndOfCommand(command,end))
					{
						printf("Error: in (line: %d, index: %d) Syntax error, unexpected text after symbol name.\n",lineNumber,skipSpace(command,end));
						ErrorJump
					}
				}
				else
				{
					printf("Error: in (line: %d, index: %d) Syntax error, expected symbol name afted directive: entry.\n",lineNumber,index);
					ErrorJump
				}
			}
			free(command);
			continue;
		}
		INTresult = checkAction(command,index);
		if(INTresult<0)
		{			
			ErrorJump
		}
		TEXTresult = commandCoding(INTresult, command, index, &IC);
		if(TEXTresult != NULL && TEXTresult[0] == '`')
		{
			printf("Error: in (line: %d, index: %d) %s.\n",lineNumber,index,(TEXTresult+1));
			ErrorJump
		}		
		free(command);
	}
	INTresult2 =  addDTC(IC);
	if(INTresult2 == -2)
	{
		printf("Error: Dynamic alocation error, could not add data list to code list.\n");
		flgError = ON;		
	}
}






