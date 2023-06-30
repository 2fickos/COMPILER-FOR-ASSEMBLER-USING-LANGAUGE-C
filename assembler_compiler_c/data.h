#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include <ctype.h>
#include "checkSyntax.h"
#include "bases.h"
#include "inputs.h"
#ifndef directiveSize
#define directiveSize
#define stringSize 7
#define structSize 7
#define dataSize 5
#define externSize 7
#define entrySize 7
#endif
#define MAX_RANGE 512


typedef struct dataList * dataPointer;
typedef struct dataList{
    int data; /* in decimal */
    int DC; /*in decimal*/
    dataPointer next;

} dataList;



int addNode(dataPointer *head , int number, int * DC);  /*adding the given data in abinary base to the dataList ,adds 1 to the given DC*/

int addNunDirective(char * command,int from,dataPointer * head,int * DC);

int addString(char *command, int from, int *DC, dataPointer *head);/* addrissing the command line after the string word, quotation and spaces Until the next quotation and adding it to the datalist, and updating DC accordingly*/

int addStruct(char *command, int from, int *DC, dataPointer *head); /* addrissing the command line after the struct word, quotation and spaces Until the next quotation and adding it to the datalist, and updating DC accordingly*/

int addData(char *command, int from, int *DC, dataPointer*head); /* addrissing the command line after the data word, quotation and spaces Until the next quotation and adding it to the datalist, and updating DC accordingly*/

void deleteDataList(dataPointer * head);  /*frees the dataList*/
void printDataList(dataPointer head);  /*prints the list*/

