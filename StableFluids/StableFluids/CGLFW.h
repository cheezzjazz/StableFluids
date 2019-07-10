#include <GLFW\glfw3.h>
#include <iostream>

class GLFW
{
public:
	GLFWwindow* window = nullptr;

	int init();
	void swapBuffers();
	bool getKeyPressed(const int& key);
	int checkWindowcreated();
	bool getWindowShouldClose();
	void terminate();

private:
	unsigned int scr_width = 800;
	unsigned int scr_height = 600;
	const char* Window_Title = "Stable Fluids";
};