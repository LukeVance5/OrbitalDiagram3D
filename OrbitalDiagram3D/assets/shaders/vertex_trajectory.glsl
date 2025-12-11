#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 worldPos;
    vec3 color;
} vs_out;

void main()
{
    vs_out.worldPos = vec3(model * vec4(aPos, 1.0));
    vs_out.color = aColor;

    // Don't output final gl_Position — GS will do it
    gl_Position = vec4(0.0); 
}