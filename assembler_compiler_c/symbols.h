typedef struct nodeSymbol * ptSymbol;
typedef struct nodeSymbol{ 
 	char *name;
 	int adress;/*in decimal*/
 	short type;/*0,1,2 or 3*/ 
	ptSymbol next;
 }nodeSymbol; 

int setNextSymbol(ptSymbol* head, char *command, int from, int length, int adress, short type);/*gets all data needed for new symbol, and head of list, adding symbol to list*/
int isSymbolExist(ptSymbol head, char *name);/*checks if the symbol exist in the list.*/
ptSymbol getSymbolIfExist(ptSymbol head, char *name);/*check if the smbol exist and return apointer to the node.*/
int isSymbolListEmpty(ptSymbol head);/*checks if the list is empty*/
ptSymbol getNextSymbol(ptSymbol* head);/*returns the next node*/
void deleteAllSymbols(ptSymbol* head);/*free symbol list*/
int addSymbols(ptSymbol* head, char *command, int from);/*gets head of list, command and index, adding the Symbol to the list*/
void updateSymbolsIC(ptSymbol head, int IC);/*updating all symbols address.*/

