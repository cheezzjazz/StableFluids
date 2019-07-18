#include <glad/glad.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include "CShader.h"
#include "CGLFW.h"
#include "CStableFluidsFunc.h"

const float SAMPLE_SIZE = 0.02f;
GLFW ourGlfw;

void loadGLADLoader()
{
	if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return;
	}
}

int main()
{
	//Initialize GLFW
	ourGlfw.init();

	//Load GLAD loader
	loadGLADLoader();

	//Create Shader
	//Shader ourShader("./Shader/VertexShader.glsl", "./Shader/FragmentShader.glsl");
	Shader ourShader("./Shader/VertexShader_StableFluids.glsl", "./Shader/FragmentShader.glsl");
	StableFluidsFunc ourFunc;
	ourFunc.sourcing();

	float vertices[] =
	{
		-0.5f, 0.5f, // top mid 0
		0.0f, 0.5f, // bottom right 1
		0.0f, -0.5f,// // bottom left 2
		-0.5f, -0.5f,

		0.0f, 0.5f, // top mid 0
		0.5f, 0.5f, // bottom right 1
		0.5f, -0.5f,// // bottom left 2
		0.0f, -0.5f,
	};

	unsigned int indices[] = {	//note that we start from 0!!
		0, 1, 2,	// triangle
		2, 3, 0,
		4, 5, 6,
		6, 7, 4
	};

	float* elements = new float[SIZE * 6];
	float* gridVertex = new float[SIZE * 2];
	float* colorArray = new float[SIZE];

	float h = (float)(2.0f / (N + 2));
	//float cellheight = (float)(ourGlfw.scr_height / (N + 2));
	float cellwidth = (float)(ourGlfw.scr_width / (N + 2));

	for (int j = 0; j < N+2 ; j++)
	{
		for (int i = 0; i < N+2; i++)
		{
			gridVertex[2 * IX(i, j)] = -1.0f + (h/2.0f) + (float)(h* i);
			gridVertex[2 * IX(i, j) + 1] = 1.0f - (h / 2.0f) - (float)(h * j);
//			std::cout <<"g["<<2 * IX(i, j)<<"]="<<gridVertex[2 * IX(i, j)] << " , " << "g[" << 2 * IX(i, j) + 1 << "]=" << gridVertex[2 * IX(i, j) + 1] << std::endl;
		}
//		std::cout << "\n" << std::endl;
	}
	
	int nIndex = 0;
	int tmp = 0;
	for (int i = 0; i < SIZE; i++)
	{
		int first = 4 * i;
		tmp = first;
		for (int j = 0; j < 6; j++)
		{
			if (j == 0 || j == 5)
				elements[nIndex++] = first;
			else
			{
				elements[nIndex++] = tmp;
			}

			if (j != 2)
			{
				tmp++;
			}
			//nIndex++;
			//std::cout << i << " " << j << " " << nIndex << " " << elements[nIndex-1] << std::endl;
		}
		//Init colorArray 
		colorArray[i] = ourFunc.dens[i];
	}

	unsigned int VBO, VAO, EBO, VBO2;
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO2);	//color

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, 2.0f * SIZE * sizeof(float), gridVertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
/*
	std::cout << "size : " << sizeof(vertices) << std::endl;
	std::cout << "size : " << 2.0f*SIZE * sizeof(float) << std::endl;*/
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, SIZE * sizeof(float), colorArray, GL_STREAM_DRAW);
	//std::cout << "size : " << sizeof(colorArray) << std::endl;
	//std::cout << "size : " << SIZE * sizeof(float) << std::endl;

	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(VAO);
	int frame = 0;
	while (!ourGlfw.getWindowShouldClose())
	{
		//ourFunc.sourcing();
		std::cout<< "\n\n"<<frame++ <<std::endl;
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourGlfw.processInput();
		//ourShader.setFloat("radius", SAMPLE_SIZE);
		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		float *VBOptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
		if (VBOptr)
		{
			//ourFunc.update2(VBOptr, colorArray);
			ourFunc.sourcing();
			ourFunc.vel___();
			ourFunc.update(VBOptr, colorArray);
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}

		ourShader.use();
		
		std::cout << "color[138] =" << colorArray[137]<<std::endl;
		glPointSize(cellwidth);
		glDrawArrays(GL_POINTS, 0, SIZE);

		ourGlfw.swapBuffers();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	
		Sleep(100);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	//glDeleteBuffers(1, &EBO);

	delete[] gridVertex;
	delete[] elements;
	delete[] colorArray;

	ourGlfw.terminate();

	return 0;
}
