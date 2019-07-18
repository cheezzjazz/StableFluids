#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in float aColor;

out vec3 ourColor;

void main()
{
	gl_Position = vec4(aPos, 0.0f, 1.0f);
	ourColor = vec3(aColor, aColor, aColor);
}