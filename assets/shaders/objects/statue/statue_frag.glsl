#version 460 core

in vec3 Normal;

out vec4 FragColor;

void main() {
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;
    
    vec3 objectColor = vec3(0.5, 0.5, 0.5); // Solid gray color
    
    vec3 result = ambient * objectColor;
    FragColor = vec4(result, 1.0);
}