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

