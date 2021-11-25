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

void DrawCube();
void DrawQuad();
void DrawSphere();
void SetupMesh(unsigned int& VAO, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
void DrawInstanceCube(unsigned int count);


class Application {

 protected:
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

protected:

    typedef std::unique_ptr<Shader> Ptr;

    // Shaders
    std::unordered_map<std::string, Shader> shaderMap;

    // Models
    std::unordered_map<std::string, std::unique_ptr<Model>> modelMap;

    // Textures
    typedef std::unique_ptr<Texture> TexturePtr;
    std::unordered_map<std::string, TexturePtr> textureMap;

public:
    void ShowMonitor(bool* p_open);

    bool Init();

    bool InitializeWindow();
    bool InitGLAD();

    void RenderGUI();
    void SetupGUI();

public:
    virtual void Run() = 0;

protected:

    virtual void SetOpenGLState() = 0;
    virtual void LoadResources() = 0;
    virtual void LoadModels() = 0;
    virtual void LoadShaders() = 0;
    virtual void LoadTextures() = 0;

public:
    // Input
    void processInput(GLFWwindow* window);

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void cursor_callback(GLFWwindow* window, int flag);
};
