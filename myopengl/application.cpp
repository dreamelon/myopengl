#include "application.h"
#include <glad/glad.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !hdrKeyPressed)
    {
        hdr = !hdr;
        hdrKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        hdrKeyPressed = false;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        if (exposure > 0.0f)
            exposure -= 0.01f;
        else
            exposure = 0.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        exposure += 0.01f;
    }
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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

void Application::RenderGUI() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //if (show_demo_window)
    //ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Panel");
        ShowMonitor(&showMonitor);
        ImGui::RadioButton("directlight", &lightopen, 0);
        ImGui::SameLine();
        ImGui::RadioButton("ambient", &lightopen, 1);

        //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        //ImGui::Checkbox("Another Window", &show_another_window);

        //ImGui::SliderFloat("viewField", &viewField, 0.0f, 90.0f, "viewField = %.3f");
        ImGui::SliderFloat("VIEWFIELD", &camera.Zoom, 0.0f, 90.0f, "viewField = %.3f");

        ImGui::Separator();

        ImGui::SliderFloat("Roughness", &Roughness, 0.0f, 1.0f, "Roughness = %.3f");
        ImGui::SliderFloat("Ao", &Ao, 0.0f, 1.0f, "Ao = %.3f");
        ImGui::SliderFloat("Metallic", &Metallic, 0.0f, 1.0f, "Metallic = %.3f");
        ImGui::ColorEdit3("Albedo", (float*)&Albedo); // Edit 3 floats representing a color

        ImGui::Separator();

        {
            const char* items[] = {
                "gold",
                "grass",
                "plastic",
                "wall",
            };
            ImGui::Combo("Material", &item_current, items, IM_ARRAYSIZE(items));
        }

        ImGui::Separator();

        {
            const char* items[] = {
                "dragon",
                "sphere",
                "cube"
            };
            ImGui::Combo("OBJ", &item, items, IM_ARRAYSIZE(items));
        }

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
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
            //std::cout << "uixpos  " << uixpos << "\n" << "uiypos  " << uiypos << std::endl;
        }
        ImGui::End();
    }
}

bool Application::InitScene(){
    // load models
    // -----------
     Model ourModel("Resources/dragon.obj");
}

void Application::LoadModels() {

}

void Application::LoadShaders() {
    // build and compile shaders
    // -------------------------
    Shader ourShader("shaders/pbrshader.vs", "shaders/pbrshader.fs");
    Shader pbrShader("shaders/dragonshader.vs", "shaders/dragonshader.fs");
    Shader modelShader("shaders/model_shader.vs", "shaders/model_shader.fs");
    Shader equirectangularToCubemapShader("shaders/cubeMap.vs", "shaders/equirectangular_to_cubeMap.fs");
    Shader backgroundShader("shaders/background.vs", "shaders/background.fs");
    Shader irradianceShader("shaders/cubeMap.vs", "shaders/irradiance_convolution.fs");
    Shader prefilterShader("shaders/cubeMap.vs", "shaders/prefilter.fs");
    Shader brdfShader("shaders/brdf.vs", "shaders/brdf.fs");
    //Shader quadShader("shaders/quadshader.vs", "shaders/quadshader.fs");
    Shader reflectionShader("shaders/reflectshader.vs", "shaders/reflectshader.fs");
    Shader refractionShader("shaders/refractshader.vs", "shaders/refractshader.fs");
    Shader phongShader("shaders/blinn-phong.vs", "shaders/blinn-phong.fs");

    ourShader.use();

    ourShader.setInt("irradianceMap", 0);
    ourShader.setInt("prefilterMap", 1);
    ourShader.setInt("brdfLUT", 2);
    ourShader.setInt("albedoMap", 3);
    ourShader.setInt("normalMap", 4);
    ourShader.setInt("metallicMap", 5);
    ourShader.setInt("roughnessMap", 6);
    ourShader.setInt("aoMap", 7);
    ourShader.setInt("skybox", 8);

    //ourShader.setVec3("albedo", 0.98, 0.97, 0.95);
    //ourShader.setFloat("ao", 1.0f);

    glm::vec3 Albedo(0.5f, 0.0f, 0.0f);
    GLfloat Ao(0.5f);
    GLfloat Metallic(0.3f);
    GLfloat Roughness(0.6f);
    pbrShader.use();
    pbrShader.setInt("irradianceMap", 0);
    pbrShader.setInt("prefilterMap", 1);
    pbrShader.setInt("brdfLUT", 2);
    pbrShader.setInt("skybox", 3);
    phongShader.use();
    phongShader.setInt("floorTexture", 0);
    phongShader.setInt("normalMap", 1);
}

void Application::LoadTextures() {
    unsigned int albedo = loadTexture("../Resources/PBR/rusted_iron/albedo.png", true);
    unsigned int normal = loadTexture("../Resources/PBR/rusted_iron/normal.png", false);
    unsigned int metallic = loadTexture("../Resources/PBR/rusted_iron/metallic.png", false);
    unsigned int roughness = loadTexture("../Resources/PBR/rusted_iron/roughness.png", false);
    unsigned int ao = loadTexture("../Resources/PBR/rusted_iron/ao.png", true);

    // grass
    unsigned int grassAlbedoMap = loadTexture("../Resources/PBR/grass/albedo.png", true);
    unsigned int grassNormalMap = loadTexture("../Resources/PBR/grass/normal.png", false);
    unsigned int grassMetallicMap = loadTexture("../Resources/PBR/grass/metallic.png", false);
    unsigned int grassRoughnessMap = loadTexture("../Resources/PBR/grass/roughness.png", false);
    unsigned int grassAOMap = loadTexture("../Resources/PBR/grass/ao.png", true);;

    // wall
    unsigned int wallAlbedoMap = loadTexture("../Resources/PBR/wall/albedo.png", true);
    unsigned int wallNormalMap = loadTexture("../Resources/PBR/wall/normal.png", false);
    unsigned int wallMetallicMap = loadTexture("../Resources/PBR/wall/metallic.png", false);
    unsigned int wallRoughnessMap = loadTexture("../Resources/PBR/wall/roughness.png", false);
    unsigned int wallAOMap = loadTexture("../Resources/PBR/wall/ao.png", true);
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