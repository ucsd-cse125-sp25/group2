#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;    // World position for lighting calculations
out vec3 Normal;     // World normal for lighting calculations  
out vec2 TexCoords;  // Texture coordinates

uniform mat4 model;
uniform mat4 viewProj;

void main()
{
    // Transform vertex position to world space
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Transform normal to world space (using normal matrix)
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    TexCoords = aTexCoords;
    
    gl_Position = viewProj * vec4(FragPos, 1.0);
}