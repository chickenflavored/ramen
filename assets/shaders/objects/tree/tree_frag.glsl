#version 460 core

in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    vec3 objectColor = texture(texture1, TexCoord).rgb;
    vec3 result = ambient * objectColor;
    FragColor = vec4(result, 1.0);
}