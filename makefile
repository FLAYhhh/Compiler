build: lexical.o lexicallib.o nfa_struct.o
	gcc -o  ./bin/build  ./obj/lexical.o  ./obj/lexicallib.o  ./obj/nfa_struct.o

lexical.o: ./src/lexical.c 
	gcc -c -g ./src/lexical.c -o  ./obj/lexical.o

lexicallib.o: ./src/lexicallib.c 
	gcc -c -g ./src/lexicallib.c -o  ./obj/lexicallib.o

nfa_struct.o: ./src/nfa_struct.c 
	gcc -c -g ./src/nfa_struct.c -o ./obj/nfa_struct.o

clean:
	rm ./bin/build ./obj/lexical.o ./obj/lexicallib.o ./obj/nfa_struct.o

.PHONY:clean