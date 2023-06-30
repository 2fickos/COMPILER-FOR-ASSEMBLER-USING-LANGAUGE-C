
char *EAE(char *command, int from, int end);/*checks Symbol list to find symbol, adding it to EAE list */
int createFiles(const char *fileName);/*gets file name, create and right the 3 output files.*/
int addDTC(int IC);	/*adding data List to code*/
char *commandCoding(int NumAction, char *command, int from,int *IC);/*gets command and the method number. return error if exist*/
void freeAD();/*free all lists*/

