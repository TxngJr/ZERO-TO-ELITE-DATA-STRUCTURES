#include "locality_trace.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
enum{N=128};
int main(void){
    size_t seq[N];assert(loc_generate_sequential(N,seq));
    LocalityStats s;assert(loc_analyze_indices(seq,N,8,64,&s));
    assert(s.accesses==N&&s.unique_lines==16U&&s.first_line_touches==16U);
    assert(s.temporal_line_reuses==112U&&s.same_line_adjacent==112U);
    assert(s.line_transitions==15U&&s.adjacent_line_transitions==15U&&s.max_index==127U);

    size_t stride_lines[16];
    for(size_t i=0;i<16U;++i)stride_lines[i]=i*8U;
    assert(loc_analyze_indices(stride_lines,16,8,64,&s));
    assert(s.unique_lines==16U&&s.temporal_line_reuses==0U);
    assert(s.same_line_adjacent==0U&&s.line_transitions==15U&&s.adjacent_line_transitions==15U);

    size_t twice[256];
    for(size_t i=0;i<128U;++i){twice[i]=i;twice[128U+i]=i;}
    assert(loc_analyze_indices(twice,256,8,64,&s));
    assert(s.unique_lines==16U&&s.temporal_line_reuses==240U);
    assert(s.same_line_adjacent+s.line_transitions==255U);

    size_t perm[127];assert(loc_generate_strided_permutation(127,31,perm));
    unsigned char seen[127]={0};
    for(size_t i=0;i<127U;++i){assert(perm[i]<127U&&!seen[perm[i]]);seen[perm[i]]=1U;}
    assert(!loc_generate_strided_permutation(128,8,seq));

    size_t bad=SIZE_MAX;assert(!loc_analyze_indices(&bad,1,2,64,&s));
    assert(loc_analyze_indices(NULL,0,8,64,&s)&&s.accesses==0U);
    puts("Locality trace tests passed");
    return 0;
}
