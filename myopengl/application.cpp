#include <glad/glad.h>
#include "application.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

bool Application::firstMouse = true;
bool Application::enterWindowFlag = true;

float Application::lastX = wndWidth / 2.0f;
float Application::lastY = wndHeight / 2.0f;

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

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

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

void Application::InitScene() {
    LoadModels();
    LoadShaders();
    LoadTextures();
}

void Application::LoadModels() {
    // load models
    // -----------
    dragonModel = Model("Resources/dragon.obj");
    cube.setupMesh();
    sphere.setupMesh();
    quad.setupMesh();
}

void Application::LoadShaders() {
    // build and compile shaders
    // -------------------------
    pbrShader                       =   Shader("shaders/pbrshader.vs", "shaders/pbrshader.fs");
    equirectangularToCubemapShader  =   Shader("shaders/cubeMap.vs", "shaders/equirectangular_to_cubeMap.fs");
    backgroundShader                =   Shader("shaders/background.vs", "shaders/background.fs");
    irradianceShader                =   Shader("shaders/cubeMap.vs", "shaders/irradiance_convolution.fs");
    prefilterShader                 =   Shader("shaders/cubeMap.vs", "shaders/prefilter.fs");
    brdfShader                      =   Shader("shaders/brdf.vs", "shaders/brdf.fs");

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
}

void Application::LoadTextures() {
    wallAlbedoMap    = Texture("../Resources/PBR/wall/albedo.png", true);
    wallNormalMap    = Texture("../Resources/PBR/wall/normal.png", false);
    wallMetallicMap  = Texture("../Resources/PBR/wall/metallic.png", false);
    wallRoughnessMap = Texture("../Resources/PBR/wall/roughness.png", false);
    wallAOMap        = Texture("../Resources/PBR/wall/ao.png", true);

    hdrTexture       = Texture("../Resources/IBL/newport_loft.hdr", false, true);
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
        ShowMonitor(&showMonitor);

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
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            enterWindowFlag = false;
        }

        else
        {
            enterWindowFlag = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
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
    // render loop
    // -----------
    double lastTime = glfwGetTime();
    int frames = 0;
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------

        float currentFrame = glfwGetTime();
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
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)wndWidth / (float)wndHeight, 0.1f, 100.0f);

        backgroundShader.use();
        backgroundShader.setMat4("projection", projection);
        pbrShader.use();
        pbrShader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        pbrShader.setMat4("view", view);
        pbrShader.setVec3("viewPos", camera.Position);

        glActiveTexture(GL_TEXTURE8);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap.id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap.id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap.id);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture.id);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.0, 0.0, 2.0));
        pbrShader.setMat4("model", model);

        // sphere
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, wallAlbedoMap.id);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, wallNormalMap.id);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, wallMetallicMap.id);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, wallRoughnessMap.id);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, wallAOMap.id);
        dragonModel.Draw();

        // dragon
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0, 0.0, 2.0));
        pbrShader.setMat4("model", model);
        sphere.Draw();

        backgroundShader.use();
        backgroundShader.setMat4("view", view);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap.id);
        cube.Draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        double currTime = glfwGetTime();
        frames++;
        if (currTime - lastTime >= 1.0) {
            // If last prinf() was more than 1 sec ago
            // printf and reset timer
            //printf("%f ms/frame\n", 1000.0 / double(nbFrames));
            //printf("%d fps\n", nbFrames);
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

void Application::preBake() {
    //环境光大多超过1.0范围，需要带浮点颜色缓冲的帧缓冲
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
    // ----------------------------------------------------------------------5
    equirectangularToCubemapShader.use();
    equirectangularToCubemapShader.setInt("equirectangularMap", 0);
    equirectangularToCubemapShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture.id);

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        equirectangularToCubemapShader.setMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap.id, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cube.Draw();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap.id);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    //ibl-diffuse部分
    // PBR: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
    // --------------------------------------------------------------------------------

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    // PBR: solve diffuse integral by convolution to create an irradiance (cube)map.
    // -----------------------------------------------------------------------------
    irradianceShader.use();
    irradianceShader.setInt("environmentMap", 0);
    irradianceShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap.id);

    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        irradianceShader.setMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap.id, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cube.Draw();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //ibl-specular prefilter
    //PBR: create a prefilter cubemap and re-scale captureFbp to pre-filter scale

    //PBR: 使用一个quasi-montecarlo(拟蒙特卡洛方法）模拟环境光创造
    prefilterShader.use();
    prefilterShader.setInt("environmentMap", 0);
    prefilterShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap.id);

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
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap.id, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            cube.Draw();
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
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture.id, 0);

    glViewport(0, 0, 512, 512);
    brdfShader.use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    quad.Draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}