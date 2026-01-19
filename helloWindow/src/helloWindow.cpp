#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
void processInput(GLFWwindow* window); 

int main()
{
    // Initialize GLFW library
    glfwInit(); 

    // GLFW requests from the graphics driver a OpenGL 3.3 context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 

    // Use the core OpenGL Profile which doesn't include legacy features
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window object 
    GLFWwindow* window {glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL)}; 
    if (!window) {
        std::cout << "Failed to create GLFW window\n"; 
        glfwTerminate(); 
        return -1; 
    }
    glfwMakeContextCurrent(window); // Tell GLFW to make the context of the window the main context on the current thread
    
    // Initialize GLAD before calling any OpenGL function 
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n"; 
        return -1; 
    }

    // Tell OpenGL the size of the rendering window 
    glViewport(0, 0, 800, 600);   

    // Tell GLFW to call this function every time window resizes
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

    // Render loop
    // glfwWindowShouldClose - Checks if GLFW has been told to close
    // glfwPollEvents - Checks if any events are triggered, updating window state and calls callback functions
    // glfwSwapBuffers - Will swap the color buffer and show it as output to screen
    while(!glfwWindowShouldClose(window)) {
        processInput(window); 
        glfwSwapBuffers(window); 
        glfwPollEvents(); 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    // Delete/Clean all of GLFW allocated resources
    glfwTerminate(); 
    return 0; 

}

// Adjusts viewpoert when user resizes the window 
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); 
}

// Function implements escape logic 
void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); 
}
