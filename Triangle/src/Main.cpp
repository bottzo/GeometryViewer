#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "GL/glew.h"


//TODO: Subsystem window
//linker properties entry point mainCRTStartup
//put the winMain function (We have to include windows :( (does glfw already include it??)
//#include <Windows.h>
 //int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
 //	PSTR lpCmdLine, int nCmdShow)
 //{
 // 
 //}

//opengl32.lib just needed for the 1.1 version functions
//https://stackoverflow.com/questions/49371475/how-does-opengl32-lib-work-on-windowsjust-version-1-1-does-it-actually-implem


void ResizeFramebufferCb(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

//https://learnopengl.com/Getting-started/Hello-Window
int main() {
	if (!glfwInit())
	{
		//TODO: posar els errors en logs

		return 0;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(640, 480,"GLFWwindow", NULL, NULL);
	const char** err = nullptr;
	int a = glfwGetError(err);
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, ResizeFramebufferCb);

	int w = 0;
	int h = 0;
	glfwGetFramebufferSize(window, &w, &h);
	//glfwGetFramebufferSize(window, 640, 480);
	glViewport(0, 0, w, h);
	glClearColor(0.f, 0.f, 0.5f, 1.f);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}