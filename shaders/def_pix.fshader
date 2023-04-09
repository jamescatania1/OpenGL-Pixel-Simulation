#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    int pix = int(256.0 * texture(ourTexture, TexCoord).r);
    if(pix == 0) { //VOID
        FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
    else if(pix == 1) { //SAND
        FragColor = vec4(0.820, 0.729, 0.517, 1.0);
    }
    else if(pix == 2) { //WATER
        FragColor = vec4(0.148, 0.292, 0.870, 1.0);
    }
    else FragColor = vec4(1.0, 0.0, .831, 1.0); //UNDEFINED PIXEL (pink)
}