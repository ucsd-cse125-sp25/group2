#version 330 core

layout (location = 0) in vec3 aPos;       // Quad vertex positions (NDC)
layout (location = 1) in vec2 aTexCoord;  // Quad UVs (0-1)

out vec2 TexCoord;

uniform vec2 frameOffset; // Current frame offset (0-1)
uniform vec2 frameSize;   // Frame size in atlas (0-1)

void main()
{
    gl_Position = vec4(aPos, 1.0); // Directly use NDC positions
    TexCoord = aTexCoord * frameSize + frameOffset;
}