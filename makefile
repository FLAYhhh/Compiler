build: lexical.o  scanner_generator.o regex_engine.o fa_struct.o  
	gcc -o  ./bin/build  ./obj/lexical.o  ./obj/scanner_generator.o ./obj/regex_engine.o  ./obj/fa_struct.o 
lexical.o: ./src/lexical.c 
	gcc -c -g ./src/lexical.c -o  ./obj/lexical.o

regex_engine.o: ./src/regex_engine.c 
	gcc -c -g ./src/regex_engine.c -o  ./obj/regex_engine.o

fa_struct.o: ./src/fa_struct.c 
	gcc -c -g ./src/fa_struct.c -o ./obj/fa_struct.o

scanner_generator.o: ./src/scanner_generator.c
	gcc -c -g ./src/scanner_generator.c -o ./obj/scanner_generator.o

clean:
	rm ./bin/build ./obj/regex_engine.o ./obj/lexical.o ./obj/fa_struct.o   ./obj/scanner_generator.o

.PHONY:clean