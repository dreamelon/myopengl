#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "camera.h"
#include "mesh.h"
#include "model.h"
#include <memory>

class Application {

private:
    // Window
    static const unsigned int wndWidth = 1280;
    static const unsigned int wndHeight = 720;
    GLFWwindow* window;

    // Camera
    static Camera camera;

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // GUI
    bool show_demo_window = true;
    bool show_another_window = false;
    double uixpos, uiypos;
    int item_current = 0;
    int item = 0;
    int lightON = 0;
    bool showMonitor = true;

    // Input
    static float lastX;
    static float lastY;
    static bool firstMouse;
    static bool enterWindowFlag;

private:
    glm::mat4 view;
    glm::mat4 projection;

    typedef std::unique_ptr<Shader> Ptr;

    // Shaders
    Shader pbrShader;
    Shader equirectangularToCubemapShader;
    Shader backgroundShader;
    Shader irradianceShader;
    Shader prefilterShader;
    Shader brdfShader;

    // Models
    Model dragonModel;
    Sphere sphere = Sphere();

    Quad quad = Quad();
    Cube cube = Cube();

    // Textures
    Texture wallAlbedoMap;
    Texture wallNormalMap;
    Texture wallMetallicMap;
    Texture wallRoughnessMap;
    Texture wallAOMap;

    TextureCube envCubemap;
    TextureCube irradianceMap = TextureCube(32, 32);
    TextureCube prefilterMap = TextureCube(128, 128, true);
    Texture hdrTexture;
    Texture brdfLUTTexture;

public:
    void ShowMonitor(bool* p_open);

    bool Init();

    bool InitializeWindow();
    bool InitGLAD();
    void InitScene();
    void SetOpenGLState();

    void preBake();
    void Run();

    void RenderGUI();
    void SetupGUI();

public:
    void LoadModels();
    void LoadShaders();
    void LoadTextures();

public:
    // Input
    void processInput(GLFWwindow* window);

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void cursor_callback(GLFWwindow* window, int flag);
};
