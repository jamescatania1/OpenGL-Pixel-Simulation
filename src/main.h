#ifndef _main_h
#define _main_h

#include "chunk.h"
#include "world.h"

double deltaTime;
double fixedDeltaTime;

extern void updateChunkTextures();

extern void setPixel(int pixMaterial, float worldX, float worldY);

#endif