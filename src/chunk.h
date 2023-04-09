#ifndef _chunk_h
#define _chunk_h

#include "shader.h"
#include <stdbool.h>

typedef struct Chunk {
	//Shader shader;
	int posX, posY;
	bool sleeping;

	/*
	 data[x][y][0] = current pixel
	 data[x][y][1] = y velocity
	 data[x][y][2] = y acceleration
	 data[x][y][3] = 
	*/
	char data[64][64][4];
	
	/*
	 fdata[x][y][0] = y acceleration
	 fdata[x][y][1] = y velocity
	 */
	//float fdata[64][64][2];

	unsigned int texture;
	
}
*Chunk;

/*
 * Chunk constructor. 
 */
extern Chunk new_Chunk(int posX, int posY);

/*
 * Frees the chunk.
 */
extern void Chunk_free(Chunk c);

typedef struct ChunkBlock {
	Chunk chunks[3][3];
}
*ChunkBlock;
//TODO: after hashmap fix, if still slow
extern ChunkBlock new_ChunkBlock(Chunk c, Chunk w, Chunk nw, Chunk n, Chunk ne, Chunk e, Chunk se, Chunk s, Chunk sw);

#endif