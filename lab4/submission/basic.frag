#version 330 core

out vec4 ColorOut;

in vec3 Color;
in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    ColorOut = texture(texture1, TexCoord);
}