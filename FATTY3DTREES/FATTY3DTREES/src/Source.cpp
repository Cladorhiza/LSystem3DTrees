//gl
#include "GL/glew.h"
#include "GLUtil.h"
//glfw
#include "GLFW/glfw3.h"
#include "InputManager.h"

#include "Shader.h"
#include "LSystem.h"
#include "GraphicsTurtle.h"
#include "Camera.h"

#include <iostream>
#include <vector>
#include <utility>
#include <sstream>

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "fatty 3 d trees", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "gl broke" << std::endl;
    }
    InputManager::Init(window);

    glViewport(0, 0, 1280, 720);
    glClearColor(0.f, 0.0f, 0.0f, 1.f);
    
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.f), 16.f/9.f, 0.f, 100.f);
    glm::mat4 viewMatrix = Camera::GetViewMatrix();
    
    std::string axiom = "B";
    std::string rules = "A-AA,B-ACEBDCFBDCGBDHB,C-C,D-D,E-E,F-F,G-G,H-H";
    std::string turtleRules = "A-F 0.1,B-F 0.05,C-[,D-],E-+ 45,F-- 45,G-& 45,H-^ 45";
    int generation = 3;

    std::string rulesAtGeneration = LSystem::CalculateLSystemAtGeneration(axiom, rules, generation);

    GraphicsTurtle turtle;
    turtle.BuildLSystemToTurtleMappings(turtleRules);
    unsigned vao3 = turtle.GenerateGeometryOfLSystemRuleString(rulesAtGeneration);
    
    Shader shader("res/shaders/BasicShader.txt");
    shader.Bind();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        if (InputManager::GetKeyToggle(GLFW_KEY_W)) {
            generation++;
            rulesAtGeneration = LSystem::CalculateLSystemAtGeneration(axiom, rules, generation);
            turtle.Reset();
            vao3 = turtle.GenerateGeometryOfLSystemRuleString(rulesAtGeneration);
        }
        
        if (InputManager::GetKeyToggle(GLFW_KEY_S)) {
            if (generation > 0) generation--;
            rulesAtGeneration = LSystem::CalculateLSystemAtGeneration(axiom, rules, generation);
            turtle.Reset();
            vao3 = turtle.GenerateGeometryOfLSystemRuleString(rulesAtGeneration);
        }
        Camera::Update();
        viewMatrix = Camera::GetViewMatrix();
        shader.SetUniformMat4f("projectionMatrix", projectionMatrix);
        shader.SetUniformMat4f("viewMatrix", viewMatrix);

        glBindVertexArray(vao3);
        glDrawElements(GL_LINES, 10000000, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        InputManager::Poll(window);
    }

    glfwTerminate();
}