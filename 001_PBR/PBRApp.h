#pragma once
#include "application.h"

class PBRApp : public Application {

public:
    virtual void Run();

protected:
    void PreBake();

    virtual void SetOpenGLState();
    virtual void LoadResources();
    virtual void LoadModels();
    virtual void LoadShaders();
    virtual void LoadTextures();

protected:
    std::unique_ptr<Sphere> sphere;
};