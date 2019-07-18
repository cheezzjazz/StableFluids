#include <GLFW\glfw3.h>
#include <iostream>

class GLFW
{
public:
	GLFWwindow* window = nullptr;
	unsigned int scr_width = 600;
	unsigned int scr_height = 600;
	bool isStart = false;
	
	int init();
	void swapBuffers();
	bool getKeyPressed(const int& key);
	int checkWindowcreated();
	bool getWindowShouldClose();
	void terminate();
	void processInput();

private:
	const char* Window_Title = "Stable Fluids";
};