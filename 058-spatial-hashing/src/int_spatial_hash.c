#include "int_spatial_hash.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    int64_t cell_x;
    int64_t cell_y;
    size_t next;
    IntSpatialPoint *points;
    size_t point_count;
    size_t point_capacity;
} SpatialCell;

struct IntSpatialHash {
    int64_t cell_size;
    size_t *bucket_heads;
    size_t bucket_capacity;
    SpatialCell *cells;
    size_t cell_count;
    size_t cell_capacity;
    size_t size;
};

static uint64_t mix64(uint64_t x) {
    x^=x>>30;
    x*=UINT64_C(0xbf58476d1ce4e5b9);
    x^=x>>27;
    x*=UINT64_C(0x94d049bb133111eb);
    x^=x>>31;
    return x;
}

static uint64_t cell_hash(int64_t x,int64_t y) {
    const uint64_t a=mix64((uint64_t)x);
    const uint64_t b=mix64((uint64_t)y+UINT64_C(0x9e3779b97f4a7c15));
    return mix64(a^(b+UINT64_C(0x9e3779b97f4a7c15)+(a<<6)+(a>>2)));
}

static int64_t floor_div(int64_t value,int64_t divisor) {
    int64_t q=value/divisor;
    const int64_t r=value%divisor;

    if(r<0)--q;
    return q;
}

static bool reserve_cells(
    IntSpatialHash *hash,
    size_t needed
) {
    if(needed<=hash->cell_capacity)return true;

    size_t capacity=hash->cell_capacity==0?16:hash->cell_capacity;

    while(capacity<needed) {
        if(capacity>SIZE_MAX/2) {
            capacity=needed;
            break;
        }
        capacity*=2;
    }

    if(capacity>SIZE_MAX/sizeof *hash->cells)return false;

    SpatialCell *next=realloc(
        hash->cells,capacity*sizeof *next
    );

    if(next==NULL)return false;

    hash->cells=next;
    hash->cell_capacity=capacity;
    return true;
}

static bool reserve_points(
    SpatialCell *cell,
    size_t needed
) {
    if(needed<=cell->point_capacity)return true;

    size_t capacity=cell->point_capacity==0?4:cell->point_capacity;

    while(capacity<needed) {
        if(capacity>SIZE_MAX/2) {
            capacity=needed;
            break;
        }
        capacity*=2;
    }

    if(capacity>SIZE_MAX/sizeof *cell->points)return false;

    IntSpatialPoint *next=realloc(
        cell->points,capacity*sizeof *next
    );

    if(next==NULL)return false;

    cell->points=next;
    cell->point_capacity=capacity;
    return true;
}

static bool allocate_buckets(
    IntSpatialHash *hash,
    size_t capacity
) {
    if(capacity>SIZE_MAX/sizeof *hash->bucket_heads) {
        return false;
    }

    size_t *buckets=malloc(
        capacity*sizeof *buckets
    );

    if(buckets==NULL)return false;

    for(size_t i=0;i<capacity;++i)buckets[i]=SIZE_MAX;

    free(hash->bucket_heads);
    hash->bucket_heads=buckets;
    hash->bucket_capacity=capacity;
    return true;
}

IntSpatialHash *int_spatial_hash_create(int64_t cell_size) {
    if(cell_size<=0)return NULL;

    IntSpatialHash *hash=calloc(1,sizeof *hash);
    if(hash==NULL)return NULL;

    hash->cell_size=cell_size;

    if(!allocate_buckets(hash,16)) {
        free(hash);
        return NULL;
    }

    return hash;
}

void int_spatial_hash_free(IntSpatialHash *hash) {
    if(hash==NULL)return;

    for(size_t i=0;i<hash->cell_count;++i) {
        free(hash->cells[i].points);
    }

    free(hash->cells);
    free(hash->bucket_heads);
    free(hash);
}

size_t int_spatial_hash_size(const IntSpatialHash *hash) {
    return hash==NULL?0:hash->size;
}

size_t int_spatial_hash_cell_count(const IntSpatialHash *hash) {
    return hash==NULL?0:hash->cell_count;
}

static size_t bucket_index(
    const IntSpatialHash *hash,
    int64_t cx,int64_t cy
) {
    return (size_t)cell_hash(cx,cy)&(hash->bucket_capacity-1);
}

static size_t find_cell(
    const IntSpatialHash *hash,
    int64_t cx,int64_t cy
) {
    const size_t bucket=bucket_index(hash,cx,cy);
    size_t index=hash->bucket_heads[bucket];

    while(index!=SIZE_MAX) {
        const SpatialCell *cell=&hash->cells[index];

        if(cell->cell_x==cx&&cell->cell_y==cy) {
            return index;
        }

        index=cell->next;
    }

    return SIZE_MAX;
}

static bool rehash(IntSpatialHash *hash,size_t new_capacity) {
    if(new_capacity<16||(new_capacity&(new_capacity-1))!=0) {
        return false;
    }

    if(new_capacity>SIZE_MAX/sizeof *hash->bucket_heads) {
        return false;
    }

    size_t *new_heads=malloc(
        new_capacity*sizeof *new_heads
    );

    if(new_heads==NULL)return false;

    for(size_t i=0;i<new_capacity;++i)new_heads[i]=SIZE_MAX;

    for(size_t i=0;i<hash->cell_count;++i) {
        SpatialCell *cell=&hash->cells[i];
        const size_t bucket=
            (size_t)cell_hash(cell->cell_x,cell->cell_y)&
            (new_capacity-1);

        cell->next=new_heads[bucket];
        new_heads[bucket]=i;
    }

    free(hash->bucket_heads);
    hash->bucket_heads=new_heads;
    hash->bucket_capacity=new_capacity;
    return true;
}

static bool maybe_grow_table(IntSpatialHash *hash) {
    if(hash->cell_count+1<=
       hash->bucket_capacity-(hash->bucket_capacity/4)) {
        return true;
    }

    if(hash->bucket_capacity>SIZE_MAX/2)return false;

    return rehash(hash,hash->bucket_capacity*2);
}

bool int_spatial_hash_insert(
    IntSpatialHash *hash,
    IntSpatialPoint point
) {
    if(hash==NULL||hash->size==SIZE_MAX)return false;

    const int64_t cx=floor_div(point.x,hash->cell_size);
    const int64_t cy=floor_div(point.y,hash->cell_size);

    size_t index=find_cell(hash,cx,cy);
    bool created=false;
    size_t created_bucket=SIZE_MAX;

    if(index==SIZE_MAX) {
        if(!maybe_grow_table(hash))return false;

        if(hash->cell_count==SIZE_MAX||
           !reserve_cells(hash,hash->cell_count+1)) {
            return false;
        }

        index=hash->cell_count++;
        SpatialCell *cell=&hash->cells[index];

        *cell=(SpatialCell){
            .cell_x=cx,
            .cell_y=cy,
            .next=SIZE_MAX
        };

        created_bucket=bucket_index(hash,cx,cy);
        cell->next=hash->bucket_heads[created_bucket];
        hash->bucket_heads[created_bucket]=index;
        created=true;
    }

    SpatialCell *cell=&hash->cells[index];

    if(cell->point_count==SIZE_MAX||
       !reserve_points(cell,cell->point_count+1)) {
        if(created) {
            hash->bucket_heads[created_bucket]=cell->next;
            *cell=(SpatialCell){0};
            --hash->cell_count;
        }
        return false;
    }

    cell->points[cell->point_count++]=point;
    ++hash->size;
    return true;
}

static bool point_inside(
    const IntSpatialPoint *p,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh
) {
    return xl<=p->x&&p->x<xh&&
           yl<=p->y&&p->y<yh;
}

static bool query_cells(
    const IntSpatialHash *hash,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    IntSpatialPoint *output,
    size_t capacity,
    size_t *out_count,
    bool report
) {
    const int64_t first_x=floor_div(xl,hash->cell_size);
    const int64_t last_x=floor_div(xh-1,hash->cell_size);
    const int64_t first_y=floor_div(yl,hash->cell_size);
    const int64_t last_y=floor_div(yh-1,hash->cell_size);

    size_t count=0;

    for(int64_t cx=first_x;;) {
        for(int64_t cy=first_y;;) {
            const size_t index=find_cell(hash,cx,cy);

            if(index!=SIZE_MAX) {
                const SpatialCell *cell=&hash->cells[index];

                for(size_t i=0;i<cell->point_count;++i) {
                    if(point_inside(
                            &cell->points[i],xl,xh,yl,yh
                        )) {
                        if(report) {
                            if(count>=capacity)return false;
                            output[count]=cell->points[i];
                        }
                        ++count;
                    }
                }
            }

            if(cy==last_y)break;
            ++cy;
        }

        if(cx==last_x)break;
        ++cx;
    }

    *out_count=count;
    return true;
}

bool int_spatial_hash_query_count(
    const IntSpatialHash *hash,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    size_t *out_count
) {
    if(hash==NULL||out_count==NULL||
       xl>=xh||yl>=yh) {
        return false;
    }

    return query_cells(
        hash,xl,xh,yl,yh,
        NULL,0,out_count,false
    );
}

bool int_spatial_hash_query_report(
    const IntSpatialHash *hash,
    int64_t xl,int64_t xh,
    int64_t yl,int64_t yh,
    IntSpatialPoint *output,
    size_t capacity,
    size_t *out_written
) {
    if(hash==NULL||out_written==NULL||
       xl>=xh||yl>=yh) {
        return false;
    }

    size_t needed=0;

    if(!int_spatial_hash_query_count(
            hash,xl,xh,yl,yh,&needed
        )) {
        return false;
    }

    if(needed>capacity||(needed>0&&output==NULL)) {
        return false;
    }

    return query_cells(
        hash,xl,xh,yl,yh,
        output,capacity,out_written,true
    );
}

bool int_spatial_hash_validate(const IntSpatialHash *hash) {
    if(hash==NULL||hash->cell_size<=0||
       hash->bucket_capacity<16||
       (hash->bucket_capacity&(hash->bucket_capacity-1))!=0||
       hash->bucket_heads==NULL||
       hash->cell_count>hash->cell_capacity) {
        return false;
    }

    bool *seen=calloc(
        hash->cell_count==0?1:hash->cell_count,
        sizeof *seen
    );

    if(seen==NULL)return false;

    size_t cells_seen=0;
    size_t points_seen=0;

    for(size_t b=0;b<hash->bucket_capacity;++b) {
        size_t index=hash->bucket_heads[b];

        while(index!=SIZE_MAX) {
            if(index>=hash->cell_count||seen[index]) {
                free(seen);
                return false;
            }

            seen[index]=true;
            ++cells_seen;

            const SpatialCell *cell=&hash->cells[index];

            if(bucket_index(hash,cell->cell_x,cell->cell_y)!=b||
               cell->point_count>cell->point_capacity) {
                free(seen);
                return false;
            }

            for(size_t i=0;i<cell->point_count;++i) {
                const IntSpatialPoint *p=&cell->points[i];

                if(floor_div(p->x,hash->cell_size)!=cell->cell_x||
                   floor_div(p->y,hash->cell_size)!=cell->cell_y) {
                    free(seen);
                    return false;
                }
            }

            if(SIZE_MAX-points_seen<cell->point_count) {
                free(seen);
                return false;
            }

            points_seen+=cell->point_count;
            index=cell->next;
        }
    }

    free(seen);

    return cells_seen==hash->cell_count&&
           points_seen==hash->size;
}
