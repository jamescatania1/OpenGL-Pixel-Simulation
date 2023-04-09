#ifndef _shader_h
#define _shader_h

#include <glad/glad.h>

typedef struct Shader {
	int id;
}
*Shader;

//Shader constructor. Use relative paths to vertex/fragment shaders.
extern Shader new_Shader(const char* vertexPath, const char* fragmentPath);

extern void Shader_free(Shader shader);

//Use the shader.
extern void Shader_use(Shader shader);

//Set boolean uniform.
extern void Shader_setBool(Shader shader, const char* name, int value);

//Set integer uniform.
extern void Shader_setInt(Shader shader, const char* name, int value);

//Set float uniform.
extern void Shader_setFloat(Shader shader, const char* name, float value);

//Set vec2 uniform.
void Shader_setVec2(Shader shader, const char* name, float x, float y);

#endif