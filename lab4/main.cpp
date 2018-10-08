// Local Headers
#define STB_IMAGE_IMPLEMENTATION
#include "glitter.hpp"
#include "camera.h"
#include "ShaderHelpers.h"
#include "ModelLoader.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = mWidth / 2.0f;
float lastY = mHeight / 2.0f;
bool firstMouse = true;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, 0.05f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, 0.05f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, 0.05f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, 0.05f);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, NULL);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

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

    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

	// **************************************
	// Setup Vertex arrays here
	// **************************************
    // data
    
    mlModel crayon;
    if (!LoadModel("/home/dallinfrank/Documents/cs455/lab4",
        "crayon.obj", crayon)) {
        return -1;
    }

    mlModel box;
    if (!LoadModel("/home/dallinfrank/Documents/cs455/lab4",
        "crayonbox.obj", box)) {
        return -1;
    }

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
    int size = crayon.meshes[0].vertices.size() * 8 * sizeof(float);
    int size2 = box.meshes[0].vertices.size() * 8 * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, size+size2, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, crayon.meshes[0].vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, size, size2, box.meshes[0].vertices.data());

    size = crayon.meshes[0].indices.size() * sizeof(int);
    size2 = box.meshes[0].indices.size() * sizeof(int);
    for (int i = 0; i < box.meshes[0].indices.size(); ++i) {
        unsigned int* x = &box.meshes[0].indices[i];
        *x = box.meshes[0].indices[i] + crayon.meshes[0].vertices.size();
    }
    // for (unsigned int i: box.meshes[0].indices) {
    //     std::cout << i << std::endl;
    // }
    std::cout << box.meshes[0].indices.size() << std::endl;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size+size2, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, crayon.meshes[0].indices.data());
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, size, size2, box.meshes[0].indices.data());

    // ======================================
    // Load the shaders
    GLuint myShader = LoadProgram("/home/dallinfrank/Documents/Glitter/Glitter/Shaders/basic.vert", "/home/dallinfrank/Documents/Glitter/Glitter/Shaders/basic.frag");

    // Connect vertex data to shader variables
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // uv attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("/home/dallinfrank/Documents/cs455/lab4/violetCrayon.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        return -1;
    }
    stbi_image_free(data);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    data = stbi_load("/home/dallinfrank/Documents/cs455/lab4/CrayonBox2.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        return -1;
    }
    stbi_image_free(data);

    // Use the Shader Program
    glUseProgram(myShader);
    glUniform1i(glGetUniformLocation(myShader, "texture1"), 0);

    GLint uniPerspective = glGetUniformLocation(myShader, "Perspective");
    GLint uniView = glGetUniformLocation(myShader, "View");
    GLint uniTrans = glGetUniformLocation(myShader, "Trans");

	// **************************************
	
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        processInput(mWindow);

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// **********************************
		// Add rendering code here        
        glBindTexture(GL_TEXTURE_2D, texture1);

        // render container
        glUseProgram(myShader);
        glBindVertexArray(VAO);

        glm::mat4 Perspective = glm::perspective(glm::radians(camera.Zoom), (float) mWidth / (float) mHeight, 0.01f, 100.0f);
        glm::mat4 View = camera.GetViewMatrix();
        glm::mat4 Trans = glm::mat4(1.0f);
        Trans = glm::translate(Trans, glm::vec3(0.0f, -1.0f, 0.0f));
        Trans = glm::scale(Trans, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(uniPerspective, 1, GL_FALSE, glm::value_ptr(Perspective));
        glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(View));
        glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(Trans));
        glDrawElements(GL_TRIANGLES, crayon.meshes[0].indices.size(), GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D, texture2);
        // glUseProgram(myShader);
        // glBindVertexArray(VAO);
        Trans = glm::mat4(1.0f);
        Trans = glm::translate(Trans, glm::vec3(2.5f, -1.0f, 0.0f));
        Trans = glm::scale(Trans, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(uniPerspective, 1, GL_FALSE, glm::value_ptr(Perspective));
        glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(View));
        glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(Trans));
        glDrawElements(GL_TRIANGLES, box.meshes[0].indices.size(), GL_UNSIGNED_INT, (void*) (crayon.meshes[0].indices.size() * sizeof(int)));
		// **********************************

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();

    return EXIT_SUCCESS;
}