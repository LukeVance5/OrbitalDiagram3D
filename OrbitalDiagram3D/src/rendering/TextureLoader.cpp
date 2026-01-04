#include <stb/stb_image.h>
#include "TextureLoader.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

unsigned int TextureLoader::LoadTexture(const std::string& filePath)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    if (!data)
    {
        std::cout << "Failed to load texture: " << filePath << std::endl;
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);

    GLenum format = GL_RGB;
    GLenum internalFormat = GL_RGB8;

    if (nrChannels == 1)
    {
        format = GL_RED;
        internalFormat = GL_R8;
    }
    else if (nrChannels == 3)
    {
        format = GL_RGB;
        internalFormat = GL_RGB8;
    }
    else if (nrChannels == 4)
    {
        format = GL_RGBA;
        internalFormat = GL_RGBA8;
    }

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        internalFormat,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data
    );

    // 🔑 SWIZZLING FOR GRAYSCALE TEXTURES
    if (format == GL_RED)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE);
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return textureID;
}

unsigned int TextureLoader::LoadTextureCube(const std::vector<std::string>& filePaths) {
	unsigned int textureID;
	const unsigned int NUM_CUBE_FACES = 6;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	for (unsigned int i = 0; i < NUM_CUBE_FACES; i++)
	{	
		std::cout << "Loading cubemap face: " << filePaths[i] << std::endl;
		stbi_set_flip_vertically_on_load(false);
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filePaths[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{	
			GLenum format = GL_RGB;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3)
				format = GL_RGB;
			else if (nrChannels == 4)
				format = GL_RGBA;
			
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				format,
				width,
				height,
				0,
				format,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << filePaths[i] << std::endl;
			stbi_image_free(data);
		}
	}

	return textureID;
}