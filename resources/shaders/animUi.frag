#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D spriteTexture;

void main()
{
    vec4 texColor = texture(spriteTexture, TexCoord);

    // Discard transparent pixels if needed
    if (texColor.a < 0.1)
        discard;

    FragColor = texColor;
}