#include "persistent_int_set.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

enum { KEY_RANGE = 256, STEPS = 1200, KEEP = 121 };

static uint64_t state = UINT64_C(0xd1b54a32d192ed03);
static uint64_t rng64(void){uint64_t x=state;x^=x>>12;x^=x<<25;x^=x>>27;state=x;return x*UINT64_C(2685821657736338717);}

static void compare_version(const PSetNode *root, const uint8_t ref[KEY_RANGE]) {
    size_t count=0;
    for(int k=0;k<KEY_RANGE;++k){bool want=ref[k]!=0;assert(pset_contains(root,k)==want);count+=want;}
    assert(pset_size(root)==count);
    assert(pset_validate(root));
}

int main(void){
    PSetArena *a=pset_arena_create(); assert(a);
    const PSetNode *root=NULL;
    const PSetNode *versions[KEEP]; uint8_t snapshots[KEEP][KEY_RANGE];
    memset(snapshots,0,sizeof snapshots); versions[0]=NULL; size_t kept=1;
    uint8_t ref[KEY_RANGE]={0};

    for(size_t step=1;step<=STEPS;++step){
        int key=(int)(rng64()%KEY_RANGE);
        const PSetNode *next=NULL; bool changed=false;
        if(rng64()&1U){
            assert(pset_insert(a,root,key,&next,&changed));
            assert(changed==(ref[key]==0)); ref[key]=1;
        }else{
            assert(pset_erase(a,root,key,&next,&changed));
            assert(changed==(ref[key]!=0)); ref[key]=0;
        }
        root=next; compare_version(root,ref);
        if(step%10U==0 && kept<KEEP){versions[kept]=root;memcpy(snapshots[kept],ref,KEY_RANGE);++kept;}
        if(step%37U==0){size_t old=(size_t)(rng64()%kept);compare_version(versions[old],snapshots[old]);}
    }

    for(size_t i=0;i<kept;++i) compare_version(versions[i],snapshots[i]);

    const PSetNode *v0=NULL,*v1=NULL,*v2=NULL;bool changed=false;
    assert(pset_insert(a,v0,10,&v1,&changed)&&changed);
    assert(pset_insert(a,v1,20,&v2,&changed)&&changed);
    assert(!pset_contains(v0,10));
    assert(pset_contains(v1,10)&&!pset_contains(v1,20));
    assert(pset_contains(v2,10)&&pset_contains(v2,20));

    size_t before=pset_arena_allocated_nodes(a);
    const PSetNode *same=NULL;
    assert(pset_insert(a,v2,20,&same,&changed)&&!changed&&same==v2);
    assert(pset_arena_allocated_nodes(a)==before);

    printf("Persistent set tests passed; arena nodes=%zu blocks=%zu\n",
           pset_arena_allocated_nodes(a),pset_arena_allocated_blocks(a));
    pset_arena_free(a);
    return 0;
}
