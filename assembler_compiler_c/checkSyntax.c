#include "checkSyntax.h"
#define lenMaxLabel 30

#if !defined (commanSize)
	#define commanSize 81 
#endif


char *directives[5] = {"data","string","struct","entry","extern"};
char *actions[16]  = {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","get","prn","jsr","rts","hlt"};



int checkNumberRange(char *command, int from)
{
    int start = from ,end = 0;
    if(start+end < commanSize && (command[start+end] == '-' || command[start+end] == '+'))
        start++;
    while (command[start+end] != '\0' && command[start+end]>='0' && command[start+end]<='9'){        
        end++;
    }
    return (end == 0)? -1: start+end;
}


int checkLabel(char *command, int from){
    int step=1;
    if (isalpha(command[from])){
        while(isalpha(command[from+step]) || isdigit(command[from+step])){
            if(step>lenMaxLabel){
                return -1;
            }
            step++;
        }
        if (command[from+step] == ':')
            return from+step;
    }
    return -1;
}



int checkAction (char *command, int from){
    int step=0;
    char action[4];
    int i;    
    for (;step<3;){
        action[step]=command[from+step];
        step++;
    }    
    action[step]='\0';
    for (i=0;i<16;i++){
        if(strcmp(action,actions[i])==0){
            return i;
        }
    }    
    return -1;
}

int checkDirective (char *command, int from){
    int step = 1;
    char directive[7];
    int i;    
    if(command[from] == '.'){        
        while((command[from+step] != ' ') && (command[from+step] !='\t') && (command[from+step] !='\0')){
            if(step>=7)
                return -1;
            directive[step-1]=command[from+step];
            step++;
        }        
        directive[step-1]='\0';
        for (i=0;i<5;i++){
            if(strcmp(directive,directives[i])==0){
                return i+1;
            }
        }
        return -1;
    }    
    return -1;
}


