#include <stdlib.h>
#include "shader.h"
#include "chunk.h"

Chunk new_Chunk(int posX, int posY) {
	Chunk this = (Chunk)malloc(sizeof(struct Chunk));
	if (this == NULL) return NULL;

	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			this->data[i][j][0] = (char)0;
			this->data[i][j][1] = (char)0;
			this->data[i][j][2] = (char)-1;
			this->data[i][j][3] = (char)0;
		}
	}
	//this->shader = new_Shader("shaders\\def_world.vshader", "shaders\\def_pix.fshader");

	//set uniform block of vertex shader
	//unsigned int uniformGlobalInfo = glGetUniformBlockIndex(this->shader->id, "GlobalInfo");
	//glUniformBlockBinding(this->shader->id, uniformGlobalInfo, 0);

	this->sleeping = false;
	this->texture = 0;
	this->posX = posX;
	this->posY = posY;
	//Shader_setVec2(this->shader, "pos", (float)posX, (float)posY);
	return this;
}

void Chunk_free(Chunk c) {
	free(c);
}

ChunkBlock new_ChunkBlock(Chunk c, Chunk w, Chunk nw, Chunk n, Chunk ne, Chunk e, Chunk se, Chunk s, Chunk sw) {
	ChunkBlock this = (ChunkBlock)malloc(sizeof(struct ChunkBlock));
	if (this == NULL) return NULL;

	this->chunks[0][0] = sw;
	this->chunks[0][1] = w;
	this->chunks[0][2] = nw;
	this->chunks[1][0] = s;
	this->chunks[1][1] = c;
	this->chunks[1][2] = n;
	this->chunks[2][0] = se;
	this->chunks[2][1] = e;
	this->chunks[2][2] = ne;

	return this;
}