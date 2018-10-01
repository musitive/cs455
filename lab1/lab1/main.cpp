// Local Headers
#define STB_IMAGE_IMPLEMENTATION
#include "glitter.hpp"
#include "ShaderHelpers.h"
#include "ModelLoader.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	// **************************************
	// Setup Vertex arrays here
	// **************************************
    // Generate the VAOs
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    // Bind a VAO for initialization
    glBindVertexArray(VAO);

    // Generate the VBOs
    GLuint VBO;
    glGenBuffers(1, &VBO);

    // Bind a VBO for initialization
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Load data into the VBO
    float vertices[] = {-0.5f, -0.75f, 0.0f,
                        0.25f, -0.5f, 0.0f,
                        0.0f,  0.75f, 0.0f};
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                    vertices, GL_STATIC_DRAW);

    // ======================================
    // Load the shaders
    GLuint myShader = LoadProgram("../Shaders/basic.vert",
			"../Shaders/basic.frag");
    
    // Use the Shader Program
    glUseProgram(myShader);

    // Connect vertex data to shader variables
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);


	// **************************************
	
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// **********************************
		// Add rendering code here
        glDrawArrays(GL_TRIANGLES, 0, 3);

		// **********************************

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();

    return EXIT_SUCCESS;
}
