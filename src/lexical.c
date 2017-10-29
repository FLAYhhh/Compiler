/*
 * Regular expression -> NFA  
 * Thompson's algorithm
 */

#include<stdlib.h>
#include<stdio.h>
#include"lexicallib.h"
#include"nfa_struct.h"

int main(){
    FILE *regex_fp;
    if((regex_fp = fopen("./file/regex.txt","r")) == NULL)
        printf("Cannot open regex.txt: file not exists.");
    char regex[100];
    fgets(regex,100,regex_fp);
    fclose(regex_fp);

    char *regex_postform = re2post(regex);
    NFA *nfa = post2nfa(regex_postform);

    /*print nfa to file*/
    FILE *nfa_fp;
    if((nfa_fp = fopen("./file/nfaviz.dot","w"))==NULL)
        perror("./file/nfaviz.dot");
    nfa2file(nfa, nfa_fp);
    fclose(nfa_fp);

    
    ConfigurationSet *dfa = subset_construction(nfa);
    
    /*print dfa to file*/
    FILE *dfa_fp;
    if((dfa_fp = fopen("./file/dfaviz.dot","w"))==NULL)
        perror("./file/dfaviz.dot");
    dfa2file(dfa, dfa_fp);
    fclose(dfa_fp);
    
    
    // Configuration *q = new_Configuration();
    // q->id = 0;
    // q->size = 1;
    // q->states[0] = nfa->start_state->outp1;

    // q = epsilon_closure(q);
    FILE *src_fp;
    if((src_fp = fopen("./file/sourcefile.txt","r")) ==NULL)
        printf("Cannot open sourcefile.txt: file not exists.");
    char src[100];
    fgets(src,100,src_fp);
    fclose(src_fp);

    Configuration *end_dstate = walk_through_DFA(dfa, src);
    if(is_accept(end_dstate))
        printf("%s is valid.\n",src);
    else{
        printf("%s is not valid.\n",src);
    }
    return 0;
}


/*==================  NFA -> DFA  ===================================*/

/*
 * genarate the epsilon-closure of a state (not include itself) 
 */


/*
 *Function:get the epsilon-closure of a single state s 
 *Arguments:State s  -  a single state in the nfa
 *Return value: a list of states represent the epsilon-closure of s
 */

// State_list *
// closure_of_sgle_state(State *s){
//     State_list *list;
//     if(s->c != EPSILON ){
//         list = (State_list *)malloc(sizeof(State_list));
//         list->count = 0;    
//         list->l[list->count++] = s;
//     }
        
//     else{
//         State_list *list1 = (State_list *)malloc(sizeof(State_list));
//         State_list *list2 = (State_list *)malloc(sizeof(State_list));
//         list1->count = 0;
//         list2->count = 0;

//         list1 = closure_of_sgle_state(s->outp1);
//         list2 = closure_of_sgle_state(s->outp2);
//         list = list_concat(list1, list2);
//         free(list2);

//         list_absorb(list, s);
//     }
//     return list;
// }

// /*
//  *Function: to get epsilon-closure of a set of states
//  *Arguments: a State_list type, represents a set of states
//  *Return val: the epsilon-closure of the set
//  *Caution: the function changes the original list,that is , it extends the original list
//  *         to its epsilon-closure.
//  */
// State_list *
// closure_of_stateSet(State_list *set){
//     int i, state_num=set->count;
//     State_list *list, *temp;
//     for(i=0;i<state_num;i++){
//         temp = closure_of_sgle_state(set->l[i]);
//         list = list_concat(set, temp);
//         free(temp);
//     }

//     return list;
// }


// /*
//  *Function: given a set of nfa_state and a input character, it generates a set of 
//  *          states that are reachable by the nfa_state_set direclty(without epsilon
//  *          -closrue)
//  *Arguments: a dfa state, a input character c
//  *Return val: a generated set of nfa state
//  *
//  */
// State_list *
// directly_trans_set_by_char(State_list *dstate, int c){
//     int i, ifnull=1;
//     State_list *list = (State_list *)malloc(sizeof(State_list));
//     for(i=0;i<dstate->count;i++){
//         if(dstate->l[i]->c==c){
//             ifnull = 0;
//             list_absorb(list, dstate->l[i]->outp1);
//             list_absorb(list, dstate->l[i]->outp2);
//         }
//     }
//     if(ifnull==1) return NULL;
//     return list;
// }

// /*
//  *Function: generate the next dfa state 
//  *Arguments: a "from" dfa-state, an input character
//  *Return val: the next dfa-state
//  */
// int  
// ptrcmp(const void *a, const void *b){
//     return (*(State**)a - *(State**)b);
// }

// State_list *
// transition(State_list *dstate, int c){
//     State_list *list;
//     list = directly_trans_set_by_char(dstate, c);
//     if(list==NULL) return NULL;
//     list = closure_of_stateSet(list);
//     //dstate->trans[c] = list;
//     qsort(list->l,list->count, sizeof(list->l[0]), ptrcmp);
//     return list;
// }

// /*
//  * now it's time to generate the complete dfa
//  */
// State_list *D_state_set[500];
// int cur=0,count=0;

// /*
//  *Funtions:
//  *Arguments: the start state of nfa
//  */
// void  
// complete(State *q){
//     D_state_set[0] = closure_of_sgle_state(q);
//     int i;
//     for(i=0;i<128;i++){
//         D_state_set[0]->trans[i]=NULL;
//     }
    
//     count = 1;
//     int c;
//     State_list *newstate;
//     for(;cur<count;cur++){
//         for(c=0;c<128;c++){
//             newstate = transition(D_state_set[cur],c); //type state_list(dstate)
//             if(newstate!=NULL && !inDstateSet(D_state_set, newstate)){
//                 for(i=0;i<128;i++){
//                     D_state_set[0]->trans[i]=NULL;
//                 }
//                 D_state_set[cur]->trans[c] = newstate;
//                 DstateSet_append(D_state_set, newstate);
//             }
//         }
//     }
// }


// int 
// state_equal(State_list *l1, State_list *l2){
//     int i;
//     for(i=0;i<l1->count;i++){
//         if(l1->l[i]!=l2->l[i])
//             return 0;
//     }
//     return 1;
// }

// int 
// inDstateSet(State_list **D_state_set, State_list *s){
//     int i;
//     for(i=0;i<count;i++){
//         if(state_equal(D_state_set[i], s))
//             return 1;
//     }
//     return 0;
// }



// /*
//  *Arguments:dfa-state-set, new dfa-state(could be NULL)
//  */
// void 
// DstateSet_append(State_list **D_state_set,State_list *s){
//     if(s!=NULL)
//         D_state_set[count++] = s;
// }












// /*
//  * append a State to a State_list
//  * algorithm to detect the repetition of states need to be optimized
//  */

// int inlist(State_list *list, State *s){
//         int i,equal_flag=0;
        
//         for(i=0;i<list->count;i++){
//            if(list->l[i]==s)
//                 equal_flag = 1;
//                 return 1;   // state is in list
//         }
//         return 0; // not in list
// }


// void 
// list_absorb(State_list *list, State *s){
//     if(s!=NULL){
//         if(!inlist(list, s))
//             list->l[list->count++] = s;
//     }
// }




// /*
//  * concatenate State_list
//  * the concatenated list is list1
//  */
// State_list *
// list_concat(State_list *list1, State_list *list2){
//    // State_list *list = (State_list *)malloc(sizeof(State_list));
//     int i,j;
//     for(i = 0; i<list2->count; i++){
//         list_absorb(list1, list2->l[i]);        
//     }

//     return list1;
// }

// void 
// mark_final(State_list **D_state_set, NFA *nfa){
//     int i,j;
//     for(i=0;i<count;i++){
//         D_state_set[i]->islast = 0;
//         for(j=0;j<D_state_set[i]->count;j++){
//             if(D_state_set[i]->l[j] == &(nfa->f)){
//                 D_state_set[i]->islast = 1;
//                 continue;
//             }
//         }
//     }
// }



// int 
// run_dfa(State_list **D_state_set, char *str){
//     State_list  *state=D_state_set[0];
//     while(*str++){
//         if((state = state->trans[*str])==NULL )
//             return 0;
//     }
//     if(state->islast==1)
//         return 1;
//     else return 0;
// }


// int main(){
//     FILE *fp;
//     char re[100];
//     if((fp = fopen("regex.txt","r"))==NULL){
//         printf("\nCannot open file.\n");
//     }
//     fgets(re,100,fp);
//     char *postfix = re2post(re);
//     printf("%s\n",postfix);

//     NFA *nfa;
//     nfa = post2nfa(postfix);
    
//     //nfa to dfa
//     complete(&(nfa->s));
//     mark_final(D_state_set, nfa);

//     FILE *fp1;
//     char str[100];
//     if( (fp1 = fopen("input.txt", "r"))==NULL )
//         printf("Cannot open file.\n");
    
//     fgets(str, 100, fp1);

//     int res = run_dfa(D_state_set, str);
//     printf("%d",res);
//     return 0;
// }






