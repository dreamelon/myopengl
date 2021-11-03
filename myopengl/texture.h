#pragma once

#include <string>

class Texture
{
public:
    Texture(const char *path, bool gamma = false);

private:
    unsigned int textureID;
};