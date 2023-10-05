#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"


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

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwTerminate();
	return 0;
}