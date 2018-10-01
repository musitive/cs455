#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 ColorIn;

uniform mat4 Trans;

out vec3 Color;

void main()
{
    gl_Position = Trans * vec4(Position, 1.0);
    Color = ColorIn;
}
