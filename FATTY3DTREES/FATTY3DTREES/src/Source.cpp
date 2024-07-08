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
#define CLIP_NEAR 0.1f
#define CLIP_FAR 1000.f
#define VFOV 70.f

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
    float cameraRotationSpeed { 0.025f};

    glm::mat4 projectionMatrix { glm::perspective(glm::radians(VFOV), static_cast<float>(WIDTH) / HEIGHT, CLIP_NEAR, CLIP_FAR) };
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
    turtle.Reset();
    unsigned vao = GLUtil::buildVAOfromData(data);
    ResourceManager::AddVertexArray(vao);

    Shader shader("res/shaders/BasicShader.txt");
    shader.Bind();
    shader.SetUniformMat4f("projectionMatrix", projectionMatrix);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(g_window))
    {
        
        
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
        shader.SetUniformMat4f("viewMatrix", viewMatrix);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("L-System Settings");

        ImGui::SeparatorText("Axiom");

        static char str0[128] = "";
        ImGui::InputText("Axiom", str0, IM_ARRAYSIZE(str0));
        
        ImGui::SeparatorText("Productions");
        
        //productions tooltip
        ImGui::SameLine(); 
        ImGui::TextDisabled("(?)");
        if (ImGui::BeginItemTooltip())
        {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(
                "Productions are Formatted as so:\n"
                "A-ABB\n"
                "The LHS of the hyphen will be replaced by the RHS of the hyphen each generation.\n"
            );
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }

        static std::vector<char*> rawRules;
        if (ImGui::Button("ADD RULE")) {

            rawRules.emplace_back();
            rawRules.back() = new char[128] {"rule"};

        }
        
        for (int i{ 0 }; i < static_cast<int>(rawRules.size()); i++) {

            std::string s{ "rule " };
            s += std::to_string(i);
            ImGui::InputText(s.c_str(), rawRules[i], 128);
            ImGui::SameLine();
            s = "DEL";
            s += std::to_string(i);
            if (ImGui::Button(s.c_str())) {
                delete[] rawRules[i];
                rawRules.erase(rawRules.begin() + i);
                i--;
            }
        }
        
        ImGui::SeparatorText("Turtle Productions");
        
        //Turtle productions tooltip
        ImGui::SameLine(); 
        ImGui::TextDisabled("(?)");
        if (ImGui::BeginItemTooltip())
        {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(
                "Turtle productions are one of these following instructions:\n"
                "F Move forward a step of length d.\n"
                "f Move forward a step of length d without drawing a line.\n"
                "+ Turn left by angle ?, using rotation matrix RU.\n"
                "? Turn right by angle ?, using rotation matrix RU(-).\n"
                "& Pitch down by angle ?, using rotation matrix RL.\n"
                "^ Pitch up by angle ?, using rotation matrix RL(-).\n"
                "\\ Roll left by angle ?, using rotation matrix RH.\n"
                "/ Roll right by angle ?, using rotation matrix RH(-).\n"
                "| Turn around, using rotation matrix RU(180).\n"
                "[ push matrix\n"
                "] pop matrix\n"
                "\nThese instructions are formatted as follows:\n"
                "X-Y Z\n"
                "Where X is an L-System symbol, Y is one of the above instructions and Z is a floating point or integer value.\n"
                "The only exception being [ and ], these are written as X-[ or X-]\n"
            );
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }

        static std::vector<char*> rawTurtleRules;
        if (ImGui::Button("ADD TURTLE RULE")) {

            rawTurtleRules.emplace_back();
            rawTurtleRules.back() = new char[128] {"turtle rule"};

        }
        
        for (int i{ 0 }; i < static_cast<int>(rawTurtleRules.size()); i++) {

            std::string s{ "turtle rule " };
            s += std::to_string(i);
            ImGui::InputText(s.c_str(), rawTurtleRules[i], 128);
            ImGui::SameLine();
            s = "DELT";
            s += std::to_string(i);
            if (ImGui::Button(s.c_str())) {
                delete[] rawTurtleRules[i];
                rawTurtleRules.erase(rawTurtleRules.begin() + i);
                i--;
            }
        }

        ImGui::SeparatorText("Generation");

        ImGui::SliderInt("Generation", &generation, 0, 100);
        if (ImGui::Button("Generate")){

            rulesAtGeneration = LSystem::CalculateLSystemAtGeneration(axiom, rules, generation);
            data = turtle.GenerateGeometryOfLSystemRuleString(rulesAtGeneration);
            turtle.Reset();
            ResourceManager::RemoveVertexArray(vao);
            vao = GLUtil::buildVAOfromData(data);
            ResourceManager::AddVertexArray(vao);
            std::cout << "L-System length at generation: " << generation << ": " << data.vertexes.size() << std::endl;

        }
        ImGui::End();
        ImGui::ShowDemoWindow();
        ImGui::Render();

        //~LOGIC



        //RENDER
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawElements(GL_LINES, data.indexes.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //draw UI over everything else
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //~RENDER

        glfwSwapBuffers(g_window);
    }
    
    glfwTerminate();
}