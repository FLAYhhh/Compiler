#ifndef NFA_STRUCT
#define NFA_STRUCT

#define bool int
#define EPSILON 128  
#define NONE 129
#define State  NFAstate
#define NFAstate_list DFAstate

typedef struct NFA NFA;
typedef struct NFAstate         NFAstate;
typedef struct NFAstate_list    NFAstate_list;
typedef struct Configuration    Configuration;
typedef struct ConfigurationSet ConfigurationSet;
typedef struct NFA_edge         NFA_edge;
typedef struct DFA_edge         DFA_edge;
typedef ConfigurationSet        DFA;

static static_nstate_id = 0;
static static_dstate_id = 0;

#define new_NFAstate() ((NFAstate*)malloc(sizeof(NFAstate)))
struct NFAstate{
    int id;
    int transition;   //state A  ----x--->  state B, that is stateA has a transition named x 
    NFAstate *outp1;
    NFAstate *outp2;
};

#define new_NFA_edge() ((NFA_edge*)malloc(sizeof(NFA_edge)))
struct NFA_edge{
    int from_state;
    int to_state;
    int character;
};


#define NEW_NFA(x) (x = (NFA*)malloc(sizeof(NFA)))
#define ASSIGN_NFA(x)   x->start_state = (NFAstate*)malloc(sizeof(NFAstate));\
                        x->accept_state = (NFAstate*)malloc(sizeof(NFAstate))
#define INIT_NFA(x)   NEW_NFA(x);ASSIGN_NFA(x)
struct NFA{
    NFAstate *start_state;
    NFAstate *accept_state;
    NFA_edge *edges[100];
    int edge_num;
};


#define new_NFAstate_list  ((NFAstate_list*)malloc(sizeof(NFAstate_list)))
struct NFAstate_list{          //also DFAstate
    NFAstate *content[1000];   //be careful, may run out of memery
    NFAstate_list *transition[128];
    int num_of_nfastate;
    bool is_accept_state;
};


/*Configuration of an NFA: the set of concurrently active states of an NFA */
#define new_Configuration()  ((Configuration*)malloc(sizeof(Configuration)))
struct Configuration{
    int id;
    int size;
    NFAstate *states[100];
};


#define new_DFA_edge() ((DFA_edge*)malloc(sizeof(DFA_edge)))
struct DFA_edge{
    int from_state;
    int to_state;
    int character;
};

#define new_ConfigurationSet() ((ConfigurationSet*)malloc(sizeof(ConfigurationSet)))
struct ConfigurationSet{
    int size;
    Configuration *q[100];
    Configuration *T[100][128];
    DFA_edge *edges[100];
    int edge_num;
};


/*
**Four manipulations of Thompson's construction
*/
NFA *construct_single_char_NFA(char character);
NFA *alternation(NFA *e1 , NFA *e2);
NFA *concatenation(NFA *e1 , NFA *e2);
NFA *kleene(NFA *e);


/*
** funtions used in Subset construction algorithm
*/
bool in_configuration(NFAstate *state,Configuration *q);
bool in_configuration_set(Configuration *q,ConfigurationSet *Q);
int nstate_adr_cmp(const void *a, const void *b);
bool is_configuration_equal(Configuration *q1, Configuration *q2);
Configuration  *epsilon_closure(Configuration *q);
Configuration  *delta(Configuration *q, int c);
ConfigurationSet *subset_construction(NFA *nfa);
bool is_accept(Configuration *q);
Configuration *walk_through_DFA(ConfigurationSet *dfa, char *str);
void cover_configuration(Configuration **q, ConfigurationSet *Q);

#endif