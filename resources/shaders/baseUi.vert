#version 330 core

layout (location = 0) in vec2 aPos;      // Vertex position (x, y)
layout (location = 1) in vec2 aTexCoord; // Texture coordinates (u, v)

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);  // z=0 for 2D UI, w=1
    TexCoord = aTexCoord;
}