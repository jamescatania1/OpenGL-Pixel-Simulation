#pragma warning( disable: 6286 )

#include <glad/glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <math.h>
#include "main.h"
#include "world.h"
#include "shader.h"
#include "input.h"
#include "quad.h"
#include "chunk.h"
#include "structures/hashmap.h"
#include "structures/list.h"

#define FPS_CAPPED 0
#define FPS_LIMIT 120
#define FIXED_UPDATES_PER_SECOND 120
#define SCREEN_DEFAULT_WIDTH 800
#define SCREEN_DEFAULT_HEIGHT 800

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void update();
void fixedUpdate();
void draw();

unsigned int quadVAO;
unsigned int camGlobalUniformBuffer;
float camVelX, camVelY, camPosX, camPosY, targCamPosX, targCamPosY, viewSize, targViewSize;
GLFWwindow* window;
int activeBrushPixel;
World world;
Shader chunkShader;

int main() {
	//initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create GLFW window
	window = glfwCreateWindow(SCREEN_DEFAULT_WIDTH, SCREEN_DEFAULT_HEIGHT, "Pixel Simulation", NULL, NULL);
	if (window == NULL) {
		printf("failed to create window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("failed to initialize GLAD\n");
		glfwTerminate();
		return -1;
	}
	
	//set resize callback function
	framebuffer_size_callback(window, SCREEN_DEFAULT_WIDTH, SCREEN_DEFAULT_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	camPosX = targCamPosX = 0.0f;
	camPosY = targCamPosY = 0.0f;
	camVelX = 0.0f;
	camVelY = 0.0f;
	targViewSize = viewSize = 8.0f;

	//initialize world
	world = new_World();
	quadVAO = createQuad();
	activeBrushPixel = PIX_SAND;
	chunkShader = new_Shader("shaders\\def_world.vshader", "shaders\\def_pix.fshader");
	unsigned int uniformGlobalInfo = glGetUniformBlockIndex(chunkShader->id, "GlobalInfo");
	glUniformBlockBinding(chunkShader->id, uniformGlobalInfo, 0);

	//create global buffer object for camera data
	glGenBuffers(1, &camGlobalUniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, camGlobalUniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, 12, NULL, GL_STATIC_DRAW); //12 bytes
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, camGlobalUniformBuffer, 0, 12);

	//main loop
	double lastDrawTime = 0.0;
	double lastFixedUpdateTime = 0.0;
	double lastUpdateTime = 0.0;
	while (!glfwWindowShouldClose(window)) {

		//input
		updateInputManager(window);

		//update loop
		update();
		deltaTime = glfwGetTime() - lastUpdateTime;
		lastUpdateTime = glfwGetTime();

		//fixed update
		fixedDeltaTime = glfwGetTime() - lastFixedUpdateTime;
		if (glfwGetTime() - lastFixedUpdateTime > (1.0 / (double)FIXED_UPDATES_PER_SECOND)) {
			lastFixedUpdateTime = glfwGetTime();

			updateInputManagerFixedTime(window);
			fixedUpdate();
		}

		//rendering
		if (!FPS_CAPPED || glfwGetTime() - lastDrawTime > (1.0 / (double)FPS_LIMIT)) {
			lastDrawTime = glfwGetTime();
			draw();

			//check events and swap buffers
			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	World_free(world);
	InputManager_free();
	glfwTerminate();
	return 0;
}

void update() {
	float scrollSizeMultiplier = log2f(targViewSize) + 0.5f;
	targViewSize += -20.0f * scrollSizeMultiplier * deltaTime * mouseState->scrollVal;
	targViewSize = fmaxf(1.0f, fminf(10.0f, targViewSize));
	viewSize += (targViewSize - viewSize) * fminf(0.6f, 70.0f * deltaTime);

	float inputY = 0.0f;
	float inputX = 0.0f;
	float scrollPanMultiplier = _fdlog(targViewSize, 5) + 0.75f;
	if (keyState->key_W->down) inputY += 1.0f;
	if (keyState->key_S->down) inputY -= 1.0f;
	if (keyState->key_A->down) inputX -= 1.0f;
	if (keyState->key_D->down) inputX += 1.0f;
	float norm = fmaxf(sqrtf(fabs(inputX) + fabs(inputY)), 1.0f);
	inputX /= norm;
	inputY /= norm;
	targCamPosX += -5.0f * scrollPanMultiplier * inputX * deltaTime;
	targCamPosY += -5.0f * scrollPanMultiplier * inputY * deltaTime;
	camPosX += (targCamPosX - camPosX) * fminf(0.5f, 65.0f * deltaTime);
	camPosY += (targCamPosY - camPosY) * fminf(0.5f, 65.0f * deltaTime);

	//set global buffer object data for camera
	glBindBuffer(GL_UNIFORM_BUFFER, camGlobalUniformBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 4, &camPosX);
	glBufferSubData(GL_UNIFORM_BUFFER, 4, 4, &camPosY);
	glBufferSubData(GL_UNIFORM_BUFFER, 8, 4, &viewSize);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if (keyState->key_1->pressed) {
		activeBrushPixel = PIX_SAND;
		printf("Selected Sand\n");
	}
	if (keyState->key_2->pressed) {
		activeBrushPixel = PIX_WATER;
		printf("Selected Water\n");
	}
	if (mouseState->left->down) {
		float mouseWorldX = mouseState->posX * viewSize - camPosX + 0.5f;
		float mouseWorldY = mouseState->posY * viewSize - camPosY + 0.5f;
		
		int brushRadius = 3;
		float brushClusterOffset = 0.05f;
		for (int x = -brushRadius; x <= brushRadius; x++) {
			for (int y = -brushRadius; y <= brushRadius; y++) {
				if (x * x + y * y > brushRadius * brushRadius) continue;

				setPixel(activeBrushPixel, 
					mouseWorldX + x * brushClusterOffset, 
					mouseWorldY + y * brushClusterOffset);
			}
		}
	}
	if (keyState->key_0->pressed) {
		int activeCt = 0;
		for (int i = 0; i < world->chunks->count; i++) {
			if (!((Chunk)List_get(world->chunks, i))->sleeping) activeCt++;
		}
		printf("==Chunk Info==\n Active: %d\n Total: %d\n", activeCt, world->chunks->count);
	}
}

void fixedUpdate() {
	World_update(world);
}

void updateChunkTextures() {
	/*
	 * TODO: optimize to identify chunks that are on camera and then only update textures for those.
	 */
	for (int i = 0; i < world->chunks->count; i++) {
		Chunk c = List_get(world->chunks, i);
		glBindTexture(GL_TEXTURE_2D, c->texture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 64, 64, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, c->data);
	}
}

void draw() {
	//clear background
	//glClearColor(0.054f, 0.02f, 0.18f, 1.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//do rendering
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(quadVAO);

	for (int i = 0; i < world->chunks->count; i++) {
		Chunk c = List_get(world->chunks, i);
		Shader_setVec2(chunkShader, "pos", (float)c->posX, (float)c->posY); 
		//this fails occasionally, it seems, causing a chunk to flicker in the wrong location
		Shader_use(chunkShader);
		glBindTexture(GL_TEXTURE_2D, c->texture);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	if (width >= height) glViewport(0, (height - width) / 2, width, width);
	else glViewport((width - height) / 2, 0, height, height);
}

void setPixel(int pixMaterial, float worldX, float worldY) {
	int chunkX, chunkY;
	chunkX = (int)floor(worldX);
	chunkY = (int)floor(worldY);
	int px = (int)(64.0f * (worldX - (float)chunkX));
	int py = (int)(64.0f * (worldY - (float)chunkY));
	if (chunkX < world->chunkBorderLeft || chunkX > world->chunkBorderRight
		|| chunkY < world->chunkBorderBottom || chunkY > world->chunkBorderTop) return;
	Chunk chunk = locateChunk(world, chunkX, chunkY);
	if (chunk == NULL || chunkX != chunk->posX || chunkY != chunk->posY) {
		return;
	}

	chunk->data[px][py][0] = (char)pixMaterial;
	chunk->sleeping = false;
	//updateChunkTextures();
}