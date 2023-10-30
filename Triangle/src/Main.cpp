#define GLFW_INCLUDE_NONE
#define _CRT_SECURE_NO_WARNINGS
#include "GLFW/glfw3.h"
#include "GL/glew.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


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

unsigned int programId = 0;
void ResizeFramebufferCb(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
	glUseProgram(programId);
	glUniform2i(glGetUniformLocation(programId, "resolution"), w, h);
}

typedef struct {
	unsigned int colorPeriod;
	unsigned int maxIterations;
} Uniforms;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Uniforms* ptr = (Uniforms*)glfwGetWindowUserPointer(window);
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		ptr->colorPeriod += 1;
		glUniform1ui(glGetUniformLocation(programId, "colorPeriod"), ptr->colorPeriod);
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		ptr->colorPeriod -= 1;
		glUniform1ui(glGetUniformLocation(programId, "colorPeriod"), ptr->colorPeriod);
	}
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		ptr->maxIterations += 1;
		glUniform1ui(glGetUniformLocation(programId, "maxIterations"), ptr->maxIterations);
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		if (ptr->maxIterations > 0)
			ptr->maxIterations -= 1;
		else
			ptr->maxIterations = 0;
		glUniform1ui(glGetUniformLocation(programId, "maxIterations"), ptr->maxIterations);
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		ptr->maxIterations += 100;
		glUniform1ui(glGetUniformLocation(programId, "maxIterations"), ptr->maxIterations);
	}
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		if (ptr->maxIterations > 0)
			ptr->maxIterations -= 100;
		else
			ptr->maxIterations = 0;
		glUniform1ui(glGetUniformLocation(programId, "maxIterations"), ptr->maxIterations);
	}
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
	//const char* vShaderSource = "#version 460\nvoid main(void){\n}";
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
	//const char* fShaderSource = "#version 460 core\nin vec3 col; out vec4 fragColor;\nvoid main(){\nfragColor = vec4(col, 1.0f);\n}";
	FILE* file = fopen("Shaders/Mandelbrot.glsl", "rb");
	if (file == NULL)
	{
		glfwTerminate();
		return 0;
	}
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	char* data = (char*)malloc(size + 1);
	fseek(file, 0, SEEK_SET);
	fread(data, 1, size, file);
	data[size] = '\0';
	fclose(file);

	//const char* fShaderSource = "#version 460 core\nin vec3 col; out vec4 fragColor;\nvoid main(){\nfragColor = vec4(col, 1.0f);\n}";
	glShaderSource(fShader, 1, &data, NULL);
	free(data);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &shaderErr);
	if (shaderErr == GL_FALSE)
	{
		glGetShaderInfoLog(fShader, 500 * sizeof(char), NULL, log);
		OutputDebugStringA(log);
		glfwTerminate();
		return 0;
	}
	//unsigned int programId = glCreateProgram();
	programId = glCreateProgram();
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
	glUseProgram(programId);
	double centerX = -0.55;
	double centerY = 0.0;
	double cLength = 2.0;
	Uniforms unis = { 24, 1000 };
	glUniform1ui(glGetUniformLocation(programId, "colorPeriod"), unis.colorPeriod);
	glUniform1ui(glGetUniformLocation(programId, "maxIterations"), unis.maxIterations);
	glUniform2i(glGetUniformLocation(programId, "resolution"), w, h);
	glUniform2d(glGetUniformLocation(programId, "center"), centerX, centerY);
	glUniform1d(glGetUniformLocation(programId, "cLength"), cLength);
	glfwSetWindowUserPointer(window, &unis);
	glfwSetKeyCallback(window, KeyCallback);

	float vertex[] = {
	-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned int indices[] = { 0,1,2,3,2,1 };
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			cLength *= 0.99;
			glUniform1d(glGetUniformLocation(programId, "cLength"), cLength);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			cLength *= 1.01;
			glUniform1d(glGetUniformLocation(programId, "cLength"), cLength);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			centerX += 0.01 * cLength;
			glUniform2d(glGetUniformLocation(programId, "center"), centerX, centerY);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			centerX -= 0.01 * cLength;
			glUniform2d(glGetUniformLocation(programId, "center"), centerX, centerY);
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			centerY += 0.01 * cLength;
			glUniform2d(glGetUniformLocation(programId, "center"), centerX, centerY);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			centerY -= 0.01 * cLength;
			glUniform2d(glGetUniformLocation(programId, "center"), centerX, centerY);
		}
		
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glUseProgram(programId);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}