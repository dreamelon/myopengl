#include "PBRApp.h"
#include <glad/glad.h>
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_opengl3.h"

void PBRApp::LoadResources() {
    LoadModels();
    LoadTextures();
    LoadShaders();
}

void PBRApp::LoadModels() {
    // load models
    // -----------
    dragonModel = std::make_unique<Model>("../Resources/dragon.obj");
}

void PBRApp::LoadShaders() {
    // build and compile shaders
    // -------------------------
    shaderMap["pbrShader"] = Shader("pbrShader", "shaders/pbrshader.vs", "shaders/pbrshader.fs");
    shaderMap["equirectangularToCubemapShader"] = Shader("equirectangularToCubemapShader", "shaders/cubeMap.vs", "shaders/equirectangular_to_cubeMap.fs");
    shaderMap["backgroundShader"] = Shader("backgroundShader", "shaders/background.vs", "shaders/background.fs");
    shaderMap["irradianceShader"] = Shader("irradianceShader", "shaders/cubeMap.vs", "shaders/irradiance_convolution.fs");
    shaderMap["prefilterShader"] = Shader("prefilterShader", "shaders/cubeMap.vs", "shaders/prefilter.fs");
    shaderMap["brdfShader"] = Shader("brdfShader", "shaders/brdf.vs", "shaders/brdf.fs");
    shaderMap["phong"] = Shader("phong", "shaders/phong.vs", "shaders/phong.fs");

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

void PBRApp::LoadTextures() {
    textureMap["wallAlbedoMap"] = std::make_unique<Texture>("wallAlbedoMap", "../Resources/PBR/rusted_iron/albedo.png", true);
    textureMap["wallNormalMap"] = std::make_unique<Texture>("wallNormalMap", "../Resources/PBR/rusted_iron/normal.png", false);
    textureMap["wallMetallicMap"] = std::make_unique<Texture>("wallMetallicMap", "../Resources/PBR/rusted_iron/metallic.png", false);
    textureMap["wallRoughnessMap"] = std::make_unique<Texture>("wallRoughnessMap", "../Resources/PBR/rusted_iron/roughness.png", false);
    textureMap["wallAOMap"] = std::make_unique<Texture>("wallAOMap", "../Resources/PBR/rusted_iron/ao.png", true);

    textureMap["equirectangularMap"] = std::make_unique<Texture>("equirectangularMap", "../Resources/IBL/newport_loft.hdr");

    textureMap["brdfLUTTex"] = std::make_unique<Texture>("brdfLUTTex");
    textureMap["irradianceMap"] = std::make_unique<TextureCube>("irradianceMap", 32, 32);
    textureMap["prefilterMap"] = std::make_unique<TextureCube>("prefilterMap", 128, 128, true);
    textureMap["envCubemap"] = std::make_unique<TextureCube>("envCubemap");
}

//void PBRApp::Run() {
//
//    LoadResources();
//    SetupGUI();
//    PreBake();
//    SetOpenGLState();
//    unsigned int sphereVAO = 0;
//    unsigned int indexCount;
//    // lights
//    // ------
//    glm::vec3 lightPositions[] = {
//        glm::vec3(-10.0f,  10.0f, 10.0f),
//        glm::vec3(10.0f,  10.0f, 10.0f),
//        glm::vec3(-10.0f, -10.0f, 10.0f),
//        glm::vec3(10.0f, -10.0f, 10.0f),
//    };
//    glm::vec3 lightColors[] = {
//        glm::vec3(300.0f, 300.0f, 300.0f),
//        glm::vec3(300.0f, 300.0f, 300.0f),
//        glm::vec3(300.0f, 300.0f, 300.0f),
//        glm::vec3(300.0f, 300.0f, 300.0f)
//    };
//
//    // render loop
//    // -----------
//    double lastTime = glfwGetTime();
//    int frames = 0;
//    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)wndWidth / (float)wndHeight, 0.1f, 100.0f);
//    while (!glfwWindowShouldClose(window))
//    {
//        RenderGUI();
//        // per-frame time logic
//        // --------------------
//
//        double currentFrame = glfwGetTime();
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//
//        // input
//        // -----
//        processInput(window);
//
//        // render
//        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//
//        glfwGetCursorPos(window, &uixpos, &uiypos);
//        // initialize static shader uniforms 
//        // --------------------------------------------------
//        auto& pbrShader = shaderMap["pbrShader"];
//        pbrShader.use();
//        pbrShader.setMat4("projection", projection);
//
//        glm::mat4 view = camera.GetViewMatrix();
//        pbrShader.setMat4("view", view);
//        pbrShader.setVec3("viewPos", camera.Position);
//
//        for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
//        {
//            glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
//            newPos = lightPositions[i];
//            pbrShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
//            pbrShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
//        }
//
//        auto& phongShader = shaderMap["phong"];
//        phongShader.use();
//        phongShader.setMat4("view", view);
//        phongShader.setMat4("projection", projection);
//        // dragon
//        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::translate(model, glm::vec3(-5.0, 0.0, 2.0));
//        phongShader.setMat4("model", model);
//        dragonModel->Draw();
//
//        // sphere
//        //model = glm::mat4(1.0f);
//        //model = glm::translate(model, glm::vec3(3.0, 0.0, 2.0));
//        //pbrShader.setMat4("model", model);
//        //sphere->Draw();
//
//        auto& backgroundShader = shaderMap["backgroundShader"];
//        backgroundShader.use();
//        backgroundShader.setMat4("projection", projection);
//        backgroundShader.setMat4("view", view);
//        cube->Draw();
//
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        double currTime = glfwGetTime();
//        frames++;
//        if (currTime - lastTime >= 1.0) {
//            // If last prinf() was more than 1 sec ago
//            // printf and reset timer
//            frames = 0;
//            lastTime += 1.0;
//        }
//
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // optional: de-allocate all resources once they've outlived their purpose:
//    // ------------------------------------------------------------------------
//
//    // glfw: terminate, clearing all previously allocated GLFW resources.
//    // ------------------------------------------------------------------
//    glfwTerminate();
//}

//void PBRApp::PreBake() {
//    unsigned int captureFBO;
//    unsigned int captureRBO;
//    glGenFramebuffers(1, &captureFBO);
//    glGenRenderbuffers(1, &captureRBO);
//
//    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
//    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
//
//    // PBR: load the HDR environment map
//    // ---------------------------------
//
//    // PBR: setup cubemap to render to and attach to framebuffer
//    // ---------------------------------------------------------
//
//    // PBR: set up projection and view matrices for capturing data onto the 6 cubemap face directions
//    // ----------------------------------------------------------------------------------------------
//    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
//    glm::mat4 captureViews[] =
//    {
//        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
//        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
//        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
//        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
//        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
//        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
//    };
//
//    // PBR: convert HDR equirectangular environment map to cubemap equivalent
//    // ----------------------------------------------------------------------
//    auto& equirectangularToCubemapShader = shaderMap["equirectangularToCubemapShader"];
//    equirectangularToCubemapShader.use();
//    equirectangularToCubemapShader.setInt("equirectangularMap", 0);
//    equirectangularToCubemapShader.setMat4("projection", captureProjection);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, textureMap["equirectangularMap"]->id);
//
//    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
//    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
//    auto& envCubemap = textureMap["envCubemap"];
//    for (unsigned int i = 0; i < 6; ++i)
//    {
//        equirectangularToCubemapShader.setMat4("view", captureViews[i]);
//        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap->id, 0);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        cube->Draw();
//    }
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap->id);
//    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
//
//    //ibl-diffuse
//    // PBR: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
//    // --------------------------------------------------------------------------------
//
//    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
//    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
//
//    // PBR: solve diffuse integral by convolution to create an irradiance (cube)map.
//    // -----------------------------------------------------------------------------
//    auto& irradianceShader = shaderMap["irradianceShader"];
//    irradianceShader.use();
//    irradianceShader.setInt("environmentMap", 0);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap->id);
//    irradianceShader.setMat4("projection", captureProjection);
//
//    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
//    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
//    for (unsigned int i = 0; i < 6; ++i)
//    {
//        irradianceShader.setMat4("view", captureViews[i]);
//        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, textureMap["irradianceMap"]->id, 0);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        cube->Draw();
//    }
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    //ibl-specular prefilter
//    //PBR: create a prefilter cubemap and re-scale captureFbp to pre-filter scale
//
//    //PBR: using quasi-montecarlo to simulate 
//    auto& prefilterShader = shaderMap["prefilterShader"];
//    prefilterShader.use();
//    prefilterShader.setInt("environmentMap", 0);
//    prefilterShader.setMat4("projection", captureProjection);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap->id);
//
//    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
//    unsigned int maxMipLevels = 5;
//    double begintime = glfwGetTime();
//    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
//    {
//        // reisze framebuffer according to mip-level size.
//        unsigned int mipWidth = 128 * std::pow(0.5, mip);
//        unsigned int mipHeight = 128 * std::pow(0.5, mip);
//        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
//        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
//        glViewport(0, 0, mipWidth, mipHeight);
//
//        float roughness = (float)mip / (float)(maxMipLevels - 1);
//        prefilterShader.setFloat("roughness", roughness);
//        for (unsigned int i = 0; i < 6; ++i)
//        {
//            prefilterShader.setMat4("view", captureViews[i]);
//            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, textureMap["prefilterMap"]->id, mip);
//
//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//            cube->Draw();
//        }
//    }
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    //ibl-specular brdf
//    // PBR: generate a 2D LUT from the BRDF equations used.
//    // ----------------------------------------------------
//
//    // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
//    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
//    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureMap["brdfLUTTex"]->id, 0);
//
//    glViewport(0, 0, 512, 512);
//    shaderMap["brdfShader"].use();
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    quad->Draw();
//
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}

void PBRApp::SetOpenGLState() {
    // configure global OpenGL state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glViewport(0, 0, wndWidth, wndHeight);
}