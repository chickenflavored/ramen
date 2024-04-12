#version 460 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 a_tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 fragPosition;
out vec2 tex_coords;

void main()
{
    fragPosition = inPosition;
    gl_Position = projection * view * model * vec4(inPosition, 1.0);
    tex_coords.x = a_tex_coords.x * 4;
    tex_coords.y = a_tex_coords.y * 4;
}