#ifndef BLOCK_CLASS_H
#define BLOCK_CLASS_H

#include <glm/glm.hpp>
#include "texture.h"

enum class BlockType
{
    Grass,
    Dirt,
    Stone
};

struct BlockTexture
{
    Texture *top;
    Texture *bottom;
    Texture *front;
    Texture *back;
    Texture *left;
    Texture *right;
};

struct Block
{
    BlockType type;
    BlockTexture textures;
};

#endif