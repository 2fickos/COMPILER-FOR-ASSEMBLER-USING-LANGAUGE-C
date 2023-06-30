#ifndef commanSize
#define commanSize 81 
#endif
char *getCommand(FILE *);/*gets pointer to file and return the comand in 81 lenght array*/
char getNextChar(FILE *);/*gets pointer to file and returns the next char*/
void setNextChar(char c);/*gets pointer to file, and sets the next char.*/
int skipSpace(char *command, int from);/*gets pointer to arry, and starting index ,returns index after all the sapaces*/
int indexAfterComma(char *command, int from);/*gets pointer to arry, and starting index ,returns index after the comma*/
int checkEndOfCommand(char *command, int from);/*gets pointer to arry, and starting index' checks if there is more text left*/
int endOfWord(char *command, int from);/*gets pointer to arry, and starting index, returns index of the end of word */

