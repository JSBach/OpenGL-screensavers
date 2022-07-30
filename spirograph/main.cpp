#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <exception>
#include "spirograph.h"

const int MAIN_WINDOW_WIDTH = 800;
const int MAIN_WINDOW_HEIGHT = 600;
const double FRAME_TIME_INTERVAL = 0.02;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 outColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
"   outColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 outColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(outColor, 1.0f);\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

int main()
{
	float* vertices; // [] = { -0.5f, -0.5f };
	float color[3] = { 0.0f, 0.5f, 0.0f };
	double prev_time, current_time;
	Spirograph spirograph((float)MAIN_WINDOW_HEIGHT / (float)MAIN_WINDOW_WIDTH);
	spirograph.SetColorMode(12);

	glfwInit();

	//creating a window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, "Spirograph", NULL, NULL);
	
	if (window == NULL)
	{
		glfwTerminate();
		throw new std::exception("Cannot create the main window");
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw new std::exception("Failed to initialize GLAD");
	}

	//setting the buffers
	unsigned int VBO, VAO;
	
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//compiling the shaders
	int success;
	char infoLog[512];
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		throw new std::exception(infoLog);
	}


	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		throw new std::exception(infoLog);
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		throw new std::exception(infoLog);
	}

	current_time = prev_time = glfwGetTime();
	spirograph.Start();

	while (!glfwWindowShouldClose(window))
	{
		process_input(window);

		current_time = glfwGetTime();
		if(current_time - prev_time >= FRAME_TIME_INTERVAL)
		{
			spirograph.CalculateFrame();

			unsigned int vertex_count = 0;
			vertices = spirograph.GetVertices(&vertex_count);
			prev_time = current_time;

			//rendering
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shaderProgram);

			//TODO: static draw vs dynamic draw ?
			glBufferData(GL_ARRAY_BUFFER, vertex_count * Spirograph::VERTEX_SIZE * sizeof(float), vertices, GL_DYNAMIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, Spirograph::VERTEX_SIZE * sizeof(float), (void*)0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Spirograph::VERTEX_SIZE * sizeof(float), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glDrawArrays(GL_POINTS, 0, vertex_count);

			//presenting
			glfwSwapBuffers(window);
		}
		
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}