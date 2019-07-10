#version 330 core
out vec4 FragColor;

in vec3 fourColor;
//in vec3 ourColor;

void main()
{
    FragColor = vec4(fourColor, 1.0f);
	//FragColor = vec4(ourColor, 1.0f);
}