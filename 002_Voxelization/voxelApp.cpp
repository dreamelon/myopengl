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
    shaderMap["voxelization"] = Shader("voxelization", "shaders/voxelization.vert", "shaders/voxelization.frag", "shaders/voxelization.geom");
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

    unsigned int resolution = 512;
    unsigned int size = resolution * resolution * resolution;
    unsigned int cntBuffer;
    glGenBuffers(1, &cntBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, cntBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size / 8, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, cntBuffer);

    int* writePtr = reinterpret_cast<int*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY));
    for (int x = 0; x < size / 32; ++x)
    {
        writePtr[x] = 0;
    }

    if (!glUnmapBuffer(GL_SHADER_STORAGE_BUFFER))
        std::cout << "unMap error\n" << std::endl;

    float halfRes = resolution / 2;
    glm::mat4 viewZ = glm::lookAt(glm::vec3(0, 0, halfRes), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
    glm::mat4 viewY = glm::lookAt(glm::vec3(0, halfRes, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));
    glm::mat4 viewX = glm::lookAt(glm::vec3(halfRes, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));

    glm::mat4 projection = glm::ortho(-halfRes, +halfRes, -halfRes, +halfRes, -halfRes, halfRes);
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(-100, -100, -100));
    model = glm::scale(model, glm::vec3(100));

    auto& voxelShader = shaderMap["voxelization"];
    voxelShader.use();
    voxelShader.setMat4("view[0]", viewX);
    voxelShader.setMat4("view[1]", viewY);
    voxelShader.setMat4("view[2]", viewZ);
    voxelShader.setMat4("model", model);
    voxelShader.setMat4("projection", projection);
    voxelShader.setInt("resolution", resolution);
    // configure global OpenGL state
    // -----------------------------
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(0.1f, 0.2f, 0.12f, 1.0f);
    glViewport(0, 0, resolution, resolution);
    glDisable(GL_CULL_FACE);
    modelMap["dragonModel"]->Draw();
    // access violation and dont know reason
    //glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, cntBuffer);
    int* ptr = (int*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    std::vector<glm::vec3> pos;

    if (ptr) {
        for (int i = 0; i < size / 32; i++) {
            for (int indexInInt = 0; indexInInt < 32; indexInInt++) {
                if ((*(ptr + i) & (1<<indexInInt)) != 0) {
                    int indexInVoxel = i * 32 + indexInInt;
                    int z = indexInVoxel / (resolution * resolution) - halfRes;
                    int y = (indexInVoxel % (resolution * resolution)) / resolution - halfRes;
                    int x = (indexInVoxel % (resolution * resolution)) % resolution - halfRes;
                    pos.push_back(glm::vec3(x, y, z));
                }
            }
        }
    }

    //if (ptr) {
    //    for (int i = 0; i < size; i++) {
    //        if (*(ptr + i)) {
    //            int z = i / (resolution * resolution) - halfRes;
    //            int y = (i % (resolution * resolution)) / resolution - halfRes;
    //            int x = (i % (resolution * resolution)) % resolution - halfRes;
    //            pos.push_back(glm::vec3(x, y, z));
    //        }
    //    }
    //}

    std::vector<glm::mat4> inst;
    for (unsigned int x = 0; x < pos.size(); ++x)
    {
        glm::mat4 transf(1.0f);
        transf = glm::translate(transf, pos[x]);
        transf = glm::scale(transf, glm::vec3(0.5));
        inst.push_back(transf);
    }

    cube = std::make_unique<InstanceCube>();
    cube->SetupInstanceData(inst);

    //while (!glfwWindowShouldClose(window))
    //{
    //    // per-frame time logic
    //    // --------------------
    //    double currTime= glfwGetTime();
    //    deltaTime = currTime - lastFrame;
    //    lastFrame = currTime;
    //    // input
    //    // -----
    //    processInput(window);

    //    // render
    //    glClearColor(0.1f, 0.2f, 0.12f, 1.0f);
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //    //model = glm::translate(model, glm::vec3(0.0, -0.8, 0.0));
    //    //voxelShader.setMat4("model", model);
    //    voxelShader.setMat4("viewX", viewX);
    //    voxelShader.setMat4("viewY", viewY);
    //    voxelShader.setMat4("viewZ", viewZ);
    //    modelMap["dragonModel"]->Draw();
    //    //cube->Draw();

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

    auto a = glm::vec4(0, 0, 0, 1);
    auto a1 = glm::vec4(-10, 121331, 0, 1);
    auto b = projection * viewZ * a;
    auto b1 = projection * viewZ * a1;
    auto c = projection * viewZ * model * a;
    auto c1 = projection * viewZ * model * a1;

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