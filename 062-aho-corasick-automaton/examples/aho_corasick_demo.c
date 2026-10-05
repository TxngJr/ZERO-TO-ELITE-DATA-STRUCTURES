#include "byte_aho_corasick.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    const BytePattern patterns[]={
        {(const uint8_t *)"he",2,1},
        {(const uint8_t *)"she",3,2},
        {(const uint8_t *)"his",3,3},
        {(const uint8_t *)"hers",4,4}
    };

    ByteAhoCorasick *ac=byte_aho_corasick_create(
        patterns,4
    );
    assert(ac!=NULL);

    const uint8_t *text=(const uint8_t *)"ushers";
    size_t count=0;

    assert(byte_aho_corasick_count_matches(
        ac,text,strlen((const char *)text),&count
    ));

    ByteACMatch matches[8];
    size_t written=0;

    assert(byte_aho_corasick_report(
        ac,text,strlen((const char *)text),
        matches,8,&written
    ));

    printf("matches=%zu states=%zu\n",
           count,byte_aho_corasick_state_count(ac));

    for(size_t i=0;i<written;++i) {
        printf(
            "id=%" PRIu64 " end=%zu\n",
            matches[i].pattern_id,
            matches[i].end_position
        );
    }

    assert(byte_aho_corasick_validate(ac));
    byte_aho_corasick_free(ac);
    return 0;
}
