//gl
#include "GL/glew.h"
#include "GLUtil.h"
#include "gl/GL.h"
//glfw
#include "GLFW/glfw3.h"
//imgui
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

//app
#include "InputManager.h"
#include "Shader.h"
#include "LSystem.h"
#include "GraphicsTurtle.h"
#include "Camera.h"
#include "ResourceManager.h"
//stl
#include <iostream>
#include <vector>
#include <utility>
#include <sstream>

#define WIDTH 1280
#define HEIGHT 720

//#define DRAW_CIRCLES

GLFWwindow* g_window;

constexpr float red[4]{0.8f,0.f,0.f,1.f};
constexpr float blue[4]{0.f,0.f,0.8f,1.f};
constexpr float green[4]{0.f,0.8f,0.f,1.f};
constexpr float yellow[4]{0.8f,0.8f,0.f,1.f};
constexpr float brown[4]{0.8f,0.2f,0.2f,1.f};

int Init(){

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    g_window = glfwCreateWindow(WIDTH, HEIGHT, "fatty 3 d trees", NULL, NULL);
    if (!g_window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(g_window);
    glfwSwapInterval(0);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "gl broke" << std::endl;
    }
    InputManager::Init(g_window);

    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    //imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(g_window, true);
    const char* glsl_version = "#version 150";
    ImGui_ImplOpenGL3_Init(glsl_version);

    return 1;
}

int main()
{
    if (Init() == -1) {
        std::cout << "Error initialising the program! aborting.\n";
        return -1;
    }

    Camera mainCamera;
    mainCamera.SetPosition(glm::vec3{0.0f, 0.0f, 1.0f});
    float cameraSpeed { 0.001f };
    float cameraRotationSpeed { 0.05f};

    glm::mat4 projectionMatrix { glm::perspective(glm::radians(45.f), static_cast<float>(WIDTH) / HEIGHT, 0.1f, 1000.f) };
    glm::mat4 viewMatrix { mainCamera.GetViewMatrix() };
    
    std::string axiom = "X";
    std::string rules = "X-FAECXDCBGXDBGFCBGFXDAEX,F-FF,A-A,B-B,C-C,D-D,E-E,G-G";
    std::string turtleRules = "F-F 0.025,A-+ 25, B-- 25,C-[,D-],E-& 25,G-^ 25";

    int generation = 1;
    float maxBranchRadius = 0.05f;
    float minBranchRadius = 0.01f;

    std::string rulesAtGeneration = LSystem::CalculateLSystemAtGeneration(axiom, rules, generation);
    
    GraphicsTurtle turtle;
    turtle.BuildLSystemToTurtleMappings(turtleRules);
    GraphicsTurtle::renderData data = turtle.GenerateGeometryOfLSystemRuleString(rulesAtGeneration);
    unsigned vao = GLUtil::buildVAOfromData(data);
    ResourceManager::AddBuffer(vao);

    bool renderCircles = false;

    std::vector<unsigned int> circleVaos;

    Shader shader("res/shaders/BasicShader.txt");
    Shader shader2("res/shaders/BasicShaderGeometry.txt");
    shader.Bind();
    bool useGeoShader = false;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(g_window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //INPUT
        InputManager::Poll(g_window);
        //~INPUT

        //LOGIC

        if (InputManager::GetKeyState(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            ResourceManager::Clear();
            break;
        }

        //camera
        if (InputManager::GetKeyState(GLFW_KEY_Q) ==     GLFW_PRESS) mainCamera.Translate(-mainCamera.GetUp() * cameraSpeed);
        if (InputManager::GetKeyState(GLFW_KEY_E) ==     GLFW_PRESS) mainCamera.Translate(mainCamera.GetUp() * cameraSpeed);
        if (InputManager::GetKeyState(GLFW_KEY_W) ==     GLFW_PRESS) mainCamera.Translate(mainCamera.GetForward() * cameraSpeed);
        if (InputManager::GetKeyState(GLFW_KEY_A) ==     GLFW_PRESS) mainCamera.Translate(-mainCamera.GetRight() * cameraSpeed);
        if (InputManager::GetKeyState(GLFW_KEY_S) ==     GLFW_PRESS) mainCamera.Translate(-mainCamera.GetForward() * cameraSpeed);
        if (InputManager::GetKeyState(GLFW_KEY_D) ==     GLFW_PRESS) mainCamera.Translate(mainCamera.GetRight() * cameraSpeed);
        if (InputManager::GetKeyState(GLFW_KEY_UP) ==    GLFW_PRESS) mainCamera.Rotate(glm::vec3{cameraRotationSpeed, 0.0f, 0.0f});
        if (InputManager::GetKeyState(GLFW_KEY_DOWN) ==  GLFW_PRESS) mainCamera.Rotate(glm::vec3{-cameraRotationSpeed, 0.0f, 0.0f});
        if (InputManager::GetKeyState(GLFW_KEY_LEFT) ==  GLFW_PRESS) mainCamera.Rotate(glm::vec3{0.0f, cameraRotationSpeed, 0.0f});
        if (InputManager::GetKeyState(GLFW_KEY_RIGHT) == GLFW_PRESS) mainCamera.Rotate(glm::vec3{0.0f, -cameraRotationSpeed, 0.0f});

        viewMatrix = mainCamera.GetViewMatrix();
        shader.SetUniformMat4f("projectionMatrix", projectionMatrix);
        shader.SetUniformMat4f("viewMatrix", viewMatrix);

        
#ifdef DRAW_CIRCLES
        if (InputManager::GetKeyToggle(GLFW_KEY_C))
            renderCircles = !renderCircles;
        if (InputManager::GetKeyToggle(GLFW_KEY_X)) {
            useGeoShader = !useGeoShader;
            if (useGeoShader) shader2.Bind();
            else shader.Bind();
        }
#endif
        if (InputManager::GetKeyToggle(GLFW_KEY_2)) {
            generation++;
            rulesAtGeneration = LSystem::CalculateLSystemAtGeneration(axiom, rules, generation);
            std::cout << "L-System at generation: " << generation << ": " << rulesAtGeneration << std::endl;
            turtle.Reset();
            GraphicsTurtle::renderData t = turtle.GenerateGeometryOfLSystemRuleString(rulesAtGeneration);
            ResourceManager::RemoveBuffer(vao);
            vao = GLUtil::buildVAOfromData(t);
            ResourceManager::AddBuffer(vao);
            std::cout << t.vertexes.size() << std::endl;
            
#ifdef DRAW_CIRCLES
            for (const unsigned int v : circleVaos) {
                ResourceManager::RemoveBuffer(v);
            }
            circleVaos.clear();

            for (int i = 0; i < t.indexes.size()-1; i+=2) {
                float radius = std::max(maxBranchRadius / (t.vertexes[t.indexes[i]].branchCount + 1), minBranchRadius);
                float radius2 = 0.0f;
                if (!t.vertexes[t.indexes[i + 1]].isLeaf)
                    radius2 = std::max(maxBranchRadius / (t.vertexes[t.indexes[i+1]].branchCount + 1), minBranchRadius);
                float cylinderColour[4]{brown[0],brown[1],brown[2],brown[3]};
                if (radius == minBranchRadius) {
                    cylinderColour[0] = green[0];
                    cylinderColour[1] = green[1];
                    cylinderColour[2] = green[2];
                    cylinderColour[3] = green[3];
                }
                unsigned int v = GLUtil::buildCylinderVAO(t.vertexes[t.indexes[i]].position, t.vertexes[t.indexes[i+1]].position, t.vertexes[t.indexes[i]].circleNormal, t.vertexes[t.indexes[i+1]].circleNormal, radius, radius2, 10, cylinderColour);
                ResourceManager::AddBuffer(v);
                circleVaos.push_back(v);
            }
#endif
        }
            
        if (InputManager::GetKeyToggle(GLFW_KEY_1)) {
            if (generation > 0) generation--;
            rulesAtGeneration = LSystem::CalculateLSystemAtGeneration(axiom, rules, generation);
            turtle.Reset();
            GraphicsTurtle::renderData t = turtle.GenerateGeometryOfLSystemRuleString(rulesAtGeneration);
            ResourceManager::RemoveBuffer(vao);
            vao = GLUtil::buildVAOfromData(t);
            ResourceManager::AddBuffer(vao);
#ifdef DRAW_CIRCLES
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
#endif
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Light Settings");
        //ImGui::SliderFloat3("Light WorldPosition", &lightPosition.x, -1000.0f, 1000.0f);
		//shader.SetUniformvec3f("lightWorldPosition", lightPosition.x, lightPosition.y, lightPosition.z);
        //ImGui::ColorEdit3("Ambient Intensity", &shader.lightInfo.ambientIntensity.x);            
        //framerate
        //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
        

        ImGui::Render();

        //~LOGIC



        //RENDER
        glBindVertexArray(vao);
        glDrawElements(GL_LINES, 10000000, GL_UNSIGNED_INT, 0);

#ifdef DRAW_CIRCLES
        if (renderCircles) {
            for (unsigned vao : circleVaos) {
                glBindVertexArray(vao);
                glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);
            }
        }
#endif
        //draw UI over everything else
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //~RENDER

        glfwSwapBuffers(g_window);
    }
    
    glfwTerminate();
}