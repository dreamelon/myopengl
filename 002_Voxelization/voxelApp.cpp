#include "voxelApp.h"
#include <glad/glad.h>

void VoxelApp::LoadModels() {
    // load models
    // -----------
    modelMap["dragonModel"] = std::make_unique<Model>("dragonModel", "../Resources/dragon.obj");
}

void VoxelApp::LoadShaders() {
    // build and compile shaders
    // -------------------------
    shaderMap["voxelization"] = Shader("voxelizationShader", "shaders/voxelization.vert", "shaders/voxelization.frag", "shaders/voxelization.geom");
    shaderMap["backgroundShader"] = Shader("backgroundShader", "shaders/background.vs", "shaders/background.fs");
    auto& backgroundShader = shaderMap["backgroundShader"];
    backgroundShader.use();
    backgroundShader.setMat4("view", view);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureMap["envCubemap"]->id);
}

void VoxelApp::LoadTextures() {
    textureMap["envCubemap"] = std::make_unique<TextureCube>("envCubemap");
}

void VoxelApp::SetOpenGLState() {
    // configure global OpenGL state
    // -----------------------------
    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glViewport(0, 0, wndWidth, wndHeight);
}

void VoxelApp::LoadResources() {
    LoadModels();
    LoadTextures();
    LoadShaders();
}

void VoxelApp::Run() {
    LoadResources();
    SetupGUI();
    // SetOpenGLState();

    unsigned int resolution = 256;
    unsigned int size = resolution * resolution * resolution;
    unsigned int cntBuffer;
    glGenBuffers(1, &cntBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, cntBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size * sizeof(int), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, cntBuffer);

    glm::mat4 view = camera.GetViewMatrix();
    float length = resolution * 0.51f;
    glm::mat4 projection = glm::ortho(-length, +length, -length, +length, 0.1f, resolution * 1.2f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.0, 0.0, 2.0));

    auto& voxelShader = shaderMap["voxelization"];
    voxelShader.use();
    voxelShader.setMat4("view", view);
    voxelShader.setMat4("model", model);
    voxelShader.setMat4("projection", projection);

    // configure global OpenGL state
    // -----------------------------
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.2f, 0.12f, 1.0f);
    glViewport(0, 0, wndWidth, wndHeight);

    modelMap["dragonModel"]->Draw();

    // render loop
    // -----------
    double lastTime = glfwGetTime();
    int frames = 0;
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)wndWidth / (float)wndHeight, 0.1f, 100.0f);
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        glClearColor(0.1f, 0.2f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glfwGetCursorPos(window, &uixpos, &uiypos);
        // initialize static shader uniforms 
        // --------------------------------------------------
        auto& pbrShader = shaderMap["pbrShader"];
        pbrShader.use();
        pbrShader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        pbrShader.setMat4("view", view);
        // dragon
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.0, 0.0, 2.0));
        modelMap["dragonModel"]->Draw();

        auto& backgroundShader = shaderMap["backgroundShader"];
        backgroundShader.use();
        backgroundShader.setMat4("projection", projection);
        backgroundShader.setMat4("view", view);
        DrawCube();

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