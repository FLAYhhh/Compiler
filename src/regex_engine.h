#ifndef LEXICALLIB
#define LEXICALLIB
#include<stdio.h>
#include "fa_struct.h"
char* re2post(char *re);
NFA *post2nfa(char *post, int classes);
ConfigurationSet *nfa2dfa(NFA *nfa);


void nfa2file(NFA *nfa, char *path);
void dfa2file(DFA *dfa, char *path);
#endif