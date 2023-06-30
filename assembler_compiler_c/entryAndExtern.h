typedef struct EAEList * ptEAEList; /*Pointer to node*/
typedef struct EAEList
{
	short isExt;/*0 or 1*/
	char *name;/*name of label*/
	char *address;/*address of label in Base32*/
	ptEAEList next; /*Pointer to next*/
}EAEList;


int addEntExt(ptEAEList * head ,char *name , int adress,short isExt); /*adding anew node including all the given data to the EAEList*/
int isEntExtEmpty(ptEAEList head);/*checks if the EAEList is empty*/
ptEAEList getNextEntExt(ptEAEList after); /*returns pointer to next node*/
char * getEntExtList (ptEAEList *head);/*returns head of list as a string to print to file, and delete head of file.*/
void deleteEntExt (ptEAEList *head);/*free the EAEList*/
void printEntExtList(ptEAEList head);/*print the list*/

