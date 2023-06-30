#include <stdio.h>
#include <stdlib.h>
#include "bases.h"

typedef struct codeList * codePointer;
typedef struct codeList{
    int address;             /* in decimal*/
    char *bin;          /*array represents a binary number*/
    codePointer next;
} codeNode;

 
int isEmptyCodeMethod(codePointer head);/*checks if the list is Empty*/                          
                                                                  
int addCodeMethod(codePointer * head ,int address ,char *bin); /*adding a new node with the given data to the end of the codeList*/

codePointer getNextCodeMethod(codePointer current);  /*returns a pointer to the next node */
                                                                
char * getBase32CodeMethod(codePointer * head);  /*  */
                                                
void deleteListCodeMethod(codePointer * head); /*frees the codeList*/
                                                               
                                                                 
