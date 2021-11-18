﻿#include <glad/glad.h>
#include "application.h"
#include <iostream>
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_opengl3.h"

unsigned int cubeVAO = 0;
unsigned int cubeIndexCount;
void DrawCube() {
    if(cubeVAO == 0){
        std::vector<Vertex> vertices = {
            // Back face
            { -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f }, // Bottom-left
            {  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f }, // top-right
            {  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f }, // bottom-right
            { -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f }, // top-left

            // Front face
            { -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f }, // bottom-left
            {  0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f }, // bottom-right
            {  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f }, // top-right
            { -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f }, // top-left

            // Left face
            { -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f }, // top-right
            { -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f }, // top-left
            { -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f }, // bottom-left
            { -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f }, // bottom-right


            // Right face
            {  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f }, // top-left
            {  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f }, // bottom-right
            {  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f }, // top-right
            {  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f }, // bottom-left

            // Bottom face
            { -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f }, // top-right
            {  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f }, // top-left
            {  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f }, // bottom-left
            { -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f }, // bottom-right

            // Top face                             
            { -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f }, // top-left
            {  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f }, // bottom-right
            {  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f }, // top-right
            { -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f }  // bottom-left
        };

        std::vector<unsigned int> indices = {
            0, 1, 2,
            1, 0, 3,

            4, 5, 6,
            6, 7, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            13, 12, 15,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            21, 20, 23
        };
        cubeIndexCount = indices.size();
        SetupMesh(cubeVAO, vertices, indices);
    }
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, cubeIndexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

unsigned int quadVAO = 0;
unsigned int quadIndexCount;
void DrawQuad() {
    if (quadVAO == 0) {
        std::vector<Vertex> vertices = {
            { -1.f,  1.f, 0.f, 0, 1, 0, 0.f, 1.f },
            { -1.f, -1.f, 0.f, 0, 1, 0, 0.f, 0.f },
            {  1.f,  1.f, 0.f, 0, 1, 0, 1.f, 1.f },
            {  1.f, -1.f, 0.f, 0, 1, 0, 1.f, 0.f },
        };
        std::vector<unsigned int> indices = { 0, 1, 2, 1, 3, 4 };
        quadIndexCount = indices.size();
        SetupMesh(cubeVAO, vertices, indices);
    }

    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, quadIndexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

unsigned int sphereVAO = 0;
unsigned int sphereIndexCount;
void DrawSphere() {
    if (sphereVAO == 0) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        const unsigned int X_SEGMENTS = 128;
        const unsigned int Y_SEGMENTS = 128;
        const float PI = 3.14159265359;
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                vertices.push_back({ xPos, yPos, zPos, xPos, yPos, zPos, xSegment, ySegment });
            }
        }

        bool oddRow = false;
        for (int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        sphereIndexCount = indices.size();
        SetupMesh(cubeVAO, vertices, indices);
    }
    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, sphereIndexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SetupMesh(unsigned int& VAO, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
    glGenVertexArrays(1, &VAO);
    unsigned int VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    unsigned int vertexSize = sizeof(Vertex);
    unsigned int bufferSize = vertices.size() * sizeof(Vertex);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

bool Application::firstMouse = true;
bool Application::enterWindowFlag = true;

double Application::lastX = wndWidth / 2.0f;
double Application::lastY = wndHeight / 2.0f;

Camera Application::camera = Camera();

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Application::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(PGUP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(PGDOWN, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void Application::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (enterWindowFlag == true) {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        double xoffset = xpos - lastX;
        double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void Application::cursor_callback(GLFWwindow* window, int flag) {
    if (flag == GLFW_TRUE)
        std::cout << "flag true" << std::endl;
}

bool Application::InitializeWindow() {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    window = glfwCreateWindow(wndWidth, wndHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorEnterCallback(window, cursor_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return true;
}

bool Application::InitGLAD() {
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

bool Application::Init() {
    if (!InitializeWindow())
        return false;

    if (!InitGLAD())
        return false;

    return true;
}

void Application::LoadResources() {
    LoadModels();
    LoadTextures();
    LoadShaders();
}

void Application::LoadModels() {
    // load models
    // -----------
    dragonModel = std::make_unique<Model>("../Resources/dragon.obj");
    //cube = std::make_unique<Cube>();
    //sphere = std::make_unique<Sphere>();
    //quad = std::make_unique<Quad>();
}

void Application::LoadShaders() {
    // build and compile shaders
    // -------------------------
    shaderMap["pbrShader"                     ]  =   Shader("pbrShader", "shaders/pbrshader.vs", "shaders/pbrshader.fs");
    shaderMap["equirectangularToCubemapShader"]  =   Shader("equirectangularToCubemapShader", "shaders/cubeMap.vs", "shaders/equirectangular_to_cubeMap.fs");
    shaderMap["backgroundShader"              ]  =   Shader("backgroundShader", "shaders/background.vs", "shaders/background.fs");
    shaderMap["irradianceShader"              ]  =   Shader("irradianceShader", "shaders/cubeMap.vs", "shaders/irradiance_convolution.fs");
    shaderMap["prefilterShader"               ]  =   Shader("prefilterShader", "shaders/cubeMap.vs", "shaders/prefilter.fs");
    shaderMap["brdfShader"                    ]  =   Shader("brdfShader", "shaders/brdf.vs", "shaders/brdf.fs");
    shaderMap["phong"                         ]  =   Shader("phong", "shaders/phong.vs", "shaders/phong.fs");

    auto& backgroundShader = shaderMap["backgroundShader"];
    backgroundShader.use();
    backgroundShader.setMat4("view", view);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureMap["envCubemap"]->id);

    auto& pbrShader = shaderMap["pbrShader"];
    pbrShader.use();
    pbrShader.setInt("irradianceMap", 0);
    pbrShader.setInt("prefilterMap", 1);
    pbrShader.setInt("brdfLUT", 2);
    pbrShader.setInt("albedoMap", 3);
    pbrShader.setInt("normalMap", 4);
    pbrShader.setInt("metallicMap", 5);
    pbrShader.setInt("roughnessMap", 6);
    pbrShader.setInt("aoMap", 7);
    pbrShader.setInt("skybox", 8);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureMap["irradianceMap"]->id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureMap["prefilterMap"]->id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureMap["brdfLUTTex"]->id);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, textureMap["wallAlbedoMap"]->id);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, textureMap["wallNormalMap"]->id);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, textureMap["wallMetallicMap"]->id);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, textureMap["wallRoughnessMap"]->id);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, textureMap["wallAOMap"]->id);
    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureMap["envCubemap"]->id);
}

void Application::LoadTextures() {
    textureMap["wallAlbedoMap"]     = std::make_unique<Texture>("wallAlbedoMap", "../Resources/PBR/rusted_iron/albedo.png", true);
    textureMap["wallNormalMap"]     = std::make_unique<Texture>("wallNormalMap", "../Resources/PBR/rusted_iron/normal.png", false);
    textureMap["wallMetallicMap"]   = std::make_unique<Texture>("wallMetallicMap", "../Resources/PBR/rusted_iron/metallic.png", false);
    textureMap["wallRoughnessMap"]  = std::make_unique<Texture>("wallRoughnessMap", "../Resources/PBR/rusted_iron/roughness.png", false);
    textureMap["wallAOMap"]         = std::make_unique<Texture>("wallAOMap", "../Resources/PBR/rusted_iron/ao.png", true);

    textureMap["equirectangularMap"]       = std::make_unique<Texture>("equirectangularMap", "../Resources/IBL/newport_loft.hdr");

    textureMap["brdfLUTTex"] = std::make_unique<Texture>("brdfLUTTex");
    textureMap["irradianceMap"] = std::make_unique<TextureCube>("irradianceMap", 32, 32);
    textureMap["prefilterMap"] = std::make_unique<TextureCube>("prefilterMap", 128, 128, true);
    textureMap["envCubemap"] = std::make_unique<TextureCube>("envCubemap");
}

void Application::SetupGUI() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    const char* glsl_version = "#version 330";
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void Application::RenderGUI() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Panel");
        //ShowMonitor(&showMonitor);

        ImGui::Checkbox("Demo Window", &show_demo_window);
        // Edit bool storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        //ImGui::SliderFloat("viewField", &viewField, 0.0f, 90.0f, "viewField = %.3f");
        ImGui::SliderFloat("VIEWFIELD", &camera.Zoom, 0.0f, 90.0f, "viewField = %.3f");

        ImGui::Separator();

        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        if (uixpos < ((double)ImGui::GetWindowPos().x + (double)ImGui::GetWindowSize().x) && uixpos > ImGui::GetWindowPos().x && uiypos < ((double)ImGui::GetWindowPos().y + (double)ImGui::GetWindowSize().y) && uiypos > ImGui::GetWindowPos().y) {
            // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            enterWindowFlag = false;
        }

        else
        {
            enterWindowFlag = true;
            // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        ImGui::End();
    }
}

void Application::ShowMonitor(bool* p_open)
{
    const float DISTANCE = 10.0f;
    static int corner = 0;
    ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
    ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
    if (corner != -1)
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
    if (ImGui::Begin("Example: Simple Overlay", p_open, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        ImGui::Separator();
        ImGui::Text("item : (%.1f)", (float)item_current);
        ImGui::Text("Camera position: (%.1f,%.1f,%.1f)", camera.Position.x, camera.Position.y, camera.Position.z);

        if (ImGui::IsMousePosValid())
        {
            ImGui::Text("Mouse position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
        }
        else
            ImGui::Text("Mouse position: <invalid>");
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom", NULL, corner == -1))
                corner = -1;
            if (ImGui::MenuItem("Top-left", NULL, corner == 0))
                corner = 0;
            if (ImGui::MenuItem("Top-right", NULL, corner == 1))
                corner = 1;
            if (ImGui::MenuItem("Bottom-left", NULL, corner == 2))
                corner = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner == 3))
                corner = 3;
            if (p_open && ImGui::MenuItem("Close"))
                *p_open = false;
            ImGui::EndPopup();
        }
        ImGui::End();
    }
}

void Application::Run() {

    LoadResources();
    SetupGUI();
    PreBake();
    SetOpenGLState();
    unsigned int sphereVAO = 0;
    unsigned int indexCount;
    // lights
    // ------
    glm::vec3 lightPositions[] = {
        glm::vec3(-10.0f,  10.0f, 10.0f),
        glm::vec3(10.0f,  10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3(10.0f, -10.0f, 10.0f),
    };
    glm::vec3 lightColors[] = {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };

    // render loop
    // -----------
    double lastTime = glfwGetTime();
    int frames = 0;
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)wndWidth / (float)wndHeight, 0.1f, 100.0f);
    while (!glfwWindowShouldClose(window))
    {
        RenderGUI();
        // per-frame time logic
        // --------------------

        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glfwGetCursorPos(window, &uixpos, &uiypos);
        // initialize static shader uniforms 
        // --------------------------------------------------
        auto& pbrShader = shaderMap["pbrShader"];
        pbrShader.use();
        pbrShader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        pbrShader.setMat4("view", view);
        pbrShader.setVec3("viewPos", camera.Position);

        for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
        {
            glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
            newPos = lightPositions[i];
            pbrShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
            pbrShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
        }

        auto& phongShader = shaderMap["phong"];
        phongShader.use();
        phongShader.setMat4("view", view);
        phongShader.setMat4("projection", projection);
        // dragon
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.0, 0.0, 2.0));
        phongShader.setMat4("model", model);
        dragonModel->Draw();

        // sphere
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(3.0, 0.0, 2.0));
        //pbrShader.setMat4("model", model);
        //sphere->Draw();
        
        auto& backgroundShader = shaderMap["backgroundShader"];
        backgroundShader.use();
        backgroundShader.setMat4("projection", projection);
        backgroundShader.setMat4("view", view);
        DrawCube();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        double currTime = glfwGetTime();
        frames++;
        if (currTime - lastTime >= 1.0) {
            // If last prinf() was more than 1 sec ago
            // printf and reset timer
            frames = 0;
            lastTime += 1.0;
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

void Application::SetOpenGLState() {
    // configure global OpenGL state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glViewport(0, 0, wndWidth, wndHeight);
}

void Application::PreBake() {
    unsigned int captureFBO;
    unsigned int captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    // PBR: load the HDR environment map
    // ---------------------------------

    // PBR: setup cubemap to render to and attach to framebuffer
    // ---------------------------------------------------------

    // PBR: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    // ----------------------------------------------------------------------------------------------
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    // PBR: convert HDR equirectangular environment map to cubemap equivalent
    // ----------------------------------------------------------------------
    auto& equirectangularToCubemapShader = shaderMap["equirectangularToCubemapShader"];
    equirectangularToCubemapShader.use();
    equirectangularToCubemapShader.setInt("equirectangularMap", 0);
    equirectangularToCubemapShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureMap["equirectangularMap"]->id);

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    auto& envCubemap = textureMap["envCubemap"];
    for (unsigned int i = 0; i < 6; ++i)
    {
        equirectangularToCubemapShader.setMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap->id, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap->id);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    //ibl-diffuse
    // PBR: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
    // --------------------------------------------------------------------------------

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    // PBR: solve diffuse integral by convolution to create an irradiance (cube)map.
    // -----------------------------------------------------------------------------
    auto& irradianceShader = shaderMap["irradianceShader"];
    irradianceShader.use();
    irradianceShader.setInt("environmentMap", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap->id);
    irradianceShader.setMat4("projection", captureProjection);

    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        irradianceShader.setMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, textureMap["irradianceMap"]->id, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //ibl-specular prefilter
    //PBR: create a prefilter cubemap and re-scale captureFbp to pre-filter scale

    //PBR: using quasi-montecarlo to simulate 
    auto& prefilterShader = shaderMap["prefilterShader"];
    prefilterShader.use();
    prefilterShader.setInt("environmentMap", 0);
    prefilterShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap->id);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;
    double begintime = glfwGetTime();
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth = 128 * std::pow(0.5, mip);
        unsigned int mipHeight = 128 * std::pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        prefilterShader.setFloat("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            prefilterShader.setMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, textureMap["prefilterMap"]->id, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            DrawCube();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //ibl-specular brdf
    // PBR: generate a 2D LUT from the BRDF equations used.
    // ----------------------------------------------------

    // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureMap["brdfLUTTex"]->id, 0);

    glViewport(0, 0, 512, 512);
    shaderMap["brdfShader"].use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawCube();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}