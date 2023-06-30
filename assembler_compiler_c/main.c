#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "passing.h"
#include "inputs.h"
#include "bases.h" 
#include "utils.h"
#define ErrorJump \
flagError = ON;\
free(command);\
continue;

#define ErrorJump \
flagError = ON;\
free(command);\
continue;
enum Status {OFF,ON};
short flagError;
char syName[commanSize];




int main(int argc, char const *argv[])
{	
	int i;
	char *fullFileName = NULL;	
	FILE * file;
	if(argc <= 1)
	{
		printf("No File where typed.\n");
		exit(0);
	}
	for	(i=1; i<argc; i++)
	{			
		fullFileName =NULL;
		fullFileName = connectchars(argv[i],".as\0");
		if(fullFileName)
		{
			printf(" -- File: %s -- \n",fullFileName);
			file = fopen(fullFileName, "r");
			if (file)
			{
				pass1(file);
				if(flagError == ON)
				{					
					printf(" -- File: %s couldn't be created becuse there are error to fix in it. --\n",fullFileName);
					fclose(file);
					free(fullFileName);
					freeAD();
					continue;
				}
				rewind(file);
				pass2(file);
				if(flagError == ON)
				{					
					printf(" -- File: %s couldn't be created becuse there are error to fix in it. --\n",fullFileName);
					fclose(file);
					free(fullFileName);
					freeAD();
					continue;
				}
				if(createFiles(argv[i]) != 1)
				{
					printf("Error: could not creat output files to: %s\n",fullFileName );
				}
				else
				{
					printf(" -- Successfully created all output files for %s -- \n",fullFileName);
				}
			   	fclose(file);
			}else{
				printf("Error: The file %s.as dose not exists.\n", argv[i] );
			}
			free(fullFileName);
			freeAD();			
		}
		else
		{
			printf("Dynamic alocation error: couldn't creat file: %s\n", argv[i]);
		}
	}
	return 1;
}







