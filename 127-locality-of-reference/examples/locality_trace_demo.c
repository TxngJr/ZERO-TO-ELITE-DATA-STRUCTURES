#include "locality_trace.h"
#include <stdio.h>
int main(void){size_t trace[32];LocalityStats s;if(!loc_generate_sequential(32,trace)||!loc_analyze_indices(trace,32,8,64,&s))return 1;printf("accesses=%zu unique_lines=%zu same_line=%zu transitions=%zu\n",s.accesses,s.unique_lines,s.same_line_adjacent,s.line_transitions);return 0;}
