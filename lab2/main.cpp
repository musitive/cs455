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
    // data
    float vertices[] = {
        // position          // color
         0.3f, -0.3f, 0.0f,  0.9f, 0.3f, 0.3f,
        -0.3f, -0.3f, 0.0f,  0.3f, 0.9f, 0.3f,
        -0.3f,  0.3f, 0.0f,  0.3f, 0.3f, 0.9f,
         0.3f,  0.3f, 0.0f,  0.5f, 0.1f, 0.5f,
         0.0f,  0.6f, 0.0f,  0.1f, 0.5f, 0.5f,
    };
    
    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0,
        3, 4, 2
    };
    
    // Generate the VAO and VBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO and VBO for initialization
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Load data into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    // ======================================
    // Load the shaders
    GLuint myShader = LoadProgram("../Shaders/basic.vert", "../Shaders/basic.frag");

    // Connect vertex data to shader variables
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Use the Shader Program
    glUseProgram(myShader);

    GLint uniTrans = glGetUniformLocation(myShader, "Trans");
    GLint uniMask = glGetUniformLocation(myShader, "Mask");

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
//        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // FIRST HOUSE
        glm::mat4 Trans = { 0.6f, 0.0f, 0.0f, 0.0f,
                            0.0f, 0.6f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.4f, 0.4f, 0.0f, 1.0f };
        glm::vec3 Mask = { 0.1f, 1.0f, 2.0f };
        glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(Trans));
        glUniform3fv(uniMask, 1, glm::value_ptr(Mask));
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        // SECOND HOUSE
        Trans = glm::mat4(1.0f);
        Mask = glm::vec3(1.0f);
        glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(Trans));
        glUniform3fv(uniMask, 1, glm::value_ptr(Mask));
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);


        // THIRD HOUSE
        Trans = {   1.4f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.4f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    -0.4f, -0.4f, 0.0f, 1.0f };
        Mask = { 1.0f, 0.3f, 0.0f };
        glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(Trans));
        glUniform3fv(uniMask, 1, glm::value_ptr(Mask));
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		// **********************************

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();

    return EXIT_SUCCESS;
}
