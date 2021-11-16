#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "camera.h"
#include "mesh.h"
#include "model.h"
#include <memory>
#include <set>
#include <map>
#include <unordered_map>
class Application {

private:
    // Window
    static const unsigned int wndWidth = 1280;
    static const unsigned int wndHeight = 720;
    GLFWwindow* window;

    // Camera
    static Camera camera;

    // timing
    double deltaTime = 0.0;
    double lastFrame = 0.0;

    // GUI
    bool show_demo_window = true;
    bool show_another_window = false;
    double uixpos, uiypos;
    int item_current = 0;
    int item = 0;
    int lightON = 0;
    bool showMonitor = true;

    // Input
    static double lastX;
    static double lastY;
    static bool firstMouse;
    static bool enterWindowFlag;

private:
    glm::mat4 view;
    glm::mat4 projection;

    typedef std::unique_ptr<Shader> Ptr;

    // Shaders
    //Shader pbrShader;
    //Shader equirectangularToCubemapShader;
    //Shader backgroundShader;
    //Shader irradianceShader;
    //Shader prefilterShader;
    //Shader brdfShader;
    std::unordered_map<std::string, Shader> shaderMap;

    // Models
    std::unique_ptr<Model> dragonModel;

    typedef std::unique_ptr<Mesh> MeshPtr;
    MeshPtr sphere;
    MeshPtr quad;
    MeshPtr cube;

    // Textures
    typedef std::unique_ptr<Texture> TexturePtr;
    TexturePtr wallAlbedoMap;
    TexturePtr wallNormalMap;
    TexturePtr wallMetallicMap;
    TexturePtr wallRoughnessMap;
    TexturePtr wallAOMap;

    TexturePtr equirectangularMap;
    TexturePtr brdfLUTTexture;
    std::unique_ptr<TextureCube> envCubemap;
    std::unique_ptr<TextureCube> irradianceMap;
    std::unique_ptr<TextureCube> prefilterMap;


public:
    void ShowMonitor(bool* p_open);

    bool Init();

    bool InitializeWindow();
    bool InitGLAD();
    void LoadResources();
    void SetOpenGLState();

    void PreBake();
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
