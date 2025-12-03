#pragma once
#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include <string>
#include <vector>
#include "OrbitalDiagram3DExport.h"
class OD_API TextureLoader {
public:
    static unsigned int LoadTexture(const std::string& filePath);
    static unsigned int LoadTextureCube(const std::vector<std::string>& filePaths);
};
#endif