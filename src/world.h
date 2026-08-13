#ifndef WORLD_CLASS_H
#define WORLD_CLASS_H

#include "perlin_noise.hpp"
#include <glm/glm.hpp>

enum class BlockType
{
    Dirt,
    Stone
};

struct Block
{
    glm::vec3 position;
    BlockType type;
};

class World
{
public:
    std::vector<Block> blocks;
    void generate();
};

#endif