#include "byte_aho_corasick.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint8_t symbol;
    size_t target;
} ACTransition;

typedef struct {
    uint64_t pattern_id;
    size_t pattern_length;
} ACOutput;

typedef struct {
    ACTransition *transitions;
    size_t transition_count;
    size_t transition_capacity;

    ACOutput *outputs;
    size_t output_count;
    size_t output_capacity;

    size_t fail;
    size_t output_link;
    size_t depth;
} ACState;

struct ByteAhoCorasick {
    ACState *states;
    size_t state_count;
    size_t state_capacity;
    size_t pattern_count;
};

static bool reserve_states(
    ByteAhoCorasick *automaton,
    size_t needed
) {
    if(needed<=automaton->state_capacity)return true;

    size_t capacity=automaton->state_capacity==0
        ?8:automaton->state_capacity;

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

    ACState *next=realloc(
        automaton->states,
        capacity*sizeof *next
    );

    if(next==NULL)return false;

    automaton->states=next;
    automaton->state_capacity=capacity;
    return true;
}

static bool reserve_transitions(
    ACState *state,
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

    ACTransition *next=realloc(
        state->transitions,
        capacity*sizeof *next
    );

    if(next==NULL)return false;

    state->transitions=next;
    state->transition_capacity=capacity;
    return true;
}

static bool reserve_outputs(
    ACState *state,
    size_t needed
) {
    if(needed<=state->output_capacity)return true;

    size_t capacity=state->output_capacity==0
        ?2:state->output_capacity;

    while(capacity<needed) {
        if(capacity>SIZE_MAX/2) {
            capacity=needed;
            break;
        }
        capacity*=2;
    }

    if(capacity>SIZE_MAX/sizeof *state->outputs) {
        return false;
    }

    ACOutput *next=realloc(
        state->outputs,
        capacity*sizeof *next
    );

    if(next==NULL)return false;

    state->outputs=next;
    state->output_capacity=capacity;
    return true;
}

static size_t append_state(
    ByteAhoCorasick *automaton,
    size_t depth
) {
    if(automaton->state_count==SIZE_MAX||
       !reserve_states(
           automaton,automaton->state_count+1
       )) {
        return SIZE_MAX;
    }

    const size_t index=automaton->state_count++;

    automaton->states[index]=(ACState){
        .fail=0,
        .output_link=SIZE_MAX,
        .depth=depth
    };

    return index;
}

static size_t transition_target(
    const ACState *state,
    uint8_t symbol
) {
    for(size_t i=0;i<state->transition_count;++i) {
        if(state->transitions[i].symbol==symbol) {
            return state->transitions[i].target;
        }
    }

    return SIZE_MAX;
}

static bool add_transition(
    ACState *state,
    uint8_t symbol,
    size_t target
) {
    if(transition_target(state,symbol)!=SIZE_MAX)return false;

    if(state->transition_count==SIZE_MAX||
       !reserve_transitions(
           state,state->transition_count+1
       )) {
        return false;
    }

    state->transitions[state->transition_count++]=
        (ACTransition){
            .symbol=symbol,
            .target=target
        };

    return true;
}

static bool add_output(
    ACState *state,
    uint64_t id,
    size_t length
) {
    if(state->output_count==SIZE_MAX||
       !reserve_outputs(
           state,state->output_count+1
       )) {
        return false;
    }

    state->outputs[state->output_count++]=
        (ACOutput){
            .pattern_id=id,
            .pattern_length=length
        };

    return true;
}

static bool insert_pattern(
    ByteAhoCorasick *automaton,
    const BytePattern *pattern
) {
    size_t state=0;

    for(size_t i=0;i<pattern->length;++i) {
        size_t next=transition_target(
            &automaton->states[state],
            pattern->bytes[i]
        );

        if(next==SIZE_MAX) {
            next=append_state(
                automaton,
                automaton->states[state].depth+1
            );

            if(next==SIZE_MAX)return false;

            if(!add_transition(
                    &automaton->states[state],
                    pattern->bytes[i],
                    next
                )) {
                return false;
            }
        }

        state=next;
    }

    return add_output(
        &automaton->states[state],
        pattern->id,
        pattern->length
    );
}

static bool build_failure_links(
    ByteAhoCorasick *automaton
) {
    if(automaton->state_count==0)return false;

    if(automaton->state_count>
       SIZE_MAX/sizeof(size_t)) {
        return false;
    }

    size_t *queue=malloc(
        automaton->state_count*sizeof *queue
    );

    if(queue==NULL)return false;

    size_t head=0;
    size_t tail=0;

    ACState *root=&automaton->states[0];
    root->fail=0;
    root->output_link=SIZE_MAX;

    for(size_t i=0;i<root->transition_count;++i) {
        const size_t child=root->transitions[i].target;

        automaton->states[child].fail=0;
        automaton->states[child].output_link=
            root->output_count>0?0:SIZE_MAX;

        queue[tail++]=child;
    }

    while(head<tail) {
        const size_t state_index=queue[head++];
        ACState *state=&automaton->states[state_index];

        for(size_t i=0;i<state->transition_count;++i) {
            const uint8_t symbol=state->transitions[i].symbol;
            const size_t child=state->transitions[i].target;

            size_t fallback=state->fail;
            size_t target=transition_target(
                &automaton->states[fallback],
                symbol
            );

            while(fallback!=0&&target==SIZE_MAX) {
                fallback=automaton->states[fallback].fail;
                target=transition_target(
                    &automaton->states[fallback],
                    symbol
                );
            }

            if(target==SIZE_MAX||target==child) {
                target=0;
            }

            automaton->states[child].fail=target;

            if(automaton->states[target].output_count>0) {
                automaton->states[child].output_link=target;
            } else {
                automaton->states[child].output_link=
                    automaton->states[target].output_link;
            }

            queue[tail++]=child;
        }
    }

    free(queue);
    return tail==automaton->state_count-1;
}

ByteAhoCorasick *byte_aho_corasick_create(
    const BytePattern *patterns,
    size_t pattern_count
) {
    if(pattern_count>0&&patterns==NULL)return NULL;

    ByteAhoCorasick *automaton=
        calloc(1,sizeof *automaton);

    if(automaton==NULL)return NULL;

    const size_t root=append_state(automaton,0);

    if(root!=0) {
        byte_aho_corasick_free(automaton);
        return NULL;
    }

    for(size_t i=0;i<pattern_count;++i) {
        if(patterns[i].length==0||
           patterns[i].bytes==NULL) {
            byte_aho_corasick_free(automaton);
            return NULL;
        }

        if(!insert_pattern(automaton,&patterns[i])) {
            byte_aho_corasick_free(automaton);
            return NULL;
        }
    }

    automaton->pattern_count=pattern_count;

    if(!build_failure_links(automaton)) {
        byte_aho_corasick_free(automaton);
        return NULL;
    }

    return automaton;
}

void byte_aho_corasick_free(
    ByteAhoCorasick *automaton
) {
    if(automaton==NULL)return;

    for(size_t i=0;i<automaton->state_count;++i) {
        free(automaton->states[i].transitions);
        free(automaton->states[i].outputs);
    }

    free(automaton->states);
    free(automaton);
}

size_t byte_aho_corasick_state_count(
    const ByteAhoCorasick *automaton
) {
    return automaton==NULL?0:automaton->state_count;
}

size_t byte_aho_corasick_pattern_count(
    const ByteAhoCorasick *automaton
) {
    return automaton==NULL?0:automaton->pattern_count;
}

static size_t step_state(
    const ByteAhoCorasick *automaton,
    size_t state,
    uint8_t symbol
) {
    size_t target=transition_target(
        &automaton->states[state],symbol
    );

    while(state!=0&&target==SIZE_MAX) {
        state=automaton->states[state].fail;
        target=transition_target(
            &automaton->states[state],symbol
        );
    }

    return target==SIZE_MAX?0:target;
}

static bool emit_state_outputs(
    const ByteAhoCorasick *automaton,
    size_t state,
    size_t end_position,
    ByteACMatch *output,
    size_t capacity,
    size_t *written,
    bool report
) {
    size_t current=state;

    for(;;) {
        const ACState *s=&automaton->states[current];

        for(size_t i=0;i<s->output_count;++i) {
            if(report) {
                if(*written>=capacity)return false;

                output[*written]=(ByteACMatch){
                    .pattern_id=s->outputs[i].pattern_id,
                    .end_position=end_position
                };
            }

            ++*written;
        }

        if(s->output_link==SIZE_MAX)break;
        current=s->output_link;
    }

    return true;
}

static bool scan(
    const ByteAhoCorasick *automaton,
    const uint8_t *text,
    size_t text_length,
    ByteACMatch *output,
    size_t capacity,
    size_t *out_count,
    bool report
) {
    if(automaton==NULL||out_count==NULL||
       (text_length>0&&text==NULL)) {
        return false;
    }

    size_t state=0;
    size_t written=0;

    for(size_t i=0;i<text_length;++i) {
        state=step_state(automaton,state,text[i]);

        if(!emit_state_outputs(
                automaton,state,i+1,
                output,capacity,&written,report
            )) {
            return false;
        }
    }

    *out_count=written;
    return true;
}

bool byte_aho_corasick_count_matches(
    const ByteAhoCorasick *automaton,
    const uint8_t *text,
    size_t text_length,
    size_t *out_count
) {
    return scan(
        automaton,text,text_length,
        NULL,0,out_count,false
    );
}

bool byte_aho_corasick_report(
    const ByteAhoCorasick *automaton,
    const uint8_t *text,
    size_t text_length,
    ByteACMatch *output,
    size_t capacity,
    size_t *out_written
) {
    if(out_written==NULL)return false;

    size_t needed=0;

    if(!byte_aho_corasick_count_matches(
            automaton,text,text_length,&needed
        )) {
        return false;
    }

    if(needed>capacity||(needed>0&&output==NULL)) {
        return false;
    }

    return scan(
        automaton,text,text_length,
        output,capacity,out_written,true
    );
}

bool byte_aho_corasick_validate(
    const ByteAhoCorasick *automaton
) {
    if(automaton==NULL||
       automaton->states==NULL||
       automaton->state_count==0) {
        return false;
    }

    if(automaton->states[0].depth!=0||
       automaton->states[0].fail!=0||
       automaton->states[0].output_link!=SIZE_MAX) {
        return false;
    }

    size_t output_records=0;

    for(size_t i=0;i<automaton->state_count;++i) {
        const ACState *state=&automaton->states[i];
        bool symbols[256]={false};

        if(i!=0) {
            if(state->fail>=automaton->state_count||
               automaton->states[state->fail].depth>=
                   state->depth) {
                return false;
            }

            if(state->output_link!=SIZE_MAX) {
                if(state->output_link>=automaton->state_count||
                   automaton->states[state->output_link].output_count==0||
                   automaton->states[state->output_link].depth>=
                       state->depth) {
                    return false;
                }
            }
        }

        for(size_t t=0;t<state->transition_count;++t) {
            const ACTransition tr=state->transitions[t];

            if(symbols[tr.symbol]||
               tr.target>=automaton->state_count||
               automaton->states[tr.target].depth!=
                   state->depth+1) {
                return false;
            }

            symbols[tr.symbol]=true;
        }

        if(SIZE_MAX-output_records<state->output_count) {
            return false;
        }

        output_records+=state->output_count;
    }

    return output_records==automaton->pattern_count;
}
