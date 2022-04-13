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
#include "ResourceManager.h"

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

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.f), 16.f / 9.f, 0.f, 100.f);
    glm::mat4 viewMatrix = Camera::GetViewMatrix();
    
    float red[4]{0.8f,0.f,0.f,1.f};
    float blue[4]{0.f,0.f,0.8f,1.f};
    float green[4]{0.f,0.8f,0.f,1.f};
    float yellow[4]{0.8f,0.8f,0.f,1.f};

    std::string axiom = "B";
    std::string rules = "A-AA,B-ACEBDFB,C-C,D-D,E-E,F-F";
    std::string turtleRules = "A-F 0.1,B-F 0.05,C-[,D-],E-+ 45,F-- 45";
    int generation = 1;
        
    std::string rulesAtGeneration = LSystem::CalculateLSystemAtGeneration(axiom, rules, generation);
        
    GraphicsTurtle turtle;
    turtle.BuildLSystemToTurtleMappings(turtleRules);
    GraphicsTurtle::renderData data = turtle.GenerateGeometryOfLSystemRuleString(rulesAtGeneration);
    unsigned vao = GLUtil::buildVAOfromData(data);
    ResourceManager::AddBuffer(vao);

    bool renderCircles = false;

    std::vector<unsigned int> circleVaos;

    Shader shader("res/shaders/BasicShader.txt");
    shader.Bind();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        Camera::Update();
        viewMatrix = Camera::GetViewMatrix();
        shader.SetUniformMat4f("projectionMatrix", projectionMatrix);
        shader.SetUniformMat4f("viewMatrix", viewMatrix);

        

        if (InputManager::GetKeyToggle(GLFW_KEY_C))
            renderCircles = !renderCircles;

        if (InputManager::GetKeyToggle(GLFW_KEY_W)) {
            generation++;
            rulesAtGeneration = LSystem::CalculateLSystemAtGeneration(axiom, rules, generation);
            turtle.Reset();
            GraphicsTurtle::renderData t = turtle.GenerateGeometryOfLSystemRuleString(rulesAtGeneration);
            ResourceManager::RemoveBuffer(vao);
            vao = GLUtil::buildVAOfromData(t);
            ResourceManager::AddBuffer(vao);
            //for (const auto& v : t.vertexes) {
            //    std::cout << v.position[0] << " " << v.position[1] << " " << v.position[2] << std::endl;
            //}
            std::cout << t.vertexes.size() << std::endl;
            
            for (const unsigned int v : circleVaos) {
                ResourceManager::RemoveBuffer(v);
            }
            circleVaos.clear();

            for (const GraphicsTurtle::vertex& p : t.vertexes)
            {
                float radiusRatio = (t.maximumDistanceFromOrigin - p.distanceFromOrigin) / t.maximumDistanceFromOrigin;
                if (radiusRatio == 0.0f) 
                    radiusRatio = 0.0001f;
                float radius = 0.05f * radiusRatio;
                unsigned int v = GLUtil::buildCircleVAO(p.position, p.circleNormal, radius, 10, green);
                ResourceManager::AddBuffer(v);
                circleVaos.push_back(v);
            }
        }
            
        if (InputManager::GetKeyToggle(GLFW_KEY_S)) {
            if (generation > 0) generation--;
            rulesAtGeneration = LSystem::CalculateLSystemAtGeneration(axiom, rules, generation);
            turtle.Reset();
            GraphicsTurtle::renderData t = turtle.GenerateGeometryOfLSystemRuleString(rulesAtGeneration);
            ResourceManager::RemoveBuffer(vao);
            vao = GLUtil::buildVAOfromData(t);
            ResourceManager::AddBuffer(vao);

            for (const unsigned int v : circleVaos) {
                ResourceManager::RemoveBuffer(v);
            }
            circleVaos.clear();

            for (const GraphicsTurtle::vertex& p : t.vertexes)
            {
                float radiusRatio = (t.maximumDistanceFromOrigin - p.distanceFromOrigin) / t.maximumDistanceFromOrigin;
                if (radiusRatio == 0.0f) 
                    radiusRatio = 0.0001f;
                float radius = 0.05f * radiusRatio;
                unsigned int v = GLUtil::buildCircleVAO(p.position, p.circleNormal, radius, 10, green);
                ResourceManager::AddBuffer(v);
                circleVaos.push_back(v);
            }
            std::cout << t.vertexes.size() << std::endl;

        }

        if (InputManager::GetKeyState(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            ResourceManager::Clear();
            break;
        }

        glBindVertexArray(vao);
        glDrawElements(GL_LINES, 10000000, GL_UNSIGNED_INT, 0);

        if (renderCircles) {
            for (unsigned vao : circleVaos) {
                glBindVertexArray(vao);
                glDrawElements(GL_LINES, 20, GL_UNSIGNED_INT, 0);
            }
        }




        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        InputManager::Poll(window);
    }
    
    glfwTerminate();
}