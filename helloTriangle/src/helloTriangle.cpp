#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
void processInput(GLFWwindow* window); 

int main() {
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

    // Vertex data definition 
    const float vertices[] { 
        -0.5f, -0.5f, 0.0f, 
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    }; 

    // Vertex buffer object
    unsigned int VBO; 
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

    // Vertex Shader
    const char* vertexShaderSource {R"(
        #version 330 core
        layout (location = 0) in vec3 aPos; 

        void main() {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); 
        }
    )"}; 
    unsigned int vertexShader {glCreateShader(GL_VERTEX_SHADER)}; 
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); 
    glCompileShader(vertexShader);

    // Check if vertex shader compilation was successful 
    int success; 
    char infoLog[512]; 
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); 
    if(!success) 
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); 

    // Fragment Shader
    const char* fragmentShaderSource {R"(
        #version 330 core
        out vec4 FragColor; 

        void main() {
            FragColor = vec4(1.0f, 0.5, 0.2f, 1.0f); 
        }
    )"}; 
    unsigned int fragmentShader {glCreateShader(GL_FRAGMENT_SHADER)}; 
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); 
    glCompileShader(fragmentShader); 
    
    // Check if vertex shader compilation was successful 
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); 
    if(!success) 
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog); 

    // Shader program 
    unsigned int shaderProgram {glCreateProgram()}; 
    glAttachShader(shaderProgram, vertexShader); 
    glAttachShader(shaderProgram, fragmentShader); 
    glLinkProgram(shaderProgram); 

    // Check if shader program linked properley
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); 
    if(!success)  
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog); 
    else {
        glDeleteShader(vertexShader); 
        glDeleteShader(fragmentShader); 
    }

    /* ============ VBO ==============
        // Tell OpenGL how to interpret vertex data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
        glEnableVertexAttribArray(0); 
        
        // 0. Copy vertices into buffer for OpenGL to use 
        glBindBuffer(GL_ARRAY_BUFFER, VBO); 
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
        // 1. Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
        glEnableVertexAttribArray(0); 
        // 2. Use shader program to render an object
        glUseProgram(shaderProgram); 
        // 3. Draw the object
    */

    // VBA
    unsigned int VAO; 
    glGenVertexArrays(1, &VAO); 
    // 1. Bind vertex array object 
    glBindVertexArray(VAO); 
    // 2. Copy vertices into a buffer for OpenGL to Use
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    // 3. Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
    glEnableVertexAttribArray(0); 
    
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
        
        // 4. Draw the object
        glUseProgram(shaderProgram); 
        glBindVertexArray(VAO); 
        glDrawArrays(GL_TRIANGLES, 0, 3); 
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

