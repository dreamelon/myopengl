#pragma once

#include <string>

class Texture
{
public:
    Texture(const char *path, bool gamma = false, bool hdr = false);
    Texture();

public:
    unsigned int id;
};

class TextureCube : public Texture{
public:
    TextureCube(unsigned int width = 512, unsigned int height = 512, bool mipmap = false);
};

