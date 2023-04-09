#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(std140) uniform GlobalInfo {
                    //base alignment //aligned offset
    float camPosX;    //4              //0
    float camPosY;
    float viewSize;
};


out vec3 ourColor;
out vec2 TexCoord;


uniform vec2 pos = { 0.0, 0.0 };

void main() {
    gl_Position = vec4((aPos.x + pos.x + camPosX) / viewSize, (aPos.y + pos.y + camPosY) / viewSize, 0.0, 1.0);
    //gl_Position = vec4(aPos.x + camPosX, aPos.y + camPosY, 0.0, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}