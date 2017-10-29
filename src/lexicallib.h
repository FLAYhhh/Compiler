#ifndef LEXICALLIB
#define LEXICALLIB

#include "nfa_struct.h"
char* re2post(char *re);
NFA *post2nfa(char *post);
ConfigurationSet *nfa2dfa(NFA *nfa);




void nfa2file(NFA *nfa, FILE *fp);
void dfa2file(DFA *dfa, FILE *fp );
#endif