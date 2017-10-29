#include<stdlib.h>
#include<stdio.h>
#include"nfa_struct.h"

//  int main(){
//      NFA *e1 = construct_single_char_NFA('a');
//      NFA *e2 = construct_single_char_NFA('b');
//      //NFA *alt = alternation(e1,e2);
//      NFA *cat = concatenation(e1,e2);
//      return 0;
//  }

NFA *
construct_single_char_NFA(char character){
    NFA *nfa;
    INIT_NFA(nfa);

    nfa->start_state->id = static_nstate_id++;
    nfa->start_state->transition = character;
    nfa->start_state->outp1 = nfa->accept_state;
    nfa->start_state->outp2 = NULL;

    nfa->accept_state->id = static_nstate_id++;
    nfa->accept_state->transition = NONE;
    nfa->accept_state->outp1 = NULL;
    nfa->accept_state->outp2 = NULL;

    nfa->edge_num = 1;
    nfa->edges[0] = (NFA_edge*)malloc(sizeof(NFA_edge));
    nfa->edges[0]->from_state = nfa->start_state->id;
    nfa->edges[0]->to_state = nfa->accept_state->id;
    nfa->edges[0]->character = character;

    return nfa;
}

NFA *
alternation(NFA *e1 , NFA *e2){
    NFA *nfa;
    INIT_NFA(nfa);
    nfa->start_state->id = static_nstate_id++;
    nfa->start_state->transition = EPSILON; 
    nfa->start_state->outp1 = e1->start_state;
    nfa->start_state->outp2 = e2->start_state;

    nfa->accept_state->id = static_nstate_id++;
    nfa->accept_state->transition = NONE;
    nfa->accept_state->outp1 = NULL;
    nfa->accept_state->outp2 = NULL;

    e1->accept_state->transition = EPSILON; 
    e1->accept_state->outp1 = nfa->accept_state;
    
    e2->accept_state->transition = EPSILON;
    e2->accept_state->outp1 = nfa->accept_state;

    int edge_sum = e1->edge_num + e2->edge_num;
    int i;
    for(i=0;i<e1->edge_num;i++)
        nfa->edges[i] = e1->edges[i];
    for(i=0;i<e2->edge_num;i++)
        nfa->edges[i+e1->edge_num] = e2->edges[i];
    
    nfa->edges[edge_sum] = new_NFA_edge();
    nfa->edges[edge_sum]->from_state = nfa->start_state->id;
    nfa->edges[edge_sum]->to_state = nfa->start_state->outp1->id;
    nfa->edges[edge_sum]->character = EPSILON;

    nfa->edges[edge_sum+1] = new_NFA_edge();
    nfa->edges[edge_sum+1]->from_state = nfa->start_state->id;
    nfa->edges[edge_sum+1]->to_state = nfa->start_state->outp2->id;
    nfa->edges[edge_sum+1]->character = EPSILON;


    nfa->edges[edge_sum+2] = new_NFA_edge();
    nfa->edges[edge_sum+2]->from_state = e1->accept_state->id;
    nfa->edges[edge_sum+2]->to_state = nfa->accept_state->id;
    nfa->edges[edge_sum+2]->character = EPSILON;



    nfa->edges[edge_sum+3] = new_NFA_edge();
    nfa->edges[edge_sum+3]->from_state = e2->accept_state->id;
    nfa->edges[edge_sum+3]->to_state = nfa->accept_state->id;
    nfa->edges[edge_sum+3]->character = EPSILON;

    nfa->edge_num = edge_sum+4;

    return nfa;
}

NFA *
concatenation(NFA *n1 , NFA *n2){
    NFA *nfa;
    INIT_NFA(nfa);
    free(nfa->start_state);
    free(nfa->accept_state);

    nfa->start_state = n1->start_state;
    nfa->accept_state = n2->accept_state;
    
    n1->accept_state->transition = EPSILON;
    n1->accept_state->outp1 = n2->start_state;

    nfa->edge_num = n1->edge_num + n2->edge_num;
    int i;
    for(i=0;i<n1->edge_num;i++)
        nfa->edges[i] = n1->edges[i];
    for(i=0;i<n2->edge_num;i++)
        nfa->edges[i+n1->edge_num] = n2->edges[i];

    nfa->edges[nfa->edge_num] = new_NFA_edge();
    nfa->edges[nfa->edge_num]->from_state = n1->accept_state->id;
    nfa->edges[nfa->edge_num]->to_state = n2->start_state->id;
    nfa->edges[nfa->edge_num]->character = EPSILON;

    nfa->edge_num += 1;

    return nfa;
}

NFA *
kleene(NFA *n){
    NFA *kle;
    INIT_NFA(kle);

    kle->start_state->id = static_nstate_id++;
    kle->start_state->transition = EPSILON;
    kle->start_state->outp1 = n->start_state;
    kle->start_state->outp2 = kle->accept_state;

    n->accept_state->transition = EPSILON;
    n->accept_state->outp1 = n->start_state;
    n->accept_state->outp2 = kle->accept_state;

    kle->accept_state->id = static_nstate_id++;
    kle->accept_state->transition = NONE;
    kle->accept_state->outp1 = NULL;
    kle->accept_state->outp2 = NULL;

    kle->edge_num = n->edge_num;
    int i;
    for(i=0;i<n->edge_num;i++){
        kle->edges[i] = n->edges[i];
    }

    kle->edges[kle->edge_num] = new_NFA_edge();
    kle->edges[kle->edge_num]->from_state = kle->start_state->id;
    kle->edges[kle->edge_num]->to_state = n->start_state->id;
    kle->edges[kle->edge_num]->character = EPSILON;

    kle->edges[kle->edge_num+1] = new_NFA_edge();
    kle->edges[kle->edge_num+1]->from_state = kle->start_state->id;
    kle->edges[kle->edge_num+1]->to_state = kle->accept_state->id;
    kle->edges[kle->edge_num+1]->character = EPSILON;
    
    kle->edges[kle->edge_num+2] = new_NFA_edge();
    kle->edges[kle->edge_num+2]->from_state = n->accept_state->id;
    kle->edges[kle->edge_num+2]->to_state = n->start_state->id;
    kle->edges[kle->edge_num+2]->character = EPSILON;

    kle->edges[kle->edge_num+3] = new_NFA_edge();
    kle->edges[kle->edge_num+3]->from_state = n->accept_state->id;
    kle->edges[kle->edge_num+3]->to_state = kle->accept_state->id;
    kle->edges[kle->edge_num+3]->character = EPSILON;

    kle->edge_num += 4;
    return kle;
}


Configuration *
epsilon_closure(Configuration *active_states){
    if(active_states==NULL)
        return NULL;

    int state_index;
    int origin_size = active_states->size;
    for(state_index = 0; state_index < origin_size; state_index++){
        NFAstate *worklist[100];
        worklist[0] = active_states->states[state_index];  /*simulate a queue*/
        int worklist_size = 1;
        int worklist_cur_index = 0;
        int worklist_end_index = 1;
        while(worklist_size != 0 ){
            if(worklist[worklist_cur_index]->transition==EPSILON){
                if(worklist[worklist_cur_index]->outp1!=NULL && !in_configuration(worklist[worklist_cur_index]->outp1, active_states)){
                    worklist_size++;
                    worklist[worklist_end_index++] = worklist[worklist_cur_index]->outp1;
                    active_states->states[active_states->size++] = worklist[worklist_cur_index]->outp1;
                }
                if(worklist[worklist_cur_index]->outp2!=NULL && !in_configuration(worklist[worklist_cur_index]->outp2, active_states)){
                    worklist_size++;
                    worklist[worklist_end_index++] = worklist[worklist_cur_index]->outp2;
                    active_states->states[active_states->size++] = worklist[worklist_cur_index]->outp2;
                }
            }
            worklist_size--;
            worklist_cur_index++;
        }
    }

    return active_states;
}

Configuration *
delta(Configuration *q ,int c){
    Configuration *target = new_Configuration();
    target->size = 0;
    //target->id = ID++;

    int index;
    for(index=0; index<q->size; index++){
        if(q->states[index]->transition == c ){
            if(!in_configuration(q->states[index]->outp1,target) && q->states[index]->outp1!=NULL)
                target->states[target->size++] = q->states[index]->outp1;
            if( q->states[index]->outp2!=NULL && !in_configuration(q->states[index]->outp2,target)) 
                target->states[target->size++] = q->states[index]->outp2;
        }
    }

    if(target->size == 0) return NULL;
    //target->id = ID++;
    return target;
}



ConfigurationSet *
subset_construction(NFA *nfa){
    
    ConfigurationSet *Q = new_ConfigurationSet();
    Q->size = 1;
    Configuration *n0 = new_Configuration();
    n0->id = static_dstate_id++;
    n0->size = 1;
    n0->states[0] = nfa->start_state;
    Q->q[0] = epsilon_closure(n0);
    Q->edge_num = 0;

    ConfigurationSet *worklist = new_ConfigurationSet();
    worklist->size = 1;
    worklist->q[0] = Q->q[0];
    
    int cur_index = 0;
    int end_index = 1;
    while(worklist->size != 0){
        worklist->size--;
        int character;
        Configuration *target_conf;
        for(character=1;character<128;character++){
            target_conf = epsilon_closure(delta(worklist->q[cur_index],character));
            if(target_conf ==NULL)
                 Q->T[worklist->q[cur_index]->id][character] = target_conf;
            else{
                if(in_configuration_set(target_conf,Q))
                    cover_configuration(&target_conf,Q);
                else{ 
                    target_conf->id = static_dstate_id++;
                    worklist->q[end_index++] = target_conf; //add target to worklist
                    worklist->size++;
                    Q->q[Q->size++] = target_conf;
                }
                Q->T[worklist->q[cur_index]->id][character] = target_conf;
                Q->edges[Q->edge_num] = new_DFA_edge();
                Q->edges[Q->edge_num]->from_state = worklist->q[cur_index]->id;
                Q->edges[Q->edge_num]->to_state = target_conf->id;
                Q->edges[Q->edge_num]->character = character;
                Q->edge_num++;
            }

        }
        cur_index++;
    }

    return Q;
}

Configuration *
walk_through_DFA(ConfigurationSet *dfa, char *str){
    char c;
    Configuration *dfa_state = dfa->q[0];
    while((c=*str++)!='\0'){
        if(dfa_state == NULL)
            return NULL;
        dfa_state = dfa->T[dfa_state->id][c];
    }
    return dfa_state;
}


bool 
in_configuration(NFAstate *state, Configuration *q){
    int i;
    for(i=0; i<q->size; i++){
        if(state==q->states[i])
            return 1;
    }
    return 0;
}

bool
in_configuration_set(Configuration *q, ConfigurationSet *Q){
    int i;
    for(i=0; i<Q->size; i++){
        if(is_configuration_equal(q,Q->q[i]))
            return 1;
    }
    return 0;
}

bool 
is_accept(Configuration *q){
    if(q==NULL) return 0;
    int i;
    for(i=0;i<q->size;i++){
        if(q->states[i]->transition==NONE)
            return 1;
    }
    return 0;
}

/*================================================================*/
int nstate_adr_cmp(const void *a, const void *b){
    return (*(NFAstate**)a - *(NFAstate**)a);
}

bool 
is_configuration_equal(Configuration *q1, Configuration *q2){
    if(q1==NULL && q2==NULL) return 1;
    if(q1==NULL || q2==NULL) return 0;
    if(q1->size != q2->size) return 0;

    qsort(q1->states, q1->size, sizeof(q1->states[0]), nstate_adr_cmp);
    qsort(q2->states, q2->size, sizeof(q2->states[0]), nstate_adr_cmp);
    
    int i;
    for(i=0;i<q1->size;i++){
        if(q1->states[i]!=q2->states[i])
            return 0;
    }

    return 1;
}
/*=================================================================*/
 void 
 cover_configuration(Configuration **q, ConfigurationSet *Q){
    int i;
    for(i=0;i<Q->size;i++){
        if(is_configuration_equal(*q,Q->q[i])){
            *q = Q->q[i];
            return;
        }
    }
 }