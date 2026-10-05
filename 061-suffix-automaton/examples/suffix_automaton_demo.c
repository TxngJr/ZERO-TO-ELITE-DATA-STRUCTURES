#include "byte_suffix_automaton.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

int main(void) {
    const uint8_t text[]={'a','b','a','b','a'};

    ByteSuffixAutomaton *sam=
        byte_suffix_automaton_create(text,sizeof text);

    assert(sam!=NULL);

    const uint8_t pattern[]={'a','b','a'};
    size_t count=0;
    size_t repeated=0;
    uint64_t distinct=0;

    assert(byte_suffix_automaton_count(
        sam,pattern,sizeof pattern,&count
    ));
    assert(byte_suffix_automaton_distinct_substrings(
        sam,&distinct
    ));
    assert(byte_suffix_automaton_longest_repeated(
        sam,&repeated
    ));

    printf(
        "aba=%zu distinct=%" PRIu64 " longest_repeated=%zu states=%zu\n",
        count,distinct,repeated,
        byte_suffix_automaton_state_count(sam)
    );

    assert(byte_suffix_automaton_validate(sam));
    byte_suffix_automaton_free(sam);
    return 0;
}
