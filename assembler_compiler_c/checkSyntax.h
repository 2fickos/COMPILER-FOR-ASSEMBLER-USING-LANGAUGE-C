#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "inputs.h" 


int checkLabel(char *command, int from);/* checks if there is a label according to the instructions and return the index of the end of the label*/ 

int checkDirective (char *command, int from);/* checks if there is a directive according to the instructions and return the index of the end of the label*/ 

int checkAction (char *command, int from); /* checks if there is an action according to the instructions from agiven index and return the number of the action according to the list*/
int checkNumberRange(char *command, int from);/* checks if there is a number from agiven index */ 
