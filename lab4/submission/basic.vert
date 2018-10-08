#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 Color;
out vec2 TexCoord;

uniform mat4 Perspective;
uniform mat4 Trans;
uniform mat4 View;

void main()
{
    gl_Position = Perspective * View * Trans * vec4(aPosition, 1.0);
    Color = aColor;
    TexCoord = aTexCoord;
}