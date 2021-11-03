#pragma once
#include <GLFW/glfw3.h>
#include "camera.h"
#include "mesh.h"
#include "model.h"

class Application {
private:
    // settings
    const unsigned int SCR_WIDTH = 1280;
    const unsigned int SCR_HEIGHT = 720;

    // camera
    Camera camera;
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    bool enterWindowFlag = true;

    int item_current = 0;
    int item = 0;

    int lightopen = 0;

    bool showMonitor = true;

    double uixpos, uiypos;

    GLFWwindow* window;

private:
    glm::mat4 view;
    glm::mat4 projection;


public:
    void ShowMonitor(bool* p_open);

    bool Init();

    bool InitializeWindow();
    bool InitGLAD();
    bool InitScene();

    int Run();

    void RenderGUI();

    void SetupGUI();

public:
    void LoadModels();
    void LoadShaders();
    void LoadTextures();

public:
    // Input
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void processInput(GLFWwindow* window);
    static void cursor_callback(GLFWwindow* window, int flag);
};
