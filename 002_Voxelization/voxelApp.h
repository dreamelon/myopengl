#pragma once
#include "application.h"

class VoxelApp : public Application {
public:
    virtual void Run();

protected:

    virtual void SetOpenGLState();
    virtual void LoadResources();
    virtual void LoadModels();
    virtual void LoadShaders();
    virtual void LoadTextures();
};