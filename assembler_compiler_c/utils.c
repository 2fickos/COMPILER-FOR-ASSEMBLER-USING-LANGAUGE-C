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
#include <unistd.h>

#define SymbolMax 30

/*Pointers to the all lists*/
extern dataPointer Head_AsData;
extern ptSymbol Head_Symbol;
extern codePointer Head_Code;
extern ptEAEList Head_EAE;


int createFiles(const char *fileName)
{
	int flagEnt=0,flagExt=0;
	char *fnameOB = NULL,*fnameENT = NULL,*fnameEXT = NULL,*textToPrint = NULL;
	FILE *fp1,*fp2;
	codePointer ptCode;
	ptEAEList ptEAE;	
	if(isEmptyCodeMethod(Head_Code))
	{		
		fnameOB = connectchars(fileName,".ob\0");		
		if(!fnameOB || !(fp1 = fopen(fnameOB,"w")))
			return -1;
		fprintf(fp1,"m\tf\n");
		ptCode = Head_Code;
		while(ptCode)
		{
			textToPrint = getBase32CodeMethod(&ptCode);			
			if(!textToPrint)
			{
				fclose(fp1);
				unlink(fnameOB);
				return -1;
			}
			fprintf(fp1,"%s\n",textToPrint);
		}
		fclose(fp1);
	}
	if(!isEntExtEmpty(Head_EAE))
	{
		ptEAE = Head_EAE;
		while(ptEAE)
		{
			flagEnt = (flagEnt || !(ptEAE->isExt));
			flagExt = (flagExt || (ptEAE->isExt));
			ptEAE = ptEAE->next;
		}
		if (flagExt)
		{
			fnameEXT = connectchars(fileName,".ext\0");
			if(!fnameEXT || !(fp1 = fopen(fnameEXT,"w")))
			{
				unlink(fnameOB);
				return -1;
			}
		}
		if(flagEnt)
		{
			fnameENT = connectchars(fileName,".ent\0");
			if(!fnameENT || !(fp2 = fopen(fnameENT,"w")))
			{
				unlink(fnameOB);
				if(fnameEXT) {unlink(fnameEXT);}
				return -1;
			}
		}

		ptEAE = Head_EAE;
		while(ptEAE)
		{
			if(ptEAE->isExt)
			{
				fprintf(fp1,"%s\t%s\n",ptEAE->name,ptEAE->address);
			}else
			{
				fprintf(fp2,"%s\t%s\n",ptEAE->name,ptEAE->address);
			}
			ptEAE = ptEAE->next;
		}
		if (flagExt)
			fclose(fp1);
		if(flagEnt)
			fclose(fp2);
	}

	return 1;
}

int addDTC(int IC) 
{
	codePointer last = Head_Code,temp;
	dataPointer head = Head_AsData;
	char *bin;
	if(last)
		while(last->next)
			last = last->next;
	else
	{
		if(head)
		{
			if((temp = (codePointer)malloc(sizeof(codeNode))) && (bin = decToBin(head->data)))
			{
				temp->address = IC+head->DC;
				temp->bin = bin;
				Head_Code = temp;
				last = temp;
			}
			else{
				return -2;
			}
		}
	}

	while(head)
	{
		if((temp = (codePointer)malloc(sizeof(codeNode))) && (bin = decToBin(head->data)))
		{
			temp->address = IC+head->DC;
			temp->bin = bin;
			last->next = temp;
			last = last->next;
		}
		else{
			return -2;
		}
		head = head->next;
	}
	return last->address;
}

char *commandCoding(int NumAction, char *command, int from, int *IC)
{
	unsigned int nextLine[5] = {0,0,0,0,0};/*array contains the max 5 lines that may needed.*/
	int numberOfLine = 0;/*the number of lines needed.*/
	int result;
	int Addressing1,Addressing2,end,i;
	static char SyName[SymbolMax];
	ptSymbol temp;
	/*frist line*/
	numberOfLine++;
	nextLine[0]|= (NumAction<<6);
	from+=3;
	if(NumAction>=14)
	{
		if(NumAction == 15)
			from+=2;			
		if(checkEndOfCommand(command,from))
			return "`Syntax error, unexpected text afer action name.\0";
		else
			{
				int i;
				char *bin;
				for(i=0;i<numberOfLine;i++)
				{
					bin = decToBin(nextLine[i]);
					if(bin[0] == '%' || addCodeMethod(&Head_Code,*IC,bin) == -2)
						return "`Dynamic allocatio error.\0";
					(*IC)+=1;
				}
				return NULL;

			}

	}
	/*second line*/
	if(!checkEndOfCommand(command,from))
		return "`Syntax error, expected operate after action name.\0";
	from = skipSpace(command,from);
	Addressing1 = addressingType(command,from);	
	switch (Addressing1)
	{
		case 0:/*command[from] = "#..."*/
		if(NumAction<=3 || NumAction == 12){
			from++;
			end = 0;
			end = checkNumberRange(command,from);
			if(end>0)/*there is a valid number between from to end*/
			{
				result = textToNum(command,from,end-1);
				if(result>128 || result<-128)/* 8 bit + 2 for type of coading*/
					return "`Syntax error, number can be between 128 to -128 in this action.\0";	
				nextLine[numberOfLine]|= ((result<<2)&(-4));
				numberOfLine++;
				from = end;
			}
			else
				return "`Syntax error, expected a number after '#' in first operate.\0";
		}
		else
			return "`Syntax error, Addressing number: 0, can't be in first operate, at this action.\0";
		break;
		case 1:
		end = 0;
		end = endOfWord(command, from);
		if(end - from >= SymbolMax)
			return "`Syntax error, too much long text afte action name. Symbol name can't be more then 30 chars.\0";
		for(i=0;i<end - from;i++)
			SyName[i] = command[from+i];
		SyName[i] = '\0';
		if((temp = getSymbolIfExist(Head_Symbol,SyName)))
		{
			nextLine[numberOfLine]|= (temp->adress)<<2;
			if(temp->type != 2)/* ENTERNAL sdymbol*/
				nextLine[numberOfLine]|= 2;
			else				/* EXTERNAL sdymbol*/
			{
				nextLine[numberOfLine]|= 1;
				if( addEntExt(&Head_EAE, temp->name , (*IC) + numberOfLine, 1) == -2)
					return "`Dynamic allocatio error.\0";
			}
			if(NumAction<=3 || NumAction == 6)/*check if target or source*/
				nextLine[0]|= 1<<4;
			else
				nextLine[0]|= 1<<2;
			numberOfLine++;
			from = end;
		}
		else
			return "`Syntax error, Symbol tyiped in fisrt action dosn't exist.\0";
		break;
		case 2:/* there is a '.' */
		end = 0;
		while(command[from+end] != '.')
			end++;
		if(end >= SymbolMax)
			return "`Syntax error, too much long text afte action name. Symbol name can't be more then 30 chars.\0";
		for(i=0;i<end;i++)
			SyName[i] = command[from+i];
		SyName[i] = '\0';		
		if((temp = getSymbolIfExist(Head_Symbol,SyName)))
		{
			if(temp->type == 1 || temp->type == 2)
			{
				from += end+1;
				nextLine[numberOfLine]|= (temp->adress)<<2;
				numberOfLine++;
				if(command[from] == '1' || command[from] == '2')
					nextLine[numberOfLine]|= (command[from] - '0')<<2;
				else
					return "`Syntax error, expected 1 or 2 after: '.' in struct name in first operate.\0";
				if(temp->type != 2){/*ENTERNAL symbol*/				
					nextLine[numberOfLine-1]|= 2;
					nextLine[numberOfLine]|= 0;
				}
				else{	/* EXTERNAL symbol*/
					nextLine[numberOfLine-1]|= 1;
					nextLine[numberOfLine]|= 0;
					if( addEntExt(&Head_EAE, temp->name , (*IC) + numberOfLine, 1) == -2)
						return "`Dynamic allocatio error.\0";
				}

				if(NumAction<=3 || NumAction == 6)/*check if target or source*/
					nextLine[0]|= 2<<4;
				else
					nextLine[0]|= 2<<2;				
				numberOfLine++;
				from++;
			}
			else
				return "`Syntax error, expected symbol of struct.\0";
		}
		else
			return "`Syntax error, Symbol (struct) tyiped in fisrt action dosn't exist.\0";
		break;
		case 3:/* command[from] = "r*..." */
		if(NumAction != 6)
		{
			from++;
			if(NumAction<=3)/*check if target or source*/
			{
				nextLine[0]|= 3<<4;
				nextLine[numberOfLine]|= (command[from] - '0')<<6;
			}
			else
			{
				nextLine[0]|= 3<<2;
				nextLine[numberOfLine]|= (command[from] - '0')<<2;
			}
			numberOfLine++;
			from++;
		}
		else
			return "`Syntax error, Addressing number: 0, can't be in first addressing at this action.\0";
		break;
	}
	/*third line*/
	if(NumAction<=3 || NumAction == 6)
	{
		from = indexAfterComma(command,from);
		if(from<0)
			return "`Syntax error, expected a break ',' and another operate in this action.\0";
		from = skipSpace(command,from);
		Addressing2 = addressingType(command,from);
		switch(Addressing2)
		{
			case 0:
			if(NumAction == 1)
			{
				from++;
				end = 0;
				end = checkNumberRange(command,from);
				if(end>0)/*there is a valid number between from to end*/
				{
					result = textToNum(command,from,end-1);
					if(result>128 || result<-128)/* 8 bit + 2 for type of coading*/
						return "`Syntax error, number can be between 128 to -128 in this action.\0";	
					nextLine[numberOfLine]|= ((result<<2)&(-4));
					numberOfLine++;
					from = end;
				}
				else
					return "`Syntax error, expected a number after '#' in second operate.\0";
			}
			else
				return "`Syntax error, Addressing number: 0, can't be in second addressing at this action.\0";
			break;
			case 1:
			end = 0;
			end = endOfWord(command, from);
			if(end - from >= SymbolMax)
				return "`Syntax error, too much long text afte action name. Symbol name can't be more then 30 chars.\0";
			for(i=0;i<end - from;i++)
				SyName[i] = command[from+i];
			SyName[i] = '\0';
			if((temp = getSymbolIfExist(Head_Symbol,SyName)))
			{
				nextLine[numberOfLine]|= (temp->adress)<<2;
				if(temp->type != 2)/* ENTERNAL symbol*/
					nextLine[numberOfLine]|= 2;
				else
				{/* EXTERNAL symbol*/
					nextLine[numberOfLine]|= 1;
					if( addEntExt(&Head_EAE, temp->name , (*IC) + numberOfLine, 1) == -2)
						return "`Dynamic allocatio error.\0";
				}				
				nextLine[0]|= 1<<2;				
				numberOfLine++;
				from = end;
			}
			else
				return "`Syntax error, Symbol tyiped in second operate dosn't exist.\0";
			break;
			case 2:/*there is a '.' */
			end = 0;
			while(command[from+end]!= '.')
				end++;		
			if(end >= SymbolMax)
				return "`Syntax error, too much long text afte break ','. Symbol name can't be more then 30 chars.\0";
			for(i=0;i<end;i++)
				SyName[i] = command[from+i];
			SyName[i] = '\0';		
			if((temp = getSymbolIfExist(Head_Symbol,SyName)))
			{
				if(temp->type == 1 || temp->type == 2)
				{
					from += end+1;
					nextLine[numberOfLine]|= (temp->adress)<<2;
					numberOfLine++;
					if(command[from] == '1' || command[from] == '2')
						nextLine[numberOfLine]|= (command[from] - '0')<<2;
					else
						return "`Syntax error, expected 1 or 2 after: '.' in second operate.\0";
					if(temp->type != 2){/*ENTERNAL symbol*/				
						nextLine[numberOfLine-1]|= 2;
						nextLine[numberOfLine]|= 0;
					}
					else{	/* EXTERNAL symbol*/
						nextLine[numberOfLine-1]|= 1;
						nextLine[numberOfLine]|= 0;
						if( addEntExt(&Head_EAE, temp->name , (*IC) + numberOfLine, 1) == -2)
							return "`Dynamic allocatio error.\0";
					}					
					nextLine[0]|= 2<<2;				
					numberOfLine++;
					from++;
				}
				else
					return "`Syntax error, expected symbol of struct.\0";
			}
			else
				return "`Syntax error, Symbol tyiped in fisrt action dosn't exist.\0";			
			break;
			case 3:/*command[from] = "r*..." */
			from++;
			nextLine[0]|= 3<<2;
			if(Addressing1 == 3)/*check if needed another word*/
			{
				nextLine[numberOfLine-1]|= (command[from] - '0')<<2;
			}
			else
			{
				nextLine[numberOfLine]|= (command[from] - '0')<<2;
				numberOfLine++;
			}
			from++;
			break;
		}
	}
	else
	{
		if(checkEndOfCommand(command,from))
			return "`Syntax error, unexpected text, after operate.\0";
	}
	if(checkEndOfCommand(command,from))
		return "`Syntax error, unexpected text after end of command.\0";
	{
		int i;
		char *bin;
		for(i=0;i<numberOfLine;i++)
		{
			bin = decToBin(nextLine[i]);
			if(bin[0] == '%' || addCodeMethod(&Head_Code,*IC,bin) == -2)
					return "`Dynamic allocatio error.\0";
			(*IC)+=1;
		}
		return NULL;

	}

}

char *EAE(char *command, int from, int end)
{
	static char name[SymbolMax];
	ptSymbol temp = Head_Symbol;
	int Nlength = end - from +1,i,result;
	for(i=0;i<Nlength;i++)
	{
		name[i] = command[from+i];
	}
	name[i] = '\0';	
	while(temp)
	{		
		if(strcmp(temp->name,name) == 0)
		{			
			if(temp->type == 2)
				return "`Syntax error, entery Symbol can't be external one.\0";
			result = addEntExt(&Head_EAE, temp->name, temp->adress, 0);
			if(result == -2)
				return "`Syntax error, Error in dynamic alocation.\0";
			else
				return NULL;
		}
		temp = temp->next;
	}
	return "`Syntax error, Symbol dosn't exist in hole file.\0";
}

void freeAD()
{
	deleteDataList(&Head_AsData);
	deleteAllSymbols(&Head_Symbol);
	deleteListCodeMethod(&Head_Code);
	deleteEntExt(&Head_EAE);
}





