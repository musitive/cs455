#version 330 core

uniform vec3 Mask;

out vec4 ColorOut;
in vec3 Color;

void main()
{
    ColorOut = vec4(Color * Mask, 1.0);
}
