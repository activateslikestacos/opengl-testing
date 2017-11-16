// Including built-in libraries
#include <cstdlib>
#include <string>
#include <iostream>

// Including other libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const int HEIGHT = 600,
	WIDTH = 800;

const char WINDOW_NAME[] = "Window Test";

// Function prototypes
void framebuffer_size_callback(GLFWwindow*, int, int);
// Callback function designed to be called everytime the window is resized
// for adjusting the viewport

void handleInput(GLFWwindow*);
// Simple function for handling user input. Will be called in our main loop

void renderItems(GLFWwindow*);
// Clears the screen with a certain color and draws everything needed

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

	// Create a basic rendering loop
	while (!glfwWindowShouldClose(window)) {

		// Handle user input
		handleInput(window);

		// Render
		renderItems(window);

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