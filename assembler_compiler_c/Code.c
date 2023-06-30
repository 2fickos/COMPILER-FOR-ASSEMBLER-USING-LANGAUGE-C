#include "Code.h"

int isEmptyCodeMethod(codePointer head){
    if(head==NULL) 
        return 1;
    return -1;
}

int addCodeMethod(codePointer * head ,int address ,char *bin) {
    codePointer newData = (codePointer) calloc(1,sizeof(codeNode));
    codePointer temp;
    if (newData == NULL) { 
        return -2;
    }
    newData->address = address;
    newData->bin = bin;    
    newData->next = NULL;
    if (isEmptyCodeMethod(*head)>0) {

        *head = newData;
        return 1;
    }

    temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newData;
    return 1;


}

codePointer getNextCodeMethod(codePointer current){
    return current->next;
}

char * getBase32CodeMethod(codePointer * head){
    char * base32Method=(char *)calloc(6,sizeof(char));
    codePointer temp=*head;
    char * charTemp;
    if(!base32Method)
        return NULL;
    (*head)=(*head)->next;
    memcpy(base32Method,decToBase32((temp)->address),2);
    base32Method[2]='\t';
    charTemp = base32Method+3;
    memcpy(charTemp,binToBase32((temp)->bin),2);
    base32Method[5]='\0';
    return base32Method;
}

void deleteListCodeMethod(codePointer * head){
    codePointer pt,temp;
	temp = *head;
	while(temp)
	{
		pt = temp->next;		
        free(temp->bin);
		free(temp);
		temp = pt;
	}
	*head = NULL;
}
