// Including built-in libraries
#include <cstdlib>
#include <string>
#include <iostream>

// Including other libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const int HEIGHT = 600,
	WIDTH = 800;

const char WINDOW_NAME[] = "Triangle Test";

// Function prototypes
void framebuffer_size_callback(GLFWwindow*, int, int);
// Callback function designed to be called everytime the window is resized
// for adjusting the viewport

void handleInput(GLFWwindow*);
// Simple function for handling user input. Will be called in our main loop

void renderItems(GLFWwindow*);
// Clears the screen with a certain color and draws everything needed

void drawTriangle(GLFWwindow*, unsigned int&, unsigned int&);
// Used to draw a basic triangle on the screen!

unsigned int generateShaders();
// Compiles and generates a shader program to be used in rendering
// basic objects

unsigned int generateTriangleVAO();
// Generates a Vertex Array Object for the triangle we want to draw!
// The VAO ID will be returned

int main() {

	// Basic startup of glfw

	glfwInit();
	
	// Setting the version of OpenGL to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Set to use core profile (just basic opengl features)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Needed to work on a Maciontosh
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Attempt to create the window object
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_NAME, NULL, NULL);

	// Make sure the window was created
	if (window == NULL) {

		// Window didn't open :(
		std::cout << "Unable to create GLFW context!\n";

		// Stop glfw (is it even working at this point anyways?!)
		glfwTerminate();

		// Return with error code
		return -1;

	}
	
	// If it gets to this point, then we can set the created context as our context
	glfwMakeContextCurrent(window);

	// Setting up glad to help us with OS specific objects
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {

		// GLAD was unable to settup, so abort
		std::cout << "Unable to initialize GLAD!\n";
		glfwTerminate();
		return -1;

	}

	// Set the viewport to the window size
	glViewport(0, 0, WIDTH, HEIGHT);

	// Register the frame buffer resize callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Generate our shader program ID
	unsigned int shaderProg = generateShaders();

	// Generate an ID for our VAO
	unsigned int VAO_ID = generateTriangleVAO();

	// Create a basic rendering loop
	while (!glfwWindowShouldClose(window)) {

		// Handle user input
		handleInput(window);

		// Render
		renderItems(window);

		// Draw the triangle!
		drawTriangle(window, shaderProg, VAO_ID);

		// Perform the double buffer goodness and display whatever is on the back buffer
		glfwSwapBuffers(window);
		
		// Basically the tick function for glfw
		glfwPollEvents();

	}

	// Make sure things are cleaned nicely!
	glfwTerminate();

	return 0;
}

// Called everytime the window is resized, so we can fix the viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);

}

// Handles basic user input
void handleInput(GLFWwindow *window) {

	// Some simple checks to see if the user pressed escape
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {

		// If escape is detected as pressed, then close the window gracefully
		glfwSetWindowShouldClose(window, true);

	}

}

// Handles basic window rendering
void renderItems(GLFWwindow *window) {

	// Set the clear color, then clear the screen
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

}

// Draws a nice triangle!
void drawTriangle(GLFWwindow *window, unsigned int &shaderID, unsigned int &VAO) {
	
	// Tell it to use our Shader program
	glUseProgram(shaderID);
	
	// Tell it to use our previously defined VAO
	glBindVertexArray(VAO);

	// Now.. With all that machine set in the proper state, tell OpenGL to draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

}

// Generates our basic shader program
unsigned int generateShaders() {

	// OpenGL has to compile our shaders, so we need to give it
	// something to compile! Below here will be our
	// Vertex and Fragment shader

	const char *vShader =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main() {\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}";

	const char *fShader =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}";

	// Hold the IDs of our shaders for later use
	unsigned int vShaderID, fShaderID;
	
	// Get the shaders ready for compilation
	vShaderID = glCreateShader(GL_VERTEX_SHADER);
	fShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Create a cstring to hold any log messages
	const int logSize = 512;
	char log[logSize];
	int success;
	bool error = false;

	// Attatch the source code to the shader
	// First argument is the shader we want to bind the source to
	// Second argument is how many strings are being passed
	// Third argument is the source code
	// Fourth is an int that tells us if the shader compiled successfully
	glShaderSource(vShaderID, 1, &vShader, NULL);
	glCompileShader(vShaderID);

	glShaderSource(fShaderID, 1, &fShader, NULL);
	glCompileShader(vShaderID);

	// Check to see if there were any compilation errors on the vertex shader
	glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &success);

	if (!success) {
		// If there was an error, generate and print out the log
		glGetShaderInfoLog(vShaderID, logSize, NULL, log);

		std::cout << "There was an error while compiling the Vertex Shader:\n"
			<< log;

		// An error occurred obviously.. :(
		error = true;

	}
	else {
		std::cout << "The Vertex Shader compiled successfully!\n";
	}

	// Next check for any errors on the Fragment Shader
	glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &success);

	if (!success) {
		// If there was an error here, print out the log
		glGetShaderInfoLog(fShaderID, logSize, NULL, log);

		std::cout << "There was an error while compiling the Fragment Shader:\n"
			<< log;

	}
	else {
		std::cout << "The Fragment shader compiled successfully!\n";
	}

	// If there was an error, close the program out
	if (error) {
		glfwTerminate();

		// Hold the terminal open, so we can see it
		char dud;
		std::cin >> dud;

		exit(-1);
	}

	// Begin creating the Shader Program using the pass-by-reference argument
	unsigned int progID = glCreateProgram();

	// Tell the linker what shaders we want to attatch to our shader program
	glAttachShader(progID, vShaderID);
	glAttachShader(progID, fShaderID);

	// Complete the linking process
	glLinkProgram(progID);

	// Cleanup the shader's unique objects since they are now in the program
	glDeleteShader(vShaderID);
	glDeleteShader(fShaderID);

	return progID;

}

// Generates a VAO, so we can easily set shaders on objects before they're drawn
unsigned int generateTriangleVAO() {

	// Generate our VBO ID for the triangle
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Start the process by generating an ID for our Vertex Array Object
	unsigned int VAO_ID;
	glGenVertexArrays(1, &VAO_ID);

	// Bind the VAO we generated above to all the configuration we are about to complete below
	glBindVertexArray(VAO_ID);

	// The 3 different vectors for the triangle
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	// Generate our VBO ID and configure it as an Array Buffer
	// (Since we are passing it an array of verticies)
	// We only want to generate 1 buffer (in this instance)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy our vertex data from the vertices array to our buffer's memory
	// The first argument is what type of buffer we are writing to
	// The second argument is the size in bytes of the data we are sending
	// The third argument IS the data
	// The last argument tells OpenGL how often the data will be changed
	// (We aren't changing it at all, so we are setting it to static)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set our Vertex Attribute Pointers, so OpenGL knows how we want it to
	// read our Vertex Array
	// The first argument is the shader location we want to use (location = 0)
	// The second argument is the size of the vector we used in the shader
	// The third argument specifies what the data should be in for the shader
	// The fourth argument is the stride through the vertex array
	// The fifth argument is the starting offset for the vertex array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	// Now enable the vertex attribute at (location = 0)
	glEnableVertexAttribArray(0);

	// Now are VAO is ready to go! Anytime we use this returned ID to bind
	// THE VAO, it will have the above attributes
	return VAO_ID;

}
