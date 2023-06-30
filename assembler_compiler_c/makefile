all: play

play : main.o passing.o inputs.o symbols.o entryAndExtern.o Code.o checkSyntax.o bases.o data.o procedures.o utils.o
	gcc -g -ansi -Wall -pedantic main.o passing.o inputs.o symbols.o entryAndExtern.o Code.o checkSyntax.o bases.o data.o procedures.o utils.o -lm -o play

main.o : main.c passing.h inputs.h bases.h utils.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o

passing.o : passing.c passing.h inputs.h symbols.h entryAndExtern.h Code.h checkSyntax.h bases.h data.h procedures.h utils.h
	gcc -c -ansi -Wall -pedantic passing.c -o passing.o

inputs.o: inputs.c inputs.h
	gcc -c -ansi -Wall -pedantic inputs.c -o inputs.o

symbols.o : symbols.c symbols.h inputs.h
	gcc -c -ansi -Wall -pedantic symbols.c -o symbols.o

entryAndExtern.o: entryAndExtern.c 
	gcc -c -ansi -Wall -pedantic entryAndExtern.c -o entryAndExtern.o

checkSyntax.o: checkSyntax.c checkSyntax.h
	gcc -c -ansi -Wall -pedantic checkSyntax.c -o checkSyntax.o

Code.o: Code.c bases.h
	gcc -c -ansi -Wall -pedantic Code.c -o Code.o

bases.o: bases.c bases.h
	gcc -c -ansi -Wall -pedantic bases.c -o bases.o

data.o: data.c data.h bases.h checkSyntax.h inputs.h
	gcc -c -ansi -Wall -pedantic data.c -o data.o

procedures.o: procedures.c procedures.h checkSyntax.h inputs.h symbols.h bases.h
	gcc -c -ansi -Wall -pedantic procedures.c -o procedures.o

utils.o: utils.c inputs.h symbols.h entryAndExtern.h Code.h checkSyntax.h bases.h data.h procedures.h utils.h
	gcc -c -ansi -Wall -pedantic utils.c -o utils.o
	
clean:
	rm *.o *~
