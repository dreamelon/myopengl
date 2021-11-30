#include "voxelApp.h"
#include <glad/glad.h>
#include <iostream>

void VoxelApp::LoadModels() {
    // load models
    // -----------
    modelMap["dragonModel"] = std::make_unique<Model>("dragonModel", "../Resources/dragon.obj");
}

void VoxelApp::LoadShaders() {
    // build and compile shaders
    // -------------------------
    shaderMap["voxelization"] = Shader("voxelization", "shaders/voxelization.vert", "shaders/voxelization.frag");
    shaderMap["voxelVisual"] = Shader("voxelVisual", "shaders/voxelVisual.vert", "shaders/voxelVisual.frag");
    shaderMap["backgroundShader"] = Shader("backgroundShader", "shaders/background.vert", "shaders/background.frag");
    auto& backgroundShader = shaderMap["backgroundShader"];
    backgroundShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureMap["envCubemap"]->id);
}

void VoxelApp::LoadTextures() {
    std::vector<std::string> paths = {
        "../Resources/SkyBox/skybox/right.jpg",
        "../Resources/SkyBox/skybox/left.jpg",
        "../Resources/SkyBox/skybox/top.jpg",
        "../Resources/SkyBox/skybox/bottom.jpg",
        "../Resources/SkyBox/skybox/front.jpg",
        "../Resources/SkyBox/skybox/back.jpg",
    };
    textureMap["envCubemap"] = std::make_unique<TextureCube>("envCubemap", paths);
}

void VoxelApp::SetOpenGLState() {
    // configure global OpenGL state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

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
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, cntBuffer);

    int* writePtr = reinterpret_cast<int*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY));
    for (int x = 0; x < size; ++x)
    {
        writePtr[x] = 0;
    }

    if (!glUnmapBuffer(GL_SHADER_STORAGE_BUFFER))
        std::cout << "unMap error\n" << std::endl;

    glm::mat4 view = camera.GetViewMatrix();
    float length = resolution * 0.51f;
    glm::mat4 projection = glm::ortho(-length, +length, -length, +length, 0.1f, resolution * 1.2f);
    glm::mat4 model = glm::mat4(1);
    //model = glm::scale(model, glm::vec3(100));
    model = glm::translate(model, glm::vec3(0.0, -0.8, 0));

    glm::vec4 a(-1, 1, 1, 1);
    auto b = projection * view * model * a;

    auto& voxelShader = shaderMap["voxelization"];
    voxelShader.use();
    voxelShader.setMat4("view", view);
    voxelShader.setMat4("model", model);
    voxelShader.setMat4("projection", projection);
    voxelShader.setInt("resolution", resolution);
    voxelShader.setVec3("boxMin", glm::vec3(-length));
    // configure global OpenGL state
    // -----------------------------
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.2f, 0.12f, 1.0f);
    glViewport(0, 0, resolution, resolution);

    modelMap["dragonModel"]->Draw();
    // access violation and dont know reason
    //glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, cntBuffer);
    int* ptr = (int*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    std::vector<glm::vec3> pos;
    std::vector<int> index;
    //resolution = resolution / 2;
    int halfRes = resolution / 2;
    if (ptr) {
        for (int i = 0; i < size; i++) {
            if (*(ptr+i)) {
                int z = i / (resolution * resolution) - halfRes;
                int y = (i % (resolution * resolution)) / resolution - halfRes;
                int x = (i % (resolution * resolution)) % resolution - halfRes;
                pos.push_back(glm::vec3(x, y, z));
                index.push_back(i);
            }
        }
    }

    std::vector<glm::mat4> inst;
    for (unsigned int x = 0; x < pos.size(); ++x)
    {
        glm::mat4 transf(1.0f);
        transf = glm::scale(transf, glm::vec3(0.25));
        transf = glm::translate(transf, pos[x]);
        inst.push_back(transf);
    }

    cube = std::make_unique<InstanceCube>();
    cube->SetupInstanceData(inst);

    //while (!glfwWindowShouldClose(window))
    //{
    //    // input
    //    // -----
    //    processInput(window);

    //    // render
    //    glClearColor(0.1f, 0.2f, 0.12f, 1.0f);
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //    //model = glm::translate(model, glm::vec3(0.0, -0.8, 0.0));
    //    //voxelShader.setMat4("model", model);
    //    //modelMap["dragonModel"]->Draw();

    //    //model = glm::mat4(1);
    //    //voxelShader.setMat4("model", model);

    //    //view = camera.GetViewMatrix();
    //    //voxelShader.setMat4("view", view);
    //    cube->Draw();
    //    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    //    // -------------------------------------------------------------------------------
    //    glfwSwapBuffers(window);
    //    glfwPollEvents();
    //}

    // render loop
    // -----------
    double lastTime = glfwGetTime();
    int frames = 0;
    glm::mat4 perspectivepProj = glm::perspective(glm::radians(camera.Zoom), (float)wndWidth / (float)wndHeight, 0.1f, 1000.0f);

    //SetOpenGLState();
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, wndWidth, wndHeight);
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
        auto& voxelVisual = shaderMap["voxelVisual"];
        voxelVisual.use();
        voxelVisual.setMat4("projection", perspectivepProj);
        glm::mat4 view = camera.GetViewMatrix();
        voxelVisual.setMat4("view", view);
        // dragon
        // first scale then rotation, translation
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0, 0.0, 100));
        voxelVisual.setMat4("model", model);
        //cube->Draw();
        cube->DrawInstance(pos.size());

        model = glm::mat4(1);
        voxelVisual.setMat4("model", model);
        //modelMap["dragonModel"]->Draw();

        glDepthFunc(GL_LEQUAL);
        auto& backgroundShader = shaderMap["backgroundShader"];
        backgroundShader.use();
        backgroundShader.setMat4("projection", perspectivepProj);
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