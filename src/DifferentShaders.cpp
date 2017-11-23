/*
* Description: A simple test of working with multiple
*		shaders. This code will be more straight to the
*		point as compared too my other files in this repo.
*/

// Including core libraries
#include <iostream>
#include <cstdlib>

// Including openGL dependencies
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const int WIDTH = 800,
	HEIGHT = 600;

const char *WINDOW_NAME = "Different Shaders Test";

void windowResized(GLFWwindow*, int, int);
// Called everytime the window is resized

void generateVAO(unsigned int*, unsigned int*);
// Generates the VAO to draw the two triangles

bool generateShaderProg(unsigned int*, unsigned int*);
// Generates out two shader programs. Returns true
// if the process was a success

int main() {

	if (glfwInit() == GLFW_FALSE) {
		std::cout << "There was an error starting GLFW!\n";

		char dud;
		std::cin >> dud;

		return -1;
	}

	// Setting the OpenGL version to 3.3 (CORE)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// For compatibility with Macs
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create window object
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_NAME, NULL, NULL);

	// Did it work?
	if (window == NULL) {
		std::cout << "Unable to create GLFW context!\n";
		glfwTerminate();

		char dud;
		std::cin >> dud;

		return -1;
	}

	// Set window as current context
	glfwMakeContextCurrent(window);

	// Load up glad to make, so it can manage our OS-Specific function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		// GLAD was unable to start :(
		std::cout << "Unable to intialize GLAD with proc address!\nQuitting\n";
		glfwTerminate();
		return -1;
	}

	// Set out viewport and configure callback function for resizing
	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, windowResized);

	// Attempt to generate shaders
	unsigned int shaderProg1, shaderProg2;
	if (!generateShaderProg(&shaderProg1, &shaderProg2)) {

		std::cout << "There was an error generating the shaders!\n";

		glfwTerminate();

		char dud;
		std::cin >> dud;

		return -1;

	}

	// Get the VAO
	unsigned int VAO1, VAO2;
	generateVAO(&VAO1, &VAO2);

	// Simple render loop
	while (!glfwWindowShouldClose(window)) {

		// Clear screen
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Set the shader
		glUseProgram(shaderProg1);
		glBindVertexArray(VAO1);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glUseProgram(shaderProg2);
		glBindVertexArray(VAO2);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		// Swap buffers
		glfwSwapBuffers(window);

		// Handle various events
		glfwPollEvents();

	}

	glfwTerminate();

	return 0;

}

void windowResized(GLFWwindow *window, int width, int height) {

	glViewport(0, 0, width, height);

}

// Will return true if the shaders could be created
bool generateShaderProg(unsigned int *SHADER_PROG1, unsigned int *SHADER_PROG2) {

	// Only need 1 vertex shader, but two fragment shaders for different colors!
	const char *vShader =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main() {\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}";

	const char *fShader1 =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main() {"
		"	FragColor = vec4(0.5, 0.5, 0.1, 1.0);\n"
		"}";

	const char *fShader2 =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main() {"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}";

	// Create shader objects
	unsigned int vShaderID, fShaderID1, fShaderID2;

	vShaderID = glCreateShader(GL_VERTEX_SHADER);
	fShaderID1 = glCreateShader(GL_FRAGMENT_SHADER);
	fShaderID2 = glCreateShader(GL_FRAGMENT_SHADER);

	// Set shader source
	glShaderSource(vShaderID, 1, &vShader, NULL);
	glShaderSource(fShaderID1, 1, &fShader1, NULL);
	glShaderSource(fShaderID2, 1, &fShader2, NULL);

	// Compile
	glCompileShader(vShaderID);
	glCompileShader(fShaderID1);
	glCompileShader(fShaderID2);

	// Check for compilation errors
	int success;
	char infoLog[512];

	glGetProgramiv(vShaderID, GL_COMPILE_STATUS, &success);

	if (!success) {

		glGetShaderInfoLog(vShaderID, 512, NULL, infoLog);

		std::cout << "There was an error compiling the Vertex Shader!\n"
			<< infoLog;

		return false;
	}

	glGetProgramiv(fShaderID1, GL_COMPILE_STATUS, &success);

	if (!success) {

		glGetShaderInfoLog(fShaderID1, 512, NULL, infoLog);

		std::cout << "There was an error compiling the Fragment Shaders!\n"
			<< infoLog;

		return false;

	}

	glGetProgramiv(fShaderID2, GL_COMPILE_STATUS, &success);

	if (!success) {

		glGetShaderInfoLog(fShaderID2, 512, NULL, infoLog);

		std::cout << "There was an error compiling the Fragment Shaders!\n"
			<< infoLog;

		return false;

	}

	// Start the creation of the two shader programs
	*SHADER_PROG1 = glCreateProgram();
	*SHADER_PROG2 = glCreateProgram();

	// Attatch the necessary shaders, and link
	glAttachShader(*SHADER_PROG1, vShaderID);
	glAttachShader(*SHADER_PROG1, fShaderID1);

	glAttachShader(*SHADER_PROG2, vShaderID);
	glAttachShader(*SHADER_PROG2, fShaderID2);

	glLinkProgram(*SHADER_PROG1);
	glLinkProgram(*SHADER_PROG2);

	// Make sure programs could be linked
	glGetProgramiv(*SHADER_PROG1, GL_LINK_STATUS, &success);

	if (!success) {

		glGetProgramInfoLog(*SHADER_PROG1, 512, NULL, infoLog);

		std::cout << "There was an error linking the shaders!\n"
			<< infoLog;

		return false;
	}

	glGetProgramiv(*SHADER_PROG2, GL_LINK_STATUS, &success);
	
	if (!success) {

		glGetProgramInfoLog(*SHADER_PROG2, 512, NULL, infoLog);

		std::cout << "There was an error linking the shaders!\n"
			<< infoLog;

		return false;
	}

	// Delete the shaders
	glDeleteShader(vShaderID);
	glDeleteShader(fShaderID1);
	glDeleteShader(fShaderID2);

	return true;

}

// Generates the VAOs for our two triangles
void generateVAO(unsigned int *VAO1, unsigned int *VAO2) {
	
	// Array of vertices
	float vertices[] = {
		-0.5f, 0.5f, 0.0f, // Top LEFT
		-1.0f, -0.5f, 0.0f, // Left LEFT
		0.0f, -0.5f, 0.0f, // Right LEFT Left RIGHT
		0.5f, 0.5f, 0.0f, // Top RIGHT
		1.0f, -0.5f, 0.0f // Right RIGHT
	};

	// Array of indices
	unsigned int indices1[] = {
		// First triangle
		0, 1, 2,
	};

	unsigned int indices2[] = {
		// Second triangle
		2, 3, 4
	};

	// Generate the VAO ID
	glGenVertexArrays(1, VAO1);

	// -- VAO 1 --

	// Generate the VBO ID and configure to VBO
	unsigned int VBO, VBO2;
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(*VAO1);

	// Copy verticies data over to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy our vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Now generate and configure the EBO
	unsigned int EBO, EBO2;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// Copy indices data over to the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

	// Configure how to use shaders with buffers and enable
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind VAO
	glBindVertexArray(0);

	// -- VAO 2 --

	// Generate the next VAO ID
	glGenVertexArrays(1, VAO2);

	// Bind it
	glBindVertexArray(*VAO2);

	// Generate another VBO to hold our next set of vertices
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);

	// Copy the data over
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Generate the EBO
	glGenBuffers(1, &EBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);

	// Copy the indices data over
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

	// Configure shaders for this VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	// Unbind VAO
	glBindVertexArray(0);

}