#include<stdlib.h>
#include<stdio.h>
#include "lexicallib.h"

/*
** Convert infix regular expression to postfix form
** Supported operators : (  )  |  * 
** Priority : ( = ) > * > |  
*/
char* 
re2post(char *re){
    #define in(c) ( *queuep++ = c)
    #define push(c) (*stackp++ = c)
    #define pop() (--stackp)
    #define top() (*(stackp-1))

    static char ch_queue[100];
    char op_stack[100] , *queuep = ch_queue, *stackp = op_stack;
    char ch;
    int flag=1; // mark the encounted ops. used to find out cancat operator. chars are connnect by ops,
    while(ch = *re++){
        switch(ch){
            default: 
                if(flag==0){ //case '.'
                    while(top()=='*' || top()=='.'){
                        in(top());
                        pop();
                    }
                    push('.');
                }
                flag = 0;
                in(ch);
                break;
            case '(': 
                if(flag == 0){   //case '.'
                    while(top()=='*'|| top()=='.'){
                        in(top());
                        pop();
                    }
                    push('.');
                }             
                push(ch);
                flag = 1;
                break;
            case '|':
                while(top()=='|'||top()=='.'||top()=='*'){
                    in(top());
                    pop();
                }
                push(ch);
                flag = 1;
                break;
            case '*':
                in(ch);
                break;
            case ')':
                while(top()!='('){
                    in(top());
                    pop();
                }
                pop();
                break;
        }
    }
    while((stackp-1)>=op_stack){
        in(top());
        pop();
    }

    return ch_queue;
}


/**
 ** Convert regular expression in postfix form to NFA
**/
NFA *
post2nfa(char *post){
    NFA *Stack[100] , **stackp = Stack;
    
    #define push2(e) (*stackp++ = e)
    #define pop2()  (*--stackp)

    char sym;
    NFA *e1, *e2;

    while(sym = *post++){
        switch(sym){
            default:
                push2(construct_single_char_NFA(sym));
                break;
            case '|':
                e2 = pop2();
                e1 = pop2();
                push2(alternation(e1, e2));
                break;
            case '.':
                e2 = pop2();
                e1 = pop2();
                push2(concatenation(e1, e2));
                break;
            case '*':
                e1 = pop2();
                push2(kleene(e1));
                break;
        }
    }
    NFA *nfa = pop2();
    return nfa;
}

ConfigurationSet *
dfa2nfa(NFA *nfa){
    return subset_construction(nfa);
}


void nfa2file(NFA *nfa, FILE *fp){
    fprintf(fp,"digraph g{\n");
    // fprintf(fp,"\trankdir=LR;\n");
    // fprintf(fp,"\tsize=\"8.5\"\n");
    fprintf(fp,"\tnode [shape = circle];\n");
    int i;
    for(i=0;i<nfa->edge_num;i++){
        if(nfa->edges[i]->character <128)
            fprintf(fp,"\t%d -> %d [label = \"%c\"]\n",nfa->edges[i]->from_state, 
                                            nfa->edges[i]->to_state, 
                                            nfa->edges[i]->character);
        else if(nfa->edges[i]->character==128)
            fprintf(fp,"\t%d -> %d [label = \"%s\"]\n",nfa->edges[i]->from_state, 
                                            nfa->edges[i]->to_state, 
                                            "EPSILON");

    }
    fprintf(fp,"}");
}


void dfa2file(DFA *dfa, FILE *fp ){
    fprintf(fp,"digraph g{\n");
    fprintf(fp,"\trankdir=LR;\n");
    fprintf(fp,"\tsize=\"8.5\"\n");
    fprintf(fp,"\tnode [shape = circle];\n");
    int i;
    for(i=0;i<dfa->edge_num;i++){
        fprintf(fp,"\t%d -> %d [label = \"%c\"]\n",dfa->edges[i]->from_state, 
                                            dfa->edges[i]->to_state, 
                                            dfa->edges[i]->character);

    }
    fprintf(fp,"}");
}