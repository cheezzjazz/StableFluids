#include <glad/glad.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include "CShader.h"
#include "CGLFW.h"

const float SAMPLE_SIZE = 0.02f;
GLFW ourGlfw;

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
	Shader ourShader("./Shader/VertexShader.glsl", "./Shader/FragmentShader.glsl", "./Shader/GeometryShader.glsl");

	float vertices[] =
	{
		0.0f, 0.5f, 0.0f, // top mid 0
		0.5f, -0.5f, 0.0f, // bottom right 1
		-0.5f, -0.5f, 0.0f// // bottom left 2
	};

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(VAO);

	while (!ourGlfw.getWindowShouldClose())
	{
		glClearColor(0.5f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use();

		float timeValue = glfwGetTime();
		float u = 0.0f;
		float v = 0.0f;
		//float diff = (sin(timeValue) / 2.0f) + 0.1f;
		//u = 0.0f + diff;
		//v = 0.0f + diff;

		ourShader.setFloat("radius", SAMPLE_SIZE);
		ourShader.setVec3("position", u, v, 0.0f);
		//ourShader.

		glDrawArrays(GL_POINTS, 0, 3);

		ourGlfw.swapBuffers();

		Sleep(100);
	}

	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	ourGlfw.terminate();
	return 0;
}