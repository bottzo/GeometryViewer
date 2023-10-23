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


	int shaderErr = 0;
	char log[500];
	unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vShaderSource = "#version 460 core\nlayout(location = 0) in vec3 pos;\nlayout(location = 1) in vec3 inCol;\nout vec3 col;\nvoid main(){\ncol=inCol;\ngl_Position = vec4(pos, 1.0f);\n}";
	glShaderSource(vShader, 1, &vShaderSource, NULL);
	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &shaderErr);
	if (shaderErr == GL_FALSE)
	{
		glGetShaderInfoLog(vShader, 500*sizeof(char), NULL, log);
		glfwTerminate();
		return 0;
	}
	unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fShaderSource = "#version 460 core\nin vec3 col; out vec4 fragColor;\nvoid main(){\nfragColor = vec4(col, 1.0f);\n}";
	glShaderSource(fShader, 1, &fShaderSource, NULL);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &shaderErr);
	if (shaderErr == GL_FALSE)
	{
		glGetShaderInfoLog(fShader, 500 * sizeof(char), NULL, log);
		glfwTerminate();
		return 0;
	}
	unsigned int programId = glCreateProgram();
	glAttachShader(programId, vShader);
	glAttachShader(programId, fShader);
	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &shaderErr);
	if (shaderErr == GL_FALSE)
	{
		glGetProgramInfoLog(programId, 500*sizeof(char), NULL, log);
		glfwTerminate();
		return 0;
	}

	float vertex[] = {
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(1);

	glViewport(0, 0, w, h);
	glClearColor(0.f, 0.f, 0.5f, 1.f);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glUseProgram(programId);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}