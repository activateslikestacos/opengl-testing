/*
* Description: This file is for showing how to work with
*		EBO's (Element Buffer Objects) which is a unique way
*		of pairing an array of indices with an array of vertices.
*		To avoid duplicate points in a vertex array, we can just
*		specify it once, and tell OpenGL what order to draw it in.
*
*	Press LEFT and RIGHT arrow keys to switch between wireframe
*	or filled to see how the rectangle is drawn
*/

// Including core libraries
#include <iostream>
#include <cstdlib>

// Including openGL dependencies
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Window options
const char *WINDOW_NAME = "Element Buffer Object Rectangle";

const int WIDTH = 800,
HEIGHT = 600;

// Shaders written in GLSL
const char *vertexShader =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}";

const char *fragmentShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}";

void framebuffer_size_callback(GLFWwindow*, int, int);
// Called by GLFW automatically when the window is resized

int startRenderLoop(GLFWwindow*);
// The function to start the render loop. Should only be
// called after OpenGL, GLFW, and GLAD are setup
// Returns the status that the program should exit with

void handleInput(GLFWwindow*);
// Handles basic user input (call in render loop)

void draw(GLFWwindow*, unsigned int, unsigned int);
// Clears the screen etc.

unsigned int generateVAO();
// Generates the VAO that should be used to draw
// the rectangle

bool generateShaderPg(unsigned int*);
// Generates the shader program

int main() {

	// Intialize GLFW
	if (glfwInit() == GL_FALSE) {
		std::cout << "An error has occurred while trying to start GLFW!\nQuitting..\n";
		return -1;
	}

	// Setting the OpenGL version to 3.3 (CORE)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// For compatibility with Macs
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create our window object!
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_NAME, NULL, NULL);

	if (window == NULL) {
		// The window couldn't be created
		std::cout << "GLFW failed to created the window context!\n";
		glfwTerminate();
		return -1;
	}

	// If window is created, set it to our current context
	glfwMakeContextCurrent(window);

	// Load up glad to make, so it can manage our OS-Specific function pointers
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		// GLAD was unable to start :(
		std::cout << "Unable to intialize GLAD with proc address!\nQuitting\n";
		glfwTerminate();
		return -1;
	}

	// Tell OpenGL the size and starting point of our window
	glViewport(0, 0, WIDTH, HEIGHT);

	// Register the framebuffer resize callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	int progStatus = startRenderLoop(window);

	glfwTerminate();

	return progStatus;

}

// Called automatically on window resize
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {

	// Fix OpenGL's viewport
	glViewport(0, 0, width, height);

}

// The main loop of the program here.. Keeps it running
int startRenderLoop(GLFWwindow *window) {

	// Get shader program ID
	unsigned int shaderProgram;

	// Make sure the shader was successfully created
	if (!generateShaderPg(&shaderProgram)) {
		
		// If we get here, something bad happened, so notify the user
		std::cout << "\nThere was an error while generating the shaders!\n";
		
		// Hold open console so we can see any errors
		char dud;
		std::cin >> dud;

		// terminate everything
		glfwSetWindowShouldClose(window, GLFW_TRUE);

		return -1;

	}

	// Get our VAO ID
	unsigned int VAO = generateVAO();

	while (!glfwWindowShouldClose(window)) {

		// Check for any user input
		handleInput(window);

		// Draw
		draw(window, shaderProgram, VAO);

		// Display all that was on the back buffer
		glfwSwapBuffers(window);

		// Poll any events triggers stored in GLFW
		glfwPollEvents();

	}

	return 0;

}

// Basically going to detect an esc press
void handleInput(GLFWwindow *window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		// If an escape key press is detected, set the window to close
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

}

// Clears the screen color and draws the next frame
void draw(GLFWwindow *window, unsigned int shaderProg, unsigned int VAO) {

	// Clear the back buffer
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Now draw that sexy rectangle!
	glUseProgram(shaderProg);
	glBindVertexArray(VAO);

	// The draw function to be used with EBOs.
	// The first argument is what we are drawing
	// The second is how many indices we are drawing
	// The third is is what type the indices are
	// The fourth is the indices offset
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Unbind the vertex array
	glBindVertexArray(0);

}

// Generates the Vertex Array Object for the rectangle
unsigned int generateVAO() {

	// Vertices and indices for our rectangle
	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// Created VAO to store my vertex's
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// Genearte the buffer for the VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Bind are VAO, so it "memorizes" everything done below
	glBindVertexArray(VAO);

	// Configure the buffer as a VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy our vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Generate the buffer for the EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// Configure the buffer as an EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// Copy our indices data into the EBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Configure how openGL uses our vertex data to render
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

	// Now enable the vertex attribute at location 0 (specified in our shader)
	glEnableVertexAttribArray(0);

	// Are VAO is all setup, so return it's ID
	return VAO;

}

// Don't mind the pass by pointer, I'm just testing some things out
// pass by reference would have worked just fine
bool generateShaderPg(unsigned int *PROG_ID) {

	unsigned int vShaderID, fShaderID;

	// Create the shader objects
	vShaderID = glCreateShader(GL_VERTEX_SHADER);
	fShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Set the shader source code and compile them
	glShaderSource(vShaderID, 1, &vertexShader, NULL);
	glShaderSource(fShaderID, 1, &fragmentShader, NULL);

	glCompileShader(vShaderID);
	glCompileShader(fShaderID);

	// Check for errors
	int success;
	char log[512];

	glGetProgramiv(vShaderID, GL_COMPILE_STATUS, &success);
	
	if (!success) {

		// Get some info on why the shader failed
		glGetShaderInfoLog(vShaderID, 512, NULL, log);

		std::cout << "There was an error compiling the vertex shader source code!\n"
			<< log;

		// 'Error' out since the shader couldn't be compiled
		return false;
	}

	glGetProgramiv(fShaderID, GL_COMPILE_STATUS, &success);

	if (!success) {

		// Get some info on why this shader failed
		glGetShaderInfoLog(fShaderID, 512, NULL, log);

		std::cout << "There was an error compiling the fragment shader source code!\n"
			<< log;

		// 'Error' out since the shader couldn't be compiled
		return false;
	
	}

	// Create the shader program
	*PROG_ID = glCreateProgram();

	// Assign the above shaders to the program
	glAttachShader(*PROG_ID, vShaderID);
	glAttachShader(*PROG_ID, fShaderID);

	// Link the shaders together as one 'program'
	glLinkProgram(*PROG_ID);

	// Figure out if the linking was successful
	glGetProgramiv(*PROG_ID, GL_LINK_STATUS, &success);

	if (!success) {

		// Get some info on why the program failed
		glGetProgramInfoLog(*PROG_ID, 512, NULL, log);

		std::cout << "There was an error linking the shader program!\n"
			<< log;

		// Once again error out if something went wrong
		return false;

	}

	// Delete the shaders since they are part of a program now
	glDeleteShader(vShaderID);
	glDeleteShader(fShaderID);

	// If all goes well, then return true!
	return true;

}