#ifndef SCANNER_GENERATOR
#define SCANNER_GENERATOR

#include<stdio.h>
#include"fa_struct.h"
#include"regex_engine.h"


ConfigurationSet* make_scanner(char *regex_path);

void lex(char *src_path, DFA *dfa, char* out_path);

//#define NUMBER 130
//#define IDENTIFIER 131

#endif