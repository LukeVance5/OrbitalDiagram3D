#version 330 core
flat in vec4 gColor;
out vec4 FragColor;

void main()
{
	FragColor = gColor;
}