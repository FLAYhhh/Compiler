#include"scanner_generator.h"
#include<stdio.h>
#include<string.h>
#define LINELEN 1024
#define LEXEMLEN 256
int  CLASS_NO=130;
char CLASS_NAME_TABLE[256][20];
//char CLASS_VAL_TABLE[256];

//scanner is a large DFA
ConfigurationSet*
make_scanner(char *regex_path){
    
    FILE *fp;
    if((fp=fopen(regex_path,"r"))==NULL){
        printf("Error:%s not exists",regex_path);
        perror("Error opening file.");
    }
    char buffer[LINELEN];

    NFA *final_nfa,*nfa;
    int line_no=0;
    while(fgets(buffer,LINELEN,fp)!=NULL){
        char dim[]=" \n";
        char *class_name = strtok(buffer, dim);
        char *regex_rule = strtok(NULL, dim);
        strcpy(CLASS_NAME_TABLE[CLASS_NO],class_name);
        if(regex_rule[0]=='$'){
            nfa = construct_single_char_NFA(regex_rule[1]);
            nfa->accept_state->transition = CLASS_NO++;
        }
        else{
            char *post = re2post(regex_rule);
            nfa = post2nfa(post, CLASS_NO++);
        }
        if(line_no==0) final_nfa = nfa;
        else{
            final_nfa = pattern_alternation(final_nfa, nfa);
        }
        line_no++;
    }
    fclose(fp);
    //char *nfa_path = "./file/nfaviz.dot";
    //nfa2file(final_nfa,nfa_path);
    ConfigurationSet *dfa;
    dfa = subset_construction(final_nfa);
    return dfa;
}


void lex(char *src_path, DFA *dfa, char *out_path){
    FILE *srcp;
    FILE *outp;
    if((srcp=fopen(src_path,"r"))==NULL)
        perror("lex:error opening.");
    if((outp=fopen(out_path,"w"))==NULL)
        perror("lex:error opening.");
    char buffer[LEXEMLEN];
    char *bufp=buffer;
    int c;
    Configuration *dstate = dfa->q[0];
    while((c=fgetc(srcp))!=EOF){
        if(dfa->T[dstate->id][c]!=NULL){
            *bufp++ = c;
            dstate = dfa->T[dstate->id][c];
        }
        else{
            *bufp='\0';
            fprintf(outp,"%s %s\n",CLASS_NAME_TABLE[get_class_num(dstate)],buffer);
            bufp = buffer;
            // if(c!=' '&&c!='\n'){
            //     dstate = dfa->T[dfa->q[0]->id][c];
            //     *bufp++ = c;
            // }
            while((c=fgetc(srcp))==' '||c=='\n'||c=='\t'){}
            if(c!=EOF){
                dstate = dfa->T[dfa->q[0]->id][c];
                *bufp++ = c;
            }
        }
    }
    *bufp =0;
    fprintf(outp,"%s %s\n",CLASS_NAME_TABLE[get_class_num(dstate)],buffer);
}

