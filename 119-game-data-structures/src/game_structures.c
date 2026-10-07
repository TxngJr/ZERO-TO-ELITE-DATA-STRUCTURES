#include "game_structures.h"
#include <math.h>
#include <stdlib.h>

enum { NONE = SIZE_MAX };

typedef struct {
    uint32_t generation;
    bool active;
} EntitySlot;

struct GameWorld {
    size_t capacity;
    size_t active_count;
    size_t free_top;
    uint64_t version;
    EntitySlot *slots;
    size_t *free_stack;
    size_t *sparse;
    size_t *dense_slots;
    float *dense_x;
    float *dense_y;
    size_t position_count;
};

struct SpatialGrid {
    size_t entity_capacity;
    float min_x,min_y,max_x,max_y,cell_size;
    size_t cols,rows,cell_count;
    size_t *heads;
    size_t *next;
    uint64_t built_version;
    bool built;
};

static EntityId make_entity(size_t slot,uint32_t generation){
    return ((uint64_t)generation<<32U)|(uint64_t)(uint32_t)slot;
}

static bool decode(const GameWorld *world,EntityId entity,size_t *out_slot){
    if(!world)return false;
    size_t slot=(size_t)(uint32_t)entity;
    uint32_t generation=(uint32_t)(entity>>32U);
    if(slot>=world->capacity||generation==0U)return false;
    if(!world->slots[slot].active||world->slots[slot].generation!=generation)return false;
    if(out_slot)*out_slot=slot;
    return true;
}

GameWorld *gw_create(size_t capacity){
    if(capacity==0U||capacity>UINT32_MAX||
       capacity>SIZE_MAX/sizeof(EntitySlot)||
       capacity>SIZE_MAX/sizeof(size_t)||
       capacity>SIZE_MAX/sizeof(float))return NULL;
    GameWorld*w=calloc(1,sizeof(*w));if(!w)return NULL;
    w->slots=calloc(capacity,sizeof(*w->slots));
    w->free_stack=malloc(capacity*sizeof(*w->free_stack));
    w->sparse=malloc(capacity*sizeof(*w->sparse));
    w->dense_slots=malloc(capacity*sizeof(*w->dense_slots));
    w->dense_x=malloc(capacity*sizeof(*w->dense_x));
    w->dense_y=malloc(capacity*sizeof(*w->dense_y));
    if(!w->slots||!w->free_stack||!w->sparse||!w->dense_slots||!w->dense_x||!w->dense_y){
        gw_free(w);return NULL;
    }
    w->capacity=capacity;w->free_top=capacity;w->version=1U;
    for(size_t i=0;i<capacity;++i){w->free_stack[i]=capacity-1U-i;w->sparse[i]=NONE;}
    return w;
}

void gw_free(GameWorld*w){
    if(!w)return;
    free(w->slots);free(w->free_stack);free(w->sparse);
    free(w->dense_slots);free(w->dense_x);free(w->dense_y);free(w);
}

bool gw_spawn(GameWorld*w,EntityId*out){
    if(!w||!out||w->free_top==0U)return false;
    size_t slot=w->free_stack[--w->free_top];
    EntitySlot*s=&w->slots[slot];
    ++s->generation;if(s->generation==0U)++s->generation;
    s->active=true;w->sparse[slot]=NONE;++w->active_count;++w->version;
    if(w->version==0U)++w->version;
    *out=make_entity(slot,s->generation);return true;
}

static bool remove_position_slot(GameWorld*w,size_t slot){
    size_t dense=w->sparse[slot];
    if(dense==NONE)return false;
    size_t last=w->position_count-1U;
    if(dense!=last){
        size_t moved=w->dense_slots[last];
        w->dense_slots[dense]=moved;
        w->dense_x[dense]=w->dense_x[last];
        w->dense_y[dense]=w->dense_y[last];
        w->sparse[moved]=dense;
    }
    w->sparse[slot]=NONE;--w->position_count;return true;
}

bool gw_destroy(GameWorld*w,EntityId entity){
    size_t slot=0U;if(!decode(w,entity,&slot))return false;
    if(w->sparse[slot]!=NONE)remove_position_slot(w,slot);
    w->slots[slot].active=false;--w->active_count;
    w->free_stack[w->free_top++]=slot;++w->version;if(w->version==0U)++w->version;
    return true;
}

bool gw_set_position(GameWorld*w,EntityId entity,float x,float y){
    size_t slot=0U;if(!decode(w,entity,&slot)||!isfinite(x)||!isfinite(y))return false;
    size_t dense=w->sparse[slot];
    if(dense==NONE){
        if(w->position_count>=w->capacity)return false;
        dense=w->position_count++;
        w->dense_slots[dense]=slot;w->sparse[slot]=dense;
    }
    w->dense_x[dense]=x;w->dense_y[dense]=y;
    ++w->version;if(w->version==0U)++w->version;
    return true;
}

bool gw_get_position(const GameWorld*w,EntityId entity,float*out_x,float*out_y){
    size_t slot=0U;if(!out_x||!out_y||!decode(w,entity,&slot))return false;
    size_t dense=w->sparse[slot];if(dense==NONE)return false;
    *out_x=w->dense_x[dense];*out_y=w->dense_y[dense];return true;
}

bool gw_remove_position(GameWorld*w,EntityId entity,bool*out_removed){
    size_t slot=0U;if(!decode(w,entity,&slot))return false;
    bool removed=remove_position_slot(w,slot);
    if(removed){++w->version;if(w->version==0U)++w->version;}
    if(out_removed)*out_removed=removed;return true;
}

size_t gw_entity_count(const GameWorld*w){return w?w->active_count:0U;}
size_t gw_position_count(const GameWorld*w){return w?w->position_count:0U;}
size_t gw_capacity(const GameWorld*w){return w?w->capacity:0U;}
uint64_t gw_version(const GameWorld*w){return w?w->version:0U;}

bool gw_validate(const GameWorld*w){
    if(!w||!w->slots||!w->free_stack||!w->sparse||!w->dense_slots||
       !w->dense_x||!w->dense_y||w->capacity==0U||w->free_top>w->capacity||
       w->active_count>w->capacity||w->position_count>w->active_count)return false;
    unsigned char*seen=calloc(w->capacity,1U);if(!seen)return false;
    for(size_t d=0;d<w->position_count;++d){
        size_t slot=w->dense_slots[d];
        if(slot>=w->capacity||seen[slot]||!w->slots[slot].active||
           w->sparse[slot]!=d||!isfinite(w->dense_x[d])||!isfinite(w->dense_y[d])){
            free(seen);return false;
        }
        seen[slot]=1U;
    }
    size_t active=0U;
    for(size_t s=0;s<w->capacity;++s){
        if(w->slots[s].active){
            ++active;
            if(w->slots[s].generation==0U){free(seen);return false;}
            if(w->sparse[s]!=NONE){
                if(w->sparse[s]>=w->position_count||w->dense_slots[w->sparse[s]]!=s){
                    free(seen);return false;
                }
            }
        }else if(w->sparse[s]!=NONE){
            free(seen);return false;
        }
    }
    for(size_t i=0;i<w->free_top;++i){
        size_t s=w->free_stack[i];
        if(s>=w->capacity||w->slots[s].active||seen[s]==2U){free(seen);return false;}
        seen[s]=2U;
    }
    bool ok=active==w->active_count&&w->free_top+w->active_count==w->capacity;
    free(seen);return ok;
}

SpatialGrid *grid_create(size_t entity_capacity,float min_x,float min_y,float max_x,float max_y,float cell_size){
    if(entity_capacity==0U||!isfinite(min_x)||!isfinite(min_y)||!isfinite(max_x)||
       !isfinite(max_y)||!isfinite(cell_size)||!(max_x>min_x)||!(max_y>min_y)||
       !(cell_size>0.0f))return NULL;
    size_t cols=(size_t)ceilf((max_x-min_x)/cell_size);
    size_t rows=(size_t)ceilf((max_y-min_y)/cell_size);
    if(cols==0U||rows==0U||cols>SIZE_MAX/rows)return NULL;
    size_t cells=cols*rows;
    if(cells>SIZE_MAX/sizeof(size_t)||entity_capacity>SIZE_MAX/sizeof(size_t))return NULL;
    SpatialGrid*g=calloc(1,sizeof(*g));if(!g)return NULL;
    g->heads=malloc(cells*sizeof(*g->heads));g->next=malloc(entity_capacity*sizeof(*g->next));
    if(!g->heads||!g->next){grid_free(g);return NULL;}
    g->entity_capacity=entity_capacity;g->min_x=min_x;g->min_y=min_y;g->max_x=max_x;
    g->max_y=max_y;g->cell_size=cell_size;g->cols=cols;g->rows=rows;g->cell_count=cells;
    return g;
}

void grid_free(SpatialGrid*g){if(!g)return;free(g->heads);free(g->next);free(g);}

static bool grid_cell(const SpatialGrid*g,float x,float y,size_t*out_cell){
    if(!(x>=g->min_x&&x<g->max_x&&y>=g->min_y&&y<g->max_y))return false;
    size_t col=(size_t)((x-g->min_x)/g->cell_size);
    size_t row=(size_t)((y-g->min_y)/g->cell_size);
    if(col>=g->cols) col=g->cols-1U;
    if(row>=g->rows) row=g->rows-1U;
    *out_cell=row*g->cols+col;return true;
}

bool grid_rebuild(SpatialGrid*g,const GameWorld*w){
    if(!g||!w||g->entity_capacity!=w->capacity)return false;
    for(size_t c=0;c<g->cell_count;++c)g->heads[c]=NONE;
    for(size_t s=0;s<g->entity_capacity;++s)g->next[s]=NONE;
    for(size_t d=0;d<w->position_count;++d){
        size_t slot=w->dense_slots[d],cell=0U;
        if(!grid_cell(g,w->dense_x[d],w->dense_y[d],&cell))return false;
        g->next[slot]=g->heads[cell];g->heads[cell]=slot;
    }
    g->built_version=w->version;g->built=true;return true;
}

bool grid_query_aabb(const SpatialGrid*g,const GameWorld*w,float qminx,float qminy,float qmaxx,float qmaxy,
                     EntityId*out,size_t out_capacity,size_t*out_count){
    if(!g||!w||!out_count||!g->built||g->built_version!=w->version||
       !isfinite(qminx)||!isfinite(qminy)||!isfinite(qmaxx)||!isfinite(qmaxy)||
       qmaxx<qminx||qmaxy<qminy||(out_capacity&& !out))return false;
    if(qmaxx<g->min_x||qmaxy<g->min_y||qminx>=g->max_x||qminy>=g->max_y){
        *out_count=0U;return true;
    }
    float ax=qminx<g->min_x?g->min_x:qminx;
    float ay=qminy<g->min_y?g->min_y:qminy;
    float bx=qmaxx>=g->max_x?g->max_x-0.000001f:qmaxx;
    float by=qmaxy>=g->max_y?g->max_y-0.000001f:qmaxy;
    size_t c0=(size_t)((ax-g->min_x)/g->cell_size),c1=(size_t)((bx-g->min_x)/g->cell_size);
    size_t r0=(size_t)((ay-g->min_y)/g->cell_size),r1=(size_t)((by-g->min_y)/g->cell_size);
    if(c1>=g->cols) c1=g->cols-1U;
    if(r1>=g->rows) r1=g->rows-1U;
    size_t written=0U;
    for(size_t r=r0;r<=r1;++r)for(size_t c=c0;c<=c1;++c){
        size_t slot=g->heads[r*g->cols+c];
        while(slot!=NONE){
            size_t dense=w->sparse[slot];
            if(dense!=NONE){
                float x=w->dense_x[dense],y=w->dense_y[dense];
                if(x>=qminx&&x<=qmaxx&&y>=qminy&&y<=qmaxy){
                    if(written<out_capacity)out[written]=make_entity(slot,w->slots[slot].generation);
                    ++written;
                }
            }
            slot=g->next[slot];
        }
    }
    *out_count=written;return true;
}

bool grid_validate(const SpatialGrid*g,const GameWorld*w){
    if(!g||!w||!g->built||g->built_version!=w->version||g->entity_capacity!=w->capacity)return false;
    unsigned char*seen=calloc(w->capacity,1U);if(!seen)return false;
    size_t count=0U;
    for(size_t cell=0;cell<g->cell_count;++cell){
        size_t slot=g->heads[cell];
        while(slot!=NONE){
            if(slot>=w->capacity||seen[slot]||!w->slots[slot].active||w->sparse[slot]==NONE){
                free(seen);return false;
            }
            size_t dense=w->sparse[slot],expected=0U;
            if(!grid_cell(g,w->dense_x[dense],w->dense_y[dense],&expected)||expected!=cell){
                free(seen);return false;
            }
            seen[slot]=1U;++count;slot=g->next[slot];
            if(count>w->position_count){free(seen);return false;}
        }
    }
    bool ok=count==w->position_count;
    free(seen);return ok;
}
