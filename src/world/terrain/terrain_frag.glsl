#version 460 core
in vec3 fragPosition;
in vec2 tex_coords;

uniform sampler2D texture0;

out vec4 FragColor;

void main()
{
    float normalizedHeight = fragPosition.z / 15;
    vec3 lowColor = vec3(0.0, 0.0, 0.0);
    vec3 highColor = vec3(1.0, 1.0, 1.0);
    vec4 col = vec4(mix(lowColor, highColor, normalizedHeight), 1.0);

    //get the color from the texture
    vec4 texColor = texture(texture0, tex_coords);

    if (normalizedHeight < 0.005)
    {
        discard;
    }
    
    //mix the colors
    FragColor = mix(texColor, col, 0.5);

}