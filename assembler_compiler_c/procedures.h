char * countLinesNum (int actionNum, int *L, char *command, int from); /*gets action number, variable, command and index, set the variable to number of lines nedded*/

int addressingType(char *command, int from); /*gets command and index, returns addrressing method number (0,1,2,3).*/

char *binSymbolAddress(char *command, int from, int end, ptSymbol head); /*gets action number, pointer to list, command and indexs , returns array including the addrissing of the addrressing method number in binary*/
