#ifndef _world_h
#define _world_h

#define PIX_VOID 0
#define PIX_SAND 1
#define PIX_WATER 2

#include "chunk.h"
#include "structures/hashmap.h"
#include "structures/list.h"

typedef struct World {
	List chunks;
	IntHashMap chunkMap;
	int chunkBorderLeft, chunkBorderRight, chunkBorderTop, chunkBorderBottom;
}
*World;

extern World new_World();

extern void World_free(World world);

extern void World_update(World world);

extern void addChunk(World world, Chunk c);

extern Chunk locateChunk(World world, int x, int y);

extern void removeChunk(World world, Chunk c);

#endif