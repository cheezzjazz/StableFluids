#include <glad/glad.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include "CShader.h"
#include "CGLFW.h"
#include "CStableFluidsFunc.h"

const float SAMPLE_SIZE = 0.02f;
GLFW ourGlfw;

int vertexCount;
float* srcVertices;

void updateVertices(float* mappedbuffer, float* srcvertices, int count)
{
	if (!mappedbuffer || !srcvertices)
	{
		std::cout << "ERROR::UPDATEVERTICES:: DATA Pointer is NULL" << std::endl;
		return;
	}

	for (int i = 0; i < count; i++)
	{
		////get source from original vertex array
		//float x, y, z;
		//x = *srcvertices; std::cout << "prev x:" << *srcvertices;
		//++srcvertices;
		//y = *srcvertices; std::cout << "prev y:" << *srcvertices;
		//++srcvertices;
		//z = *srcvertices; std::cout << "prev z:" << *srcvertices << std::endl;
		//++srcvertices;

		//float diff = 0.1f;
		////update vertex coords
		//*mappedbuffer = x + diff; std::cout << "x:" << *mappedbuffer;
		//++mappedbuffer;
		//*mappedbuffer = y + diff; std::cout << "y:" << *mappedbuffer;
		//++mappedbuffer;
		//*mappedbuffer = z; std::cout << "z:" << *mappedbuffer << std::endl;
		//++mappedbuffer;

		float diff = 0.008f;
		//update vertex coords
		*srcvertices += diff;
		*mappedbuffer = *srcvertices; std::cout << "x:" << *mappedbuffer;
		++srcvertices; ++mappedbuffer;
		*srcvertices += diff;
		*mappedbuffer = *srcvertices + diff; std::cout << "y:" << *mappedbuffer;
		++srcvertices; ++mappedbuffer;
		*srcvertices += diff;
		*mappedbuffer = *srcvertices; std::cout << "z:" << *mappedbuffer << std::endl;
		++srcvertices; ++mappedbuffer;
	}
	
	
}

void loadGLADLoader()
{
	if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return ;
	}
}

int main()
{
	//Initialize GLFW
	ourGlfw.init();

	//Load GLAD loader
	loadGLADLoader();

	//Create Shader
	//Shader ourShader("./Shader/VertexShader_uniform.glsl", "./Shader/FragmentShader_uniform.glsl", "./Shader/GeometryShader_uniform.glsl");
	Shader ourShader("./Shader/VertexShader.glsl", "./Shader/FragmentShader.glsl");
	StableFluidsFunc ourFunc;

	ourFunc.sourcing();
	float vertices[] =
	{
		0.0f, 0.5f, 0.0f, // top mid 0
		0.5f, -0.5f, 0.0f, // bottom right 1
		-0.5f, -0.5f, 0.0f// // bottom left 2
	};
	//vertexCount = sizeof(vertices) / (3 * sizeof(float));
	vertexCount = 2 * 4 * sizeof(ourFunc.dens) / (sizeof(float));
	srcVertices = new float[vertexCount];

	float h = (float)(1.0f / (N + 2));

	/*for (int i = 0; i < vertexCount; i++)
	{
		srcVertices[2 * i] = i - h/2.0f 
	}
*/
	for (int j = 0; j < N + 2; j++)
	{
		for (int i = 0; i < N + 2; i++)
		{
			srcVertices[2 * 4 * IX(i, j) + 0] = i - h / 2.0f;
			//std::cout << "s[" << 2 * 4 * IX(i, j) + 0 << "]" ;
			srcVertices[2 * 4 * IX(i, j) + 2] = i + h / 2.0f;
			//std::cout << "s[" << 2 * 4 * IX(i, j) + 2 << "]";
			srcVertices[2 * 4 * IX(i, j) + 4] = i + h / 2.0f;
			//std::cout << "s[" << 2 * 4 * IX(i, j) + 4 << "]" ;
			srcVertices[2 * 4 * IX(i, j) + 6] = i - h / 2.0f;
			//std::cout << "s[" << 2 * 4 * IX(i, j) + 6 << "]" ;

			srcVertices[2 * 4 * IX(i, j) + 1] = j + h / 2.0f;
			//std::cout << "s[" << 2 * 4 * IX(i, j) + 1 << "]" ;
			srcVertices[2 * 4 * IX(i, j) + 3] = j + h / 2.0f;
			//std::cout << "s[" << 2 * 4 * IX(i, j) + 3 << "]";
			srcVertices[2 * 4 * IX(i, j) + 5] = j - h / 2.0f;
			//std::cout << "s[" << 2 * 4 * IX(i, j) + 5 << "]" ;
			srcVertices[2 * 4 * IX(i, j) + 7] = j - h / 2.0f;
			//std::cout << "s[" << 2 * 4 * IX(i, j) + 7 << "]" << std::endl;

		}
		//srcVertices[i] = vertices[i];
		//std::cout << "src[" << i << "]" << srcVertices[i] << std::endl;
	}

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexCount, srcVertices, GL_STREAM_DRAW);
	std::cout << "size : " << sizeof(vertices) << std::endl;
	std::cout << "size : " << sizeof(float)*vertexCount << std::endl;

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
 

	while (!ourGlfw.getWindowShouldClose())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		ourShader.use();
		//ourShader.setFloat("radius", SAMPLE_SIZE);

		float *VBOptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
		if (VBOptr)
		{
			//updateVertices(VBOptr, srcVertices, vertexCount);
			ourFunc.update(VBOptr, srcVertices);
			//vertexCount = sizeof(srcVertices) / (2 * sizeof(float));
			//for (int i = 0; i < vertexCount; i++)
			//{
			//	std::cout << "v : " << srcVertices[i] << std::endl;
			//	//srcVertices++;
			//}
			glUnmapBuffer(GL_ARRAY_BUFFER);
			/*if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_TRUE)
				std::cout << "!" << std::endl;*/
		}

		//ourShader.setFloat("radius", SAMPLE_SIZE); //geometryshader
		//ourShader.setVec3("position", u, v, 0.0f);//use uniform
		
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		for (int i = 0; i < N + 2; i++)
		{
			for (int j = 0; j < N + 2; j++)
			{
				glDrawArrays(GL_POINTS, 0, 2);
			}
		}
		
		ourGlfw.swapBuffers();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		Sleep(100);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	delete[] srcVertices;
	ourGlfw.terminate();

	return 0;
}