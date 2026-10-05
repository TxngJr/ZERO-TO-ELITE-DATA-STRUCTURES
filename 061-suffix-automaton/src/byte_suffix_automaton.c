#include "byte_suffix_automaton.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint8_t symbol;
    size_t target;
} SamTransition;

typedef struct {
    size_t max_len;
    size_t link;
    size_t occurrences;
    SamTransition *transitions;
    size_t transition_count;
    size_t transition_capacity;
} SamState;

struct ByteSuffixAutomaton {
    SamState *states;
    size_t state_count;
    size_t state_capacity;
    size_t last;
    size_t text_length;
};

static bool reserve_states(
    ByteSuffixAutomaton *automaton,
    size_t needed
) {
    if(needed<=automaton->state_capacity)return true;

    size_t capacity=automaton->state_capacity==0
        ?4:automaton->state_capacity;

    while(capacity<needed) {
        if(capacity>SIZE_MAX/2) {
            capacity=needed;
            break;
        }
        capacity*=2;
    }

    if(capacity>SIZE_MAX/sizeof *automaton->states) {
        return false;
    }

    SamState *next=realloc(
        automaton->states,
        capacity*sizeof *next
    );

    if(next==NULL)return false;

    automaton->states=next;
    automaton->state_capacity=capacity;
    return true;
}

static bool reserve_transitions(
    SamState *state,
    size_t needed
) {
    if(needed<=state->transition_capacity)return true;

    size_t capacity=state->transition_capacity==0
        ?4:state->transition_capacity;

    while(capacity<needed) {
        if(capacity>SIZE_MAX/2) {
            capacity=needed;
            break;
        }
        capacity*=2;
    }

    if(capacity>SIZE_MAX/sizeof *state->transitions) {
        return false;
    }

    SamTransition *next=realloc(
        state->transitions,
        capacity*sizeof *next
    );

    if(next==NULL)return false;

    state->transitions=next;
    state->transition_capacity=capacity;
    return true;
}

static size_t find_transition_index(
    const SamState *state,
    uint8_t symbol
) {
    for(size_t i=0;i<state->transition_count;++i) {
        if(state->transitions[i].symbol==symbol)return i;
    }

    return SIZE_MAX;
}

static size_t transition_target(
    const SamState *state,
    uint8_t symbol
) {
    const size_t i=find_transition_index(state,symbol);
    return i==SIZE_MAX?SIZE_MAX:state->transitions[i].target;
}

static bool set_transition(
    SamState *state,
    uint8_t symbol,
    size_t target
) {
    const size_t i=find_transition_index(state,symbol);

    if(i!=SIZE_MAX) {
        state->transitions[i].target=target;
        return true;
    }

    if(state->transition_count==SIZE_MAX||
       !reserve_transitions(
           state,state->transition_count+1
       )) {
        return false;
    }

    state->transitions[state->transition_count++]=
        (SamTransition){
            .symbol=symbol,
            .target=target
        };

    return true;
}

static size_t append_empty_state(
    ByteSuffixAutomaton *automaton
) {
    if(automaton->state_count==SIZE_MAX||
       !reserve_states(
           automaton,automaton->state_count+1
       )) {
        return SIZE_MAX;
    }

    const size_t index=automaton->state_count++;
    automaton->states[index]=(SamState){
        .link=SIZE_MAX
    };
    return index;
}

static bool clone_state_into(
    ByteSuffixAutomaton *automaton,
    size_t source,
    size_t max_len,
    size_t *out_clone
) {
    const size_t clone=append_empty_state(automaton);

    if(clone==SIZE_MAX)return false;

    const SamState src=automaton->states[source];
    SamState *dst=&automaton->states[clone];

    dst->max_len=max_len;
    dst->link=src.link;
    dst->occurrences=0;

    if(src.transition_count>0) {
        if(!reserve_transitions(dst,src.transition_count)) {
            free(dst->transitions);
            automaton->state_count=clone;
            return false;
        }

        for(size_t i=0;i<src.transition_count;++i) {
            dst->transitions[i]=src.transitions[i];
        }

        dst->transition_count=src.transition_count;
    }

    *out_clone=clone;
    return true;
}

static bool extend_one(
    ByteSuffixAutomaton *automaton,
    uint8_t symbol
) {
    const size_t previous_last=automaton->last;
    const size_t cur=append_empty_state(automaton);

    if(cur==SIZE_MAX)return false;

    automaton->states[cur].max_len=
        automaton->states[previous_last].max_len+1;
    automaton->states[cur].occurrences=1;

    size_t p=previous_last;

    while(p!=SIZE_MAX&&
          transition_target(
              &automaton->states[p],symbol
          )==SIZE_MAX) {
        if(!set_transition(
                &automaton->states[p],symbol,cur
            )) {
            return false;
        }

        p=automaton->states[p].link;
    }

    if(p==SIZE_MAX) {
        automaton->states[cur].link=0;
        automaton->last=cur;
        return true;
    }

    const size_t q=transition_target(
        &automaton->states[p],symbol
    );

    if(automaton->states[p].max_len+1==
       automaton->states[q].max_len) {
        automaton->states[cur].link=q;
        automaton->last=cur;
        return true;
    }

    size_t clone=SIZE_MAX;

    if(!clone_state_into(
            automaton,q,
            automaton->states[p].max_len+1,
            &clone
        )) {
        return false;
    }

    while(p!=SIZE_MAX) {
        const size_t i=find_transition_index(
            &automaton->states[p],symbol
        );

        if(i==SIZE_MAX||
           automaton->states[p].transitions[i].target!=q) {
            break;
        }

        automaton->states[p].transitions[i].target=clone;
        p=automaton->states[p].link;
    }

    automaton->states[q].link=clone;
    automaton->states[cur].link=clone;
    automaton->last=cur;
    return true;
}

static bool propagate_occurrences(
    ByteSuffixAutomaton *automaton
) {
    const size_t n=automaton->text_length;
    const size_t states=automaton->state_count;

    if(n==SIZE_MAX||states==0)return false;

    size_t *count=calloc(n+1,sizeof *count);
    size_t *order=malloc(states*sizeof *order);

    if(count==NULL||order==NULL) {
        free(count);
        free(order);
        return false;
    }

    for(size_t i=0;i<states;++i) {
        if(automaton->states[i].max_len>n) {
            free(count);
            free(order);
            return false;
        }
        ++count[automaton->states[i].max_len];
    }

    for(size_t i=1;i<=n;++i)count[i]+=count[i-1];

    for(size_t i=states;i>0;--i) {
        const size_t state=i-1;
        const size_t len=automaton->states[state].max_len;
        order[--count[len]]=state;
    }

    for(size_t i=states;i>1;--i) {
        const size_t state=order[i-1];
        const size_t link=automaton->states[state].link;

        if(link==SIZE_MAX||
           SIZE_MAX-automaton->states[link].occurrences<
               automaton->states[state].occurrences) {
            free(count);
            free(order);
            return false;
        }

        automaton->states[link].occurrences+=
            automaton->states[state].occurrences;
    }

    free(count);
    free(order);
    return true;
}

ByteSuffixAutomaton *byte_suffix_automaton_create(
    const uint8_t *text,
    size_t length
) {
    if(length>0&&text==NULL)return NULL;

    ByteSuffixAutomaton *automaton=
        calloc(1,sizeof *automaton);

    if(automaton==NULL)return NULL;

    automaton->text_length=length;

    const size_t root=append_empty_state(automaton);

    if(root!=0) {
        byte_suffix_automaton_free(automaton);
        return NULL;
    }

    automaton->states[0].max_len=0;
    automaton->states[0].link=SIZE_MAX;
    automaton->states[0].occurrences=0;
    automaton->last=0;

    for(size_t i=0;i<length;++i) {
        if(!extend_one(automaton,text[i])) {
            byte_suffix_automaton_free(automaton);
            return NULL;
        }
    }

    if(!propagate_occurrences(automaton)) {
        byte_suffix_automaton_free(automaton);
        return NULL;
    }

    return automaton;
}

void byte_suffix_automaton_free(
    ByteSuffixAutomaton *automaton
) {
    if(automaton==NULL)return;

    for(size_t i=0;i<automaton->state_count;++i) {
        free(automaton->states[i].transitions);
    }

    free(automaton->states);
    free(automaton);
}

size_t byte_suffix_automaton_text_length(
    const ByteSuffixAutomaton *automaton
) {
    return automaton==NULL?0:automaton->text_length;
}

size_t byte_suffix_automaton_state_count(
    const ByteSuffixAutomaton *automaton
) {
    return automaton==NULL?0:automaton->state_count;
}

static bool walk(
    const ByteSuffixAutomaton *automaton,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *out_state
) {
    if(automaton==NULL||pattern==NULL||
       pattern_length==0||out_state==NULL) {
        return false;
    }

    size_t state=0;

    for(size_t i=0;i<pattern_length;++i) {
        state=transition_target(
            &automaton->states[state],pattern[i]
        );

        if(state==SIZE_MAX)return false;
    }

    *out_state=state;
    return true;
}

bool byte_suffix_automaton_contains(
    const ByteSuffixAutomaton *automaton,
    const uint8_t *pattern,
    size_t pattern_length
) {
    size_t state=0;
    return walk(
        automaton,pattern,pattern_length,&state
    );
}

bool byte_suffix_automaton_count(
    const ByteSuffixAutomaton *automaton,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *out_count
) {
    if(out_count==NULL)return false;

    size_t state=0;

    if(!walk(
            automaton,pattern,pattern_length,&state
        )) {
        *out_count=0;
        return true;
    }

    *out_count=automaton->states[state].occurrences;
    return true;
}

bool byte_suffix_automaton_distinct_substrings(
    const ByteSuffixAutomaton *automaton,
    uint64_t *out_count
) {
    if(automaton==NULL||out_count==NULL)return false;

    uint64_t total=0;

    for(size_t i=1;i<automaton->state_count;++i) {
        const size_t link=automaton->states[i].link;

        if(link==SIZE_MAX)return false;

        const size_t contribution=
            automaton->states[i].max_len-
            automaton->states[link].max_len;

        if(UINT64_MAX-total<(uint64_t)contribution) {
            return false;
        }

        total+=(uint64_t)contribution;
    }

    *out_count=total;
    return true;
}

bool byte_suffix_automaton_longest_repeated(
    const ByteSuffixAutomaton *automaton,
    size_t *out_length
) {
    if(automaton==NULL||out_length==NULL)return false;

    size_t best=0;

    for(size_t i=1;i<automaton->state_count;++i) {
        if(automaton->states[i].occurrences>=2&&
           automaton->states[i].max_len>best) {
            best=automaton->states[i].max_len;
        }
    }

    *out_length=best;
    return true;
}

bool byte_suffix_automaton_validate(
    const ByteSuffixAutomaton *automaton
) {
    if(automaton==NULL||
       automaton->states==NULL||
       automaton->state_count==0||
       automaton->last>=automaton->state_count) {
        return false;
    }

    if(automaton->states[0].max_len!=0||
       automaton->states[0].link!=SIZE_MAX) {
        return false;
    }

    if(automaton->text_length==0) {
        return automaton->state_count==1&&
               automaton->last==0;
    }

    if(automaton->text_length>SIZE_MAX/2+1) {
        return false;
    }

    const size_t bound=
        1+2*(automaton->text_length-1);

    if(automaton->state_count>bound)return false;

    for(size_t i=0;i<automaton->state_count;++i) {
        const SamState *state=&automaton->states[i];

        if(i!=0) {
            if(state->link>=automaton->state_count||
               automaton->states[state->link].max_len>=
                   state->max_len||
               state->occurrences==0) {
                return false;
            }
        }

        bool seen[256]={false};

        for(size_t t=0;t<state->transition_count;++t) {
            const SamTransition tr=state->transitions[t];

            if(seen[tr.symbol]||
               tr.target>=automaton->state_count||
               automaton->states[tr.target].max_len<=
                   state->max_len) {
                return false;
            }

            seen[tr.symbol]=true;
        }
    }

    return true;
}
