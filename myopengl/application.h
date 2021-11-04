#pragma once
#include <GLFW/glfw3.h>
#include "camera.h"
#include "mesh.h"
#include "model.h"

class Application {

private:
    // Window
    const unsigned int wndWidth = 1280;
    const unsigned int wndHeight = 720;
    GLFWwindow* window;

    // Camera
    static Camera camera;

    // Input
    float lastX = wndWidth / 2.0f;
    float lastY = wndHeight / 2.0f;
    bool firstMouse = true;

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;



    double uixpos, uiypos;

    // GUI
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool enterWindowFlag = true;
    int item_current = 0;
    int item = 0;
    int lightON = 0;
    bool showMonitor = true;

private:
    glm::mat4 view;
    glm::mat4 projection;


public:
    void ShowMonitor(bool* p_open);

    bool Init();

    bool InitializeWindow();
    bool InitGLAD();
    bool InitScene();
    void SetOpenGLState();

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
