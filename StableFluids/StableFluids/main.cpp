#include <glad/glad.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include "CShader.h"
#include "CGLFW.h"
#include "CStableFluidsFunc.h"

GLFW ourGlfw;
float cellwidth = 1.0f;

void loadGLADLoader()
{
	if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return;
	}
}

void windowsize_func(GLFWwindow* window, int width, int height)
{
	glfwGetWindowSize(ourGlfw.window, &width, &height);
	float size = (float)((width >= height) ? width : height);
	cellwidth = (float)(size / (N + 2));
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

	float* gridVertex = new float[ourFunc.SIZE * 2];	//grid vertex
	float* colorArray = new float[ourFunc.SIZE];		//color

	float h = (float)(2.0f / (N + 2));	//cell size
	//float cellheight = (float)(ourGlfw.scr_height / (N + 2));
	cellwidth = (float)(ourGlfw.scr_width / (N + 2));
	std::cout << cellwidth << std::endl;
	for (int j = 0; j < N + 2; j++)
	{
		for (int i = 0; i < N + 2; i++)
		{
			gridVertex[2 * IX(i, j)] = -1.0f + (h / 2.0f) + (float)(h* i);
			gridVertex[2 * IX(i, j) + 1] = 1.0f - (h / 2.0f) - (float)(h * j);
			//			std::cout <<"g["<<2 * IX(i, j)<<"]="<<gridVertex[2 * IX(i, j)] << " , " << "g[" << 2 * IX(i, j) + 1 << "]=" << gridVertex[2 * IX(i, j) + 1] << std::endl;
		}
		//		std::cout << "\n" << std::endl;
	}

	//Init colorArray 
	for (int i = 0; i < ourFunc.SIZE; i++)
	{
		colorArray[i] = ourFunc.dens[i];
	}

	unsigned int VBO, VAO, VBO2;// EBO, ;
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);	//indices
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO2);	//color

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * ourFunc.SIZE * sizeof(float), gridVertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, ourFunc.SIZE * sizeof(float), colorArray, GL_STREAM_DRAW);

	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(VAO);
//	int frame = 0;
	while (!ourGlfw.getWindowShouldClose())
	{
//		std::cout << "\n\n" << frame++ << std::endl;
		ourGlfw.processInput();
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		float *VBOptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
		if (VBOptr)
		{
			//ourFunc.update2(VBOptr, colorArray);//test for drawing points with changing color
			if (ourGlfw.isStart)
			{
				ourFunc.sourcing();
				ourFunc.addvelocity();
				ourFunc.update(VBOptr, colorArray);
			}
				glUnmapBuffer(GL_ARRAY_BUFFER);
		}

		ourShader.use();

		glPointSize(cellwidth + 2.0f);
		glDrawArrays(GL_POINTS, 0, ourFunc.SIZE);

		ourGlfw.swapBuffers();
		//window resize callback
		glfwSetWindowSizeCallback(ourGlfw.window, windowsize_func);

		Sleep(100);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);

	delete[] gridVertex;
	delete[] colorArray;

	ourGlfw.terminate();

	return 0;
}
