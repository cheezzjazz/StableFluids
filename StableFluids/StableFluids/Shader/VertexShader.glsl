#version 330 core
layout (location = 0) in vec3 aPos;

//out vec3 ourColor;
out VS_OUT{
	vec3 color;
} vs_out;

uniform vec3 position;

void main()
{
	//aPos = position;
	//gl_Position = vec4(aPos, 1.0f);
	gl_Position = vec4(position, 1.0f);
	vs_out.color = vec3(1.0f, 1.0f, 0.0f);
	//ourColor = vec3(1.0f, 1.0f, 0.0f);
}