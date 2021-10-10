#pragma warning(disable : 28251)
#pragma warning(disable : 28159)

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Application.h"
#include "Sample.h"

GLFWwindow* window;
std::string g_app_title = "gameanimation_gltf";

int g_width = 1024;
int g_height = 512;

std::string rootData = "";

static bool findFullPath(const std::string& root, std::string& filePath)
{
	bool fileFound = false;
	const std::string resourcePath = root;

	filePath = resourcePath + filePath;
	for (unsigned int i = 0; i < 16; ++i)
	{
		std::ifstream file;
		file.open(filePath.c_str());
		if (file.is_open())
		{
			fileFound = true;
			break;
		}

		filePath = "../" + filePath;
	}

	return fileFound;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	g_width = width;
	g_height = height;
}


Application* gApplication = 0;
GLuint gVertexArrayObject = 0;

int main(void)
{
	{
		std::string locStr = "resources.loc";
		size_t len = locStr.size();

		bool fileFound = findFullPath("./", locStr);
		rootData = locStr.substr(0, locStr.size() - len);
	}

	// initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "glfwInit failed.\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// open a window and create its OpenGL context
	window = glfwCreateWindow(g_width, g_height, g_app_title.c_str(), NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "glfwCreateWindow failed.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// set glfw callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "glewInit failed.\n");
		glfwTerminate();
		return -1;
	}

	// Output some info on the OpenGL implementation
	const GLubyte* glvendor = glGetString(GL_VENDOR);
	const GLubyte* glrenderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);

	printf("GL_VENDOR: %s\n", glvendor);
	printf("GL_RENDERER: %s\n", glrenderer);
	printf("GL_VERSION: %s\n", glversion);

	// set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, g_width / 2, g_height / 2);

	glGenVertexArrays(1, &gVertexArrayObject);
	glBindVertexArray(gVertexArrayObject);

	gApplication = new Sample();
	gApplication->Initialize(rootData);

	double lastTick = glfwGetTime();
	while (glfwWindowShouldClose(window) == 0)
	{
		double thisTick = glfwGetTime();
		double deltaTime = float(thisTick - lastTick);
		lastTick = thisTick;
		if (gApplication != 0) {
			gApplication->Update((float)deltaTime);
		}
		if (gApplication != 0) {
			glViewport(0, 0, g_width, g_height);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glPointSize(5.0f);
			glBindVertexArray(gVertexArrayObject);

			glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			float aspect = (float)g_width / (float)g_height;
			gApplication->Render(aspect);
		}
		if (gApplication != 0) {
			// Swap buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	} // End of game loop

	if (gApplication != 0) {
		gApplication->Shutdown();
		std::cout << "Expected application to be null on exit\n";
		delete gApplication;
		gApplication = 0;
	}

	// close ogl window and terminate glfw
	glfwDestroyWindow(window);
	// finalize and clean up glfw
	glfwTerminate();

	return 0;
}