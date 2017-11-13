#include<stdlib.h>
#include<stdio.h>
#include"regex_engine.h"
//#include"fa_struct.h"
#include"scanner_generator.h"
void single_regex_test1();
void mul_test();
int main(){
    //single_regex_test1();
    mul_test();
    return 0;
}
void mul_test(){

    //====================Test 2=====================================
    char *regex_path = "./file/regex.txt";
    DFA* dfa = make_scanner(regex_path);
    char *dfa_path = "./file/dfaviz.dot";
    dfa2file(dfa,dfa_path);

    char *src_path = "./file/sourcefile.txt";
    char *token_path = "./file/tokens.txt";
    lex(src_path, dfa, token_path);

}

void single_regex_test1(){
    //==============================Test 1================================
    FILE *regex_fp;
    if((regex_fp = fopen("./file/single_regex.txt","r")) == NULL)
        printf("Cannot open regex.txt: file not exists.");
    char regex[1024];
    fgets(regex,1024,regex_fp);
    fclose(regex_fp);

    char *regex_postform = re2post(regex);
    NFA *nfa = post2nfa(regex_postform,130);

    /*print nfa to file*/
    char *nfaviz_path="./file/nfaviz.dot";
    nfa2file(nfa, nfaviz_path);

    
    ConfigurationSet *dfa = subset_construction(nfa);
    
    /*print dfa to file*/
    char *dfaviz_path = "./file/dfaviz.dot";
    dfa2file(dfa, dfaviz_path);
    
    
    // Configuration *q = new_Configuration();
    // q->id = 0;
    // q->size = 1;
    // q->states[0] = nfa->start_state->outp1;

    // q = epsilon_closure(q);
    FILE *src_fp;
    if((src_fp = fopen("./file/single_src.txt","r")) ==NULL)
        printf("Cannot open sourcefile.txt: file not exists.");
    char src[100];
    fgets(src,100,src_fp);
    fclose(src_fp);

    Configuration *end_dstate = walk_through_DFA(dfa, src);
    if(get_class_num(end_dstate))
        printf("%s is valid.\nClasses is %d\n",src,get_class_num(end_dstate));
    else{
        printf("%s is not valid.\n",src);
    }
}