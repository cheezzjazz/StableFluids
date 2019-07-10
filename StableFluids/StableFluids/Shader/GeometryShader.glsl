#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

//in vec3 ourColor;
in VS_OUT{
	vec3 color;
}gs_in[];

out vec3 fourColor;

uniform float radius;		//for setting circle radius

int Max = 24;
float PI = 3.1415926538979323846264338;
void Circle(vec4 pos)
{
	float r = radius;
	float theta = 2 * PI / Max;

	gl_Position = pos + vec4(r, 0.0, 0.0, 0.0);
	EmitVertex();
	for(int i = 0; i < (Max-2)/2; i++)
	{
		gl_Position = pos + vec4(r*cos(theta), r*sin(theta), 0.0, 0.0);
		EmitVertex();
		gl_Position = pos + vec4(r*cos(theta), -r*sin(theta), 0.0, 0.0);
		EmitVertex();
		theta += PI/12.0;
	}
	gl_Position = pos + vec4(-r, 0.0, 0.0, 0.0);
	EmitVertex();
}

void main()
{
	//fourColor = ourColor;
	fourColor = gs_in[0].color;
	Circle(gl_in[0].gl_Position);
}