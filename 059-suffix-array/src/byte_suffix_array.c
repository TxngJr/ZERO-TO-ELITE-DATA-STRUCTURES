#include "byte_suffix_array.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t index;
    size_t first;
    size_t second;
} RankPair;

struct ByteSuffixArray {
    uint8_t *text;
    size_t length;
    size_t *sa;
    size_t *rank;
    size_t *lcp;
};

static int pair_compare(const void *a,const void *b) {
    const RankPair *pa=a;
    const RankPair *pb=b;

    if(pa->first<pb->first)return -1;
    if(pa->first>pb->first)return 1;
    if(pa->second<pb->second)return -1;
    if(pa->second>pb->second)return 1;
    if(pa->index<pb->index)return -1;
    if(pa->index>pb->index)return 1;
    return 0;
}

static bool alloc_size_array(size_t n,size_t **out) {
    if(n>SIZE_MAX/sizeof **out)return false;
    *out=n==0?NULL:malloc(n*sizeof **out);
    return n==0||*out!=NULL;
}

static bool build_sa(ByteSuffixArray *array) {
    const size_t n=array->length;
    if(n==0)return true;
    if(n==1) {
        array->sa[0]=0;
        return true;
    }

    size_t *classes=NULL;
    size_t *next_classes=NULL;
    RankPair *pairs=NULL;

    if(!alloc_size_array(n,&classes)||
       !alloc_size_array(n,&next_classes)||
       n>SIZE_MAX/sizeof *pairs) {
        free(classes);
        free(next_classes);
        return false;
    }

    pairs=malloc(n*sizeof *pairs);

    if(pairs==NULL) {
        free(classes);
        free(next_classes);
        return false;
    }

    for(size_t i=0;i<n;++i) {
        classes[i]=(size_t)array->text[i];
    }

    size_t span=1;

    while(span<n) {
        for(size_t i=0;i<n;++i) {
            pairs[i]=(RankPair){
                .index=i,
                .first=classes[i]+1,
                .second=i<=n-1-span
                    ?classes[i+span]+1
                    :0
            };
        }

        qsort(pairs,n,sizeof *pairs,pair_compare);

        array->sa[0]=pairs[0].index;
        size_t class_id=0;
        next_classes[pairs[0].index]=class_id;

        for(size_t r=1;r<n;++r) {
            if(pairs[r].first!=pairs[r-1].first||
               pairs[r].second!=pairs[r-1].second) {
                ++class_id;
            }

            array->sa[r]=pairs[r].index;
            next_classes[pairs[r].index]=class_id;
        }

        size_t *tmp=classes;
        classes=next_classes;
        next_classes=tmp;

        if(class_id+1==n)break;

        if(span>n/2)span=n;
        else span*=2;
    }

    free(pairs);
    free(next_classes);
    free(classes);
    return true;
}

static bool build_lcp(ByteSuffixArray *array) {
    const size_t n=array->length;
    if(n==0)return true;

    for(size_t r=0;r<n;++r) {
        array->rank[array->sa[r]]=r;
    }

    size_t h=0;

    for(size_t i=0;i<n;++i) {
        const size_t r=array->rank[i];

        if(r==0) {
            array->lcp[0]=0;
            continue;
        }

        const size_t j=array->sa[r-1];

        while(i+h<n&&j+h<n&&
              array->text[i+h]==array->text[j+h]) {
            ++h;
        }

        array->lcp[r]=h;

        if(h>0)--h;
    }

    return true;
}

ByteSuffixArray *byte_suffix_array_create(
    const uint8_t *text,
    size_t length
) {
    if(length>0&&text==NULL)return NULL;

    ByteSuffixArray *array=calloc(1,sizeof *array);
    if(array==NULL)return NULL;

    array->length=length;

    if(length==0)return array;

    array->text=malloc(length);

    if(array->text==NULL||
       !alloc_size_array(length,&array->sa)||
       !alloc_size_array(length,&array->rank)||
       !alloc_size_array(length,&array->lcp)) {
        byte_suffix_array_free(array);
        return NULL;
    }

    memcpy(array->text,text,length);

    if(!build_sa(array)||!build_lcp(array)) {
        byte_suffix_array_free(array);
        return NULL;
    }

    return array;
}

void byte_suffix_array_free(ByteSuffixArray *array) {
    if(array==NULL)return;
    free(array->text);
    free(array->sa);
    free(array->rank);
    free(array->lcp);
    free(array);
}

size_t byte_suffix_array_size(const ByteSuffixArray *array) {
    return array==NULL?0:array->length;
}

bool byte_suffix_array_at(
    const ByteSuffixArray *array,
    size_t rank,
    size_t *out_suffix_start
) {
    if(array==NULL||out_suffix_start==NULL||
       rank>=array->length) {
        return false;
    }

    *out_suffix_start=array->sa[rank];
    return true;
}

bool byte_suffix_array_rank_of(
    const ByteSuffixArray *array,
    size_t suffix_start,
    size_t *out_rank
) {
    if(array==NULL||out_rank==NULL||
       suffix_start>=array->length) {
        return false;
    }

    *out_rank=array->rank[suffix_start];
    return true;
}

bool byte_suffix_array_lcp_at(
    const ByteSuffixArray *array,
    size_t rank,
    size_t *out_lcp
) {
    if(array==NULL||out_lcp==NULL||
       rank>=array->length) {
        return false;
    }

    *out_lcp=array->lcp[rank];
    return true;
}

static int suffix_pattern_compare(
    const ByteSuffixArray *array,
    size_t suffix_start,
    const uint8_t *pattern,
    size_t pattern_length
) {
    size_t i=0;

    while(i<pattern_length&&suffix_start+i<array->length) {
        const uint8_t a=array->text[suffix_start+i];
        const uint8_t b=pattern[i];

        if(a<b)return -1;
        if(a>b)return 1;
        ++i;
    }

    if(i==pattern_length)return 0;
    return -1;
}

static bool match_range(
    const ByteSuffixArray *array,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *out_first,
    size_t *out_last
) {
    if(array==NULL||pattern==NULL||
       pattern_length==0||
       out_first==NULL||out_last==NULL) {
        return false;
    }

    size_t low=0;
    size_t high=array->length;

    while(low<high) {
        const size_t mid=low+(high-low)/2;
        const int cmp=suffix_pattern_compare(
            array,array->sa[mid],
            pattern,pattern_length
        );

        if(cmp<0)low=mid+1;
        else high=mid;
    }

    const size_t first=low;
    high=array->length;

    while(low<high) {
        const size_t mid=low+(high-low)/2;
        const int cmp=suffix_pattern_compare(
            array,array->sa[mid],
            pattern,pattern_length
        );

        if(cmp<=0)low=mid+1;
        else high=mid;
    }

    *out_first=first;
    *out_last=low;
    return true;
}

bool byte_suffix_array_contains(
    const ByteSuffixArray *array,
    const uint8_t *pattern,
    size_t pattern_length
) {
    size_t first=0,last=0;

    return match_range(
        array,pattern,pattern_length,&first,&last
    )&&first<last;
}

bool byte_suffix_array_count(
    const ByteSuffixArray *array,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *out_count
) {
    if(out_count==NULL)return false;

    size_t first=0,last=0;

    if(!match_range(
            array,pattern,pattern_length,
            &first,&last
        )) {
        return false;
    }

    *out_count=last-first;
    return true;
}

bool byte_suffix_array_report(
    const ByteSuffixArray *array,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *positions,
    size_t capacity,
    size_t *out_written
) {
    if(out_written==NULL)return false;

    size_t first=0,last=0;

    if(!match_range(
            array,pattern,pattern_length,
            &first,&last
        )) {
        return false;
    }

    const size_t count=last-first;

    if(count>capacity||(count>0&&positions==NULL)) {
        return false;
    }

    for(size_t i=0;i<count;++i) {
        positions[i]=array->sa[first+i];
    }

    *out_written=count;
    return true;
}

static int suffix_compare(
    const ByteSuffixArray *array,
    size_t a,
    size_t b
) {
    while(a<array->length&&b<array->length) {
        if(array->text[a]<array->text[b])return -1;
        if(array->text[a]>array->text[b])return 1;
        ++a;
        ++b;
    }

    if(a==array->length&&b==array->length)return 0;
    return a==array->length?-1:1;
}

static size_t naive_lcp(
    const ByteSuffixArray *array,
    size_t a,
    size_t b
) {
    size_t h=0;

    while(a+h<array->length&&b+h<array->length&&
          array->text[a+h]==array->text[b+h]) {
        ++h;
    }

    return h;
}

bool byte_suffix_array_validate(const ByteSuffixArray *array) {
    if(array==NULL)return false;

    if(array->length==0) {
        return array->text==NULL&&array->sa==NULL&&
               array->rank==NULL&&array->lcp==NULL;
    }

    if(array->text==NULL||array->sa==NULL||
       array->rank==NULL||array->lcp==NULL) {
        return false;
    }

    bool *seen=calloc(array->length,sizeof *seen);

    if(seen==NULL)return false;

    for(size_t r=0;r<array->length;++r) {
        const size_t s=array->sa[r];

        if(s>=array->length||seen[s]||
           array->rank[s]!=r) {
            free(seen);
            return false;
        }

        seen[s]=true;

        if(r==0) {
            if(array->lcp[r]!=0) {
                free(seen);
                return false;
            }
        } else {
            if(suffix_compare(
                    array,array->sa[r-1],s
                )>=0||
               array->lcp[r]!=naive_lcp(
                    array,array->sa[r-1],s
                )) {
                free(seen);
                return false;
            }
        }
    }

    free(seen);
    return true;
}
