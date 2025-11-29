#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform int hasTexture;   // 0 = no texture, 1 = use texture

void main()
{
    vec4 color = vec4(1.0f, 0.2f, 0.6f, 1.0f); 
    if (hasTexture == 1) {
        color = texture(texture1, TexCoords);
    }
    
    FragColor = color;
}
