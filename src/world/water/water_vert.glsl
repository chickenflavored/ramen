#version 460 core
layout (location = 0) in vec3 inPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 fragPosition;

void main()
{
    fragPosition = inPosition;
    gl_Position = projection * view * model * vec4(inPosition, 1.0);
}