#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#ifndef mask 
#define mask 1
#endif




int textToNum(char *text, int start, int finish);  /* converting a text to number within a given range */
char * decToBin(int num);	  /* converting from Decimal base to binary base  */
int binToDec(char *bin);   /* converting from binary base to Decimal base*/
char *binToBase32(char *bin);       /* converting from binary base to Our new base */
char * decToBase32(int num);	  /* converting from Decimal base to Our new base */
char* connectchars(const char *s1, const char *s2);       /*linking to strings to one string */


