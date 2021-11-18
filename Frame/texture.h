#pragma once

#include <string>

class Texture
{
public:
    Texture(const char* texName, const char *path, bool gamma);
    Texture(const char* texName, const char *path);
    Texture(const char* texName = "");

public:
    unsigned int id;
    std::string name;
};

class TextureCube : public Texture{
public:
    TextureCube(const char* texName, unsigned int width = 512, unsigned int height = 512, bool mipmap = false);
};

