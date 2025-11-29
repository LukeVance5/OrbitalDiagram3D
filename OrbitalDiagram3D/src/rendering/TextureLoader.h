#pragma once
#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include <string>
class TextureLoader {
public:
    static unsigned int LoadTexture(const std::string& filePath);
};
#endif