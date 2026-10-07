#ifndef GAME_STRUCTURES_H
#define GAME_STRUCTURES_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef uint64_t EntityId;
typedef struct GameWorld GameWorld;
typedef struct SpatialGrid SpatialGrid;
GameWorld *gw_create(size_t capacity);
void gw_free(GameWorld *world);
bool gw_spawn(GameWorld *world,EntityId *out_entity);
bool gw_destroy(GameWorld *world,EntityId entity);
bool gw_set_position(GameWorld *world,EntityId entity,float x,float y);
bool gw_get_position(const GameWorld *world,EntityId entity,float *out_x,float *out_y);
bool gw_remove_position(GameWorld *world,EntityId entity,bool *out_removed);
size_t gw_entity_count(const GameWorld *world);
size_t gw_position_count(const GameWorld *world);
size_t gw_capacity(const GameWorld *world);
uint64_t gw_version(const GameWorld *world);
bool gw_validate(const GameWorld *world);
SpatialGrid *grid_create(size_t entity_capacity,float min_x,float min_y,float max_x,float max_y,float cell_size);
void grid_free(SpatialGrid *grid);
bool grid_rebuild(SpatialGrid *grid,const GameWorld *world);
bool grid_query_aabb(const SpatialGrid *grid,const GameWorld *world,float min_x,float min_y,float max_x,float max_y,EntityId *out_entities,size_t out_capacity,size_t *out_count);
bool grid_validate(const SpatialGrid *grid,const GameWorld *world);
#endif
