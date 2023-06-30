#include "data.h" 



int addNode(dataPointer *head , int number, int *DC){
    dataPointer newData = (dataPointer)malloc(sizeof(dataList));
    dataPointer temp;
    if(newData == NULL){
        return -2;
    }
    newData->data=number;
    newData->DC=*DC;
    newData->next=NULL;
    *DC+=1;
    if(*head==NULL){
        *head=newData;
        return 1;
    }

    temp=*head;
    while(temp->next!=NULL){
        temp=temp->next;
    }
    temp->next=newData;
    return 1;

}
int addNunDirective(char * command,int from,dataPointer * head,int * DC){
    int step=from;
    int added;
    if(command[step]!='\"')
        return -1;
    step++;
    while(command[step]!='\"' && checkEndOfCommand(command,step)) {
        if ((added = addNode(head, command[step], DC)) < 0)
            return added;
        step++;
    }
    if(command[step] != '\"')
        return -1;
    if((added = addNode(head,'\0',DC))<0)
        return added;
    return step+1;
}
int addString(char *command, int from, int *DC, dataPointer *head){
    int step=from+stringSize;    
    step=skipSpace(command,step);
    return addNunDirective(command,step,head,DC);
}

int addStruct(char *command, int from, int *DC, dataPointer *head) {
    int step = from+structSize;
    int endNum;
    int num;
    int endStruct;    
    step = skipSpace(command, step);
    if ((endNum = checkNumberRange(command, step)) < 0) {
        return -1;
    }    
    {
	    int newNumber = textToNum(command,step,endNum-1);        
	    
	    if(newNumber>MAX_RANGE || newNumber<-(MAX_RANGE))
		num = -1;
	    else if ((num = addNode(head, newNumber, DC) < 0))
		num = num + 0;
	    else num = 1;
    }
    if(num<0)
        return num;
    step=endNum;
    step=skipSpace(command,step);
    if(command[step]!=',')
        return -1;
    step=skipSpace(command,step+1);
    if(command[step]!='\"')
        return -1;
    if((endStruct=addNunDirective(command,step,head,DC))<0)
        return endStruct;    
    return endStruct;
}

int addData(char *command, int from, int *DC, dataPointer*head){
    int step=from+dataSize;
    int endNum;
    int number;    
    step=skipSpace(command,step);
    if(!checkEndOfCommand(command,step))
        return -1;
    if((endNum = checkNumberRange(command,step))<0)
        return endNum;    

    {	
    	    
	    int newNumber = textToNum(command,step,endNum-1);        
	    number = addNode(head, newNumber, DC);
	    if(newNumber>MAX_RANGE || newNumber<-(MAX_RANGE))
		number = -1;
	
	    
	    else if (number < 0)
		number = number +0;
	    else number =1;
    
    }
    if(number<0)
        return number;
    step=endNum;    
    while(checkEndOfCommand(command,step)) {
        step=skipSpace(command,step);
        if(command[step]!=',')
            return step;
        step=skipSpace(command,step+1);
        if((endNum = checkNumberRange(command,step))<0)
            return endNum;        
       
         {	
    	    
	    int newNumber = textToNum(command,step,endNum-1);        
	    number = addNode(head, newNumber, DC);
	    if(newNumber>MAX_RANGE || newNumber<-(MAX_RANGE))
		number = -1;
	
	    
	    else if (number < 0)
		number = number +0;
	    else number =1;
    
         }
        if(number<0)
            return number;
        step=endNum;        
    }
    return step;
}

void deleteDataList(dataPointer * head){    
    dataPointer pt,temp;
    temp = *head;
    while(temp)
    {
        pt = temp->next;        
        free(temp);
        temp = pt;
    }
    *head = NULL;
}

void printDataList(dataPointer head){
    dataPointer temp = head;
    while(temp){
        printf("(%d,%d)\n",temp->data,temp->DC);
        temp=temp->next;
    }
}


