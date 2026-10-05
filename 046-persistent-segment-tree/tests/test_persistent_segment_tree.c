#include "persistent_segment_tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static void naive_query(
    const int64_t *snapshot,
    size_t left,
    size_t right,
    int64_t *sum,
    int64_t *min
) {
    *sum=0;
    *min=snapshot[left];

    for(size_t i=left;i<right;++i) {
        *sum+=snapshot[i];
        if(snapshot[i]<*min)*min=snapshot[i];
    }
}

static void test_branching_versions(void) {
    const int64_t initial[]={1,2,3,4};

    PersistentSegmentTree *tree=
        persistent_segment_tree_create(initial,4);
    assert(tree!=NULL);

    size_t v1=0,v2=0,v3=0;

    assert(persistent_segment_tree_point_set(
        tree,0,1,20,&v1
    ));
    assert(persistent_segment_tree_point_set(
        tree,0,3,40,&v2
    ));
    assert(persistent_segment_tree_point_set(
        tree,v1,0,10,&v3
    ));

    const int64_t expected[][4]={
        {1,2,3,4},
        {1,20,3,4},
        {1,2,3,40},
        {10,20,3,4}
    };

    for(size_t version=0;version<4;++version) {
        for(size_t i=0;i<4;++i) {
            int64_t actual=0;
            assert(persistent_segment_tree_point_get(
                tree,version,i,&actual
            ));
            assert(actual==expected[version][i]);
        }
    }

    assert(persistent_segment_tree_version_count(tree)==4);
    assert(persistent_segment_tree_validate(tree));

    persistent_segment_tree_free(tree);
}

static void test_randomized(void) {
    enum { N=96, STEPS=6000 };

    int64_t initial[N];

    for(size_t i=0;i<N;++i) {
        initial[i]=(int64_t)(i%17)-8;
    }

    PersistentSegmentTree *tree=
        persistent_segment_tree_create(initial,N);
    assert(tree!=NULL);

    const size_t max_versions=STEPS+1;

    if(max_versions>SIZE_MAX/N ||
       max_versions*N>SIZE_MAX/sizeof(int64_t)) {
        assert(0);
    }

    int64_t *snapshots=malloc(
        max_versions*N*sizeof *snapshots
    );
    assert(snapshots!=NULL);

    memcpy(snapshots,initial,N*sizeof *initial);

    size_t versions=1;
    uint32_t rng=0x46A12345u;

    for(int step=0;step<STEPS;++step) {
        const unsigned op=next_rng(&rng)%3U;

        if(op!=2U) {
            const size_t base=next_rng(&rng)%versions;
            const size_t index=next_rng(&rng)%N;
            const int64_t value=
                (int64_t)(next_rng(&rng)%2001U)-1000;

            size_t new_version=0;

            assert(persistent_segment_tree_point_set(
                tree,
                base,
                index,
                value,
                &new_version
            ));

            assert(new_version==versions);

            memcpy(
                &snapshots[versions*N],
                &snapshots[base*N],
                N*sizeof *snapshots
            );

            snapshots[versions*N+index]=value;
            ++versions;
        } else {
            const size_t version=next_rng(&rng)%versions;

            size_t left=next_rng(&rng)%N;
            size_t right=next_rng(&rng)%N;

            if(left>right) {
                const size_t tmp=left;
                left=right;
                right=tmp;
            }

            ++right;

            int64_t expected_sum=0;
            int64_t expected_min=0;

            naive_query(
                &snapshots[version*N],
                left,
                right,
                &expected_sum,
                &expected_min
            );

            int64_t actual=0;

            assert(persistent_segment_tree_range_sum(
                tree,version,left,right,&actual
            ));
            assert(actual==expected_sum);

            assert(persistent_segment_tree_range_min(
                tree,version,left,right,&actual
            ));
            assert(actual==expected_min);
        }

        if((step%997)==0) {
            assert(
                persistent_segment_tree_version_count(tree)
                ==versions
            );

            const size_t version=next_rng(&rng)%versions;
            const size_t index=next_rng(&rng)%N;

            int64_t actual=0;
            assert(persistent_segment_tree_point_get(
                tree,version,index,&actual
            ));
            assert(actual==snapshots[version*N+index]);
        }
    }

    assert(persistent_segment_tree_validate(tree));

    /* Re-check random historical snapshots after every update has finished. */
    for(int check=0;check<1000;++check) {
        const size_t version=next_rng(&rng)%versions;
        const size_t index=next_rng(&rng)%N;

        int64_t actual=0;

        assert(persistent_segment_tree_point_get(
            tree,version,index,&actual
        ));
        assert(actual==snapshots[version*N+index]);
    }

    free(snapshots);
    persistent_segment_tree_free(tree);
}

int main(void) {
    PersistentSegmentTree *empty=
        persistent_segment_tree_create(NULL,0);

    assert(empty!=NULL);
    assert(persistent_segment_tree_version_count(empty)==1);
    assert(persistent_segment_tree_validate(empty));
    persistent_segment_tree_free(empty);

    test_branching_versions();
    test_randomized();

    puts("Persistent Segment Tree tests passed");
    return 0;
}
