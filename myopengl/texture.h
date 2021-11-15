#pragma once

#include <string>

class Texture
{
public:
    Texture(const char *path, bool gamma);
    Texture(const char *path);
    Texture();

public:
    unsigned int id;
};

class TextureCube : public Texture{
public:
    TextureCube(unsigned int width = 512, unsigned int height = 512, bool mipmap = false);
};
