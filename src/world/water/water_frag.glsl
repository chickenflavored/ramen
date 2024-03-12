#version 460 core
in vec3 fragPosition;


out vec4 FragColor;

void main()
{
    //set frag color to blue
    FragColor = vec4(0.039f, 0.153f, 0.278f, 1.0f);
}