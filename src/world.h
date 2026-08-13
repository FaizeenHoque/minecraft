#ifndef WORLD_CLASS_H
#define WORLD_CLASS_H

#include "block.h"
#include "perlin_noise.hpp"
#include <glm/glm.hpp>

class World
{
public:
    std::vector<Block> blocks;
    void generate();
};

#endif