#pragma warning( disable: 6011 )
#pragma warning( disable: 6385 )
#pragma warning( disable: 6386 )

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "world.h"
#include "main.h"

void chunkUpdate(World world, int cx, int cy, int scanDir);
void pix_Sand(ChunkBlock block, int x, int y);
void pix_Water(ChunkBlock block, int x, int y);
bool attemptSwap(ChunkBlock block, int x, int y, int dx, int dy, int* cmpElements, int cmpElemCt);

int* sandSwapElements;
int* waterSwapElements;

// ~115FPS with sqSize = 8, 240 UPS
World new_World() {
	World this = (World)malloc(sizeof(struct World));
	this->chunks = new_List();
	this->chunkMap = new_IntHashMap();

	sandSwapElements = (int*)malloc(2 * sizeof(int));
	sandSwapElements[0] = PIX_VOID;
	sandSwapElements[1] = PIX_WATER;

	waterSwapElements = (int*)malloc(sizeof(int));
	waterSwapElements[0] = PIX_VOID;

	int sqSize = 4;
	this->chunkBorderRight = this->chunkBorderTop = sqSize;
	this->chunkBorderLeft = this->chunkBorderBottom = -sqSize;
	for (int i = -sqSize; i <= sqSize; i++) {
		for (int j = -sqSize; j <= sqSize; j++) {
			Chunk c = new_Chunk(i, j);
			addChunk(this, c);
		}
	}
	for (int i = 0; i < this->chunks->count; i++) {
		Chunk c = List_get(this->chunks, i);
		for (int i = 0; i < 64; i++) {
			for (int j = 0; j < 64; j++) {
				c->data[i][j][0] = (char)PIX_VOID;
			}
		}
		glGenTextures(1, &c->texture);
		glBindTexture(GL_TEXTURE_2D, c->texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, c->data);
	}
	return this;
}

void addChunk(World world, Chunk c) {
	List_add(world->chunks, c);
	int hashX = (unsigned short)c->posX * 2 - (c->posX < 0 ? 1 : 0);
	int hashY = (unsigned short)c->posY * 2 - (c->posY < 0 ? 1 : 0);
	unsigned int hash = (unsigned int)((unsigned short)hashX << 16) | (unsigned short)(hashY);
	//printf("(%d, %d)    hash = (%d, %d) => %d\n", c->posX, c->posY, hashX, hashY, hash);
	IntHashMap_insert(world->chunkMap, hash, c);
}
Chunk locateChunk(World world, int x, int y) {
	if (x < world->chunkBorderLeft || x > world->chunkBorderRight
		|| y < world->chunkBorderBottom || y > world->chunkBorderTop) return NULL;
	int hashX = (unsigned short)x * 2 - (x < 0 ? 1 : 0);
	int hashY = (unsigned short)y * 2 - (y < 0 ? 1 : 0);
	unsigned int hash = (unsigned int)((unsigned short)hashX << 16) | (unsigned short)(hashY);
	//printf("loc (%d, %d)    hash = (%d, %d) => %d\n", x, y, hashX, hashY, hash);
	return IntHashMap_get(world->chunkMap, hash);
}
void removeChunk(World world, Chunk c) {
	for (int i = 0; i < world->chunks->count; i++) {
		if (List_get(world->chunks, i) == c) {
			List_remove(world->chunks, i);
			break;
		}
	}
	int hashX = (unsigned short)c->posX * 2 - (c->posX < 0 ? 1 : 0);
	int hashY = (unsigned short)c->posY * 2 - (c->posY < 0 ? 1 : 0);
	unsigned int hash = (unsigned int)((unsigned short)hashX << 16) | (unsigned short)(hashY);
	IntHashMap_remove(world->chunkMap, hash);
	Chunk_free(c);
}

void World_update(World world) {
	for (int i = 0; i < world->chunks->count; i++) {
		Chunk c = List_get(world->chunks, i);
	}

	for (int j = world->chunkBorderBottom; j <= world->chunkBorderTop; j++) {
		for (int i = world->chunkBorderLeft; i <= world->chunkBorderRight; i++) {
			chunkUpdate(world, i, j, rand() % 4);
		}
	}

	for (int i = 0; i < world->chunks->count; i++) {
		Chunk c = List_get(world->chunks, i);		

		for (int x = 0; x < 64; x++) {
			for (int y = 0; y < 64; y++) {
				//c->data[x][y][1] = c->data[x][y][0];
			}
		}
	}
	updateChunkTextures();
}

void chunkUpdate(World world, int cx, int cy, int scanDir) {
	Chunk c = locateChunk(world, cx, cy);
	//if (c->sleeping) return; //TODO: FIX? might be good

	ChunkBlock block = new_ChunkBlock(
		c,
		locateChunk(world, cx - 1, cy),
		locateChunk(world, cx - 1, cy + 1),
		locateChunk(world, cx, cy + 1),
		locateChunk(world, cx + 1, cy + 1),
		locateChunk(world, cx + 1, cy),
		locateChunk(world, cx + 1, cy - 1),
		locateChunk(world, cx, cy - 1),
		locateChunk(world, cx - 1, cy - 1));

	if (scanDir == 0) {
		for (int x = 0; x < 64; x++) {
			for (int y = 0; y < 64; y++) {
				if (c->data[x][y][0] == PIX_SAND) { //sand behavior
					pix_Sand(block, x, y);
				}
				if (c->data[x][y][0] == PIX_WATER) { //water behavior
					pix_Water(block, x, y);
				}
			}
		}
	}
	else if (scanDir == 1) {
		for (int x = 63; x >= 0; x--) {
			for (int y = 0; y < 64; y++) {
				if (c->data[x][y][0] == PIX_SAND) { //sand behavior
					pix_Sand(block, x, y);
				}
				if (c->data[x][y][0] == PIX_WATER) { //water behavior
					pix_Water(block, x, y);
				}
			}
		}
	}
	else if (scanDir == 2) {
		for (int x = 0; x < 64; x++) {
			for (int y = 63; y >= 0; y--) {
				if (c->data[x][y][0] == PIX_SAND) { //sand behavior
					pix_Sand(block, x, y);
				}
				if (c->data[x][y][0] == PIX_WATER) { //water behavior
					pix_Water(block, x, y);
				}
			}
		}
	}
	else if (scanDir == 3) {
		for (int x = 63; x >= 0; x--) {
			for (int y = 63; y >= 0; y--) {
				if (c->data[x][y][0] == PIX_SAND) { //sand behavior
					pix_Sand(block, x, y);
				}
				if (c->data[x][y][0] == PIX_WATER) { //water behavior
					pix_Water(block, x, y);
				}
			}
		}
	}
	free(block);
}

void pix_Sand(ChunkBlock block, int x, int y) {
	block->chunks[1][1]->data[x][y][1] = (char)min(20, max(-20, ((int)block->chunks[1][1]->data[x][y][1] + (int)block->chunks[1][1]->data[x][y][2])));
	int dy = (int)block->chunks[1][1]->data[x][y][1];
	int dx = 1;
	if(attemptSwap(block, x, y, 0, dy, sandSwapElements, 2)) return; //move down
	if (attemptSwap(block, x, y, -dx, dy, sandSwapElements, 2)) return; //move down/left
	if (attemptSwap(block, x, y, dx, dy, sandSwapElements, 2)) return; //move down/right

	//nothing happened, pixel is stationary
	block->chunks[1][1]->data[x][y][1] = 0;
}

void pix_Water(ChunkBlock block, int x, int y) {
	block->chunks[1][1]->data[x][y][1] = (char)min(31, max(-31, ((int)block->chunks[1][1]->data[x][y][1] + (int)block->chunks[1][1]->data[x][y][2])));
	int dy = (int)block->chunks[1][1]->data[x][y][1];
	int dx = 1;
	if (attemptSwap(block, x, y, 0, dy, waterSwapElements, 1)) return; //move down
	if (attemptSwap(block, x, y, -dx, dy, waterSwapElements, 1)) return; //move down/left
	if (attemptSwap(block, x, y, dx, dy, waterSwapElements, 1)) return; //move down/right
	if (attemptSwap(block, x, y, -dx, 0, waterSwapElements, 1)) return; //move left
	if (attemptSwap(block, x, y, dx, 0, waterSwapElements, 1)) return; //move right

	//nothing happened, pixel is stationary
	block->chunks[1][1]->data[x][y][1] = 0;
}

bool attemptSwap(ChunkBlock block, int x1, int y1, int dx, int dy, int* cmpElements, int cmpElemCt) {

	x1 += 64; y1 += 64;
	int x2 = x1 + dx; int y2 = y1 + dy;
	int lBoundX = block->chunks[0][1] ? 0 : 64; int uBoundX = block->chunks[2][1] ? 191 : 127;
	int lBoundY = block->chunks[1][0] ? 0 : 64; int uBoundY = block->chunks[1][2] ? 191 : 127;
	x2 = min(uBoundX, max(lBoundX, x2));
	y2 = min(uBoundY, max(lBoundY, y2));

	int curX = x1; int curY = y1;
	int xDir = dx > 0 ? 1 : -1; int yDir = dy > 0 ? 1 : -1;
	int swapX = x1; int swapY = y1;

	while (curX != x2 || curY != y2) {
		curX += (curX != x2) * xDir;
		curY += (curY != y2) * yDir;
		if (curX < lBoundX || curX > uBoundX || curY < lBoundY || curY > uBoundY) break;
		int canSwap = 0;
		for (int i = 0; i < cmpElemCt; i++) {
			if (block->chunks[curX / 64][curY / 64]->data[curX % 64][curY % 64][0] == cmpElements[i]) { //can swap
				swapX = curX; swapY = curY;
				canSwap = 1;
				break;
			}
		}
		if (!canSwap) break;
	}

	if (swapX == x1 && swapY == y1) return false;
	char tmp[4];
	for (int i = 0; i < 4; i++) {
		tmp[i] = block->chunks[1][1]->data[x1 - 64][y1 - 64][i];
	}
	for (int i = 0; i < 4; i++) {
		block->chunks[1][1]->data[x1 - 64][y1 - 64][i] = block->chunks[swapX / 64][swapY / 64]->data[swapX % 64][swapY % 64][i];
		block->chunks[swapX / 64][swapY / 64]->data[swapX % 64][swapY % 64][i] = tmp[i];
	}
	return true;

	/*
	x1 += 64; y1 += 64;
	int x2 = x1 + dx; int y2 = y1 + dy;
	int lBoundX = block->chunks[0][1] ? 0 : 64; int uBoundX = block->chunks[2][1] ? 191 : 127;
	int lBoundY = block->chunks[1][0] ? 0 : 64; int uBoundY = block->chunks[1][2] ? 191 : 127;
	x2 = min(uBoundX, max(lBoundX, x2));
	y2 = min(uBoundY, max(lBoundY, y2));

	//using a vector step traversal
	int curX = x1; int curY = y1;
	float fcurX = (float)x1; float fcurY = (float)y1;
	int xDir = dx > 0 ? 1 : -1; int yDir = dy > 0 ? 1 : -1;
	int swapX = x1; int swapY = y1;


	if (abs(dy) >= abs(dx)) {
		float slope = (float)dx / (float)dy;
		bool firstIteration = true;
		while (curY != y2) {
			if (firstIteration && abs(dx) > 0) {
				//traverse both direcitons on first iteration if applicable
				firstIteration = false;
				curX += xDir; curY += yDir;
				slope = (float)(dx - xDir) / (float)(dy - yDir); //slope offset
			}
			else {
				curY += yDir;
				fcurX += slope; curX = (int)roundf(fcurX);
			}
			if (curX < lBoundX || curX > uBoundX) break;

			int canSwap = 0;
			for (int i = 0; i < cmpElemCt; i++) {
				if (block->chunks[curX / 64][curY / 64]->data[curX % 64][curY % 64][0] == cmpElements[i]) { //can swap
					swapX = curX; swapY = curY;
					canSwap = 1;
					break;
				}
			}
			if (!canSwap) break;
		}
	}
	else {
		float slope = (float)dy / (float)dx;
		bool firstIteration = true;
		while (curX != x2) {
			if (firstIteration && abs(dy) > 0) {
				//traverse both direcitons on first iteration if applicable
				firstIteration = false;
				curX += xDir; curY += yDir;
				slope = (float)(dy - yDir) / (float)(dx - xDir); //slope offset
			}
			else {
				curX += xDir;
				fcurY += slope; curY = (int)roundf(fcurY);
			}
			if (curY < lBoundY || curY > uBoundY) break;

			int canSwap = 0;
			for (int i = 0; i < cmpElemCt; i++) {
				if (block->chunks[curX / 64][curY / 64]->data[curX % 64][curY % 64][0] == cmpElements[i]) { //can swap
					swapX = curX; swapY = curY;
					canSwap = 1;
					break;
				}
			}
			if (!canSwap) break;
		}
		
	}

	if (swapX == x1 && swapY == y1) return false;
	char tmp[4];
	for (int i = 0; i < 4; i++) {
		tmp[i] = block->chunks[1][1]->data[x1 - 64][y1 - 64][i];
	}
	for (int i = 0; i < 4; i++) {
		block->chunks[1][1]->data[x1 - 64][y1 - 64][i] = block->chunks[swapX / 64][swapY / 64]->data[swapX % 64][swapY % 64][i];
		block->chunks[swapX / 64][swapY / 64]->data[swapX % 64][swapY % 64][i] = tmp[i];
	}
	return true;*/
}

void World_free(World world) {

}