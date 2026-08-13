#ifndef WORLD_CLASS_H
#define WORLD_CLASS_H

#include "block.h"
#include <vector>
#include <unordered_set>

class World
{
public:
    std::vector<Block> blocks;

    void generate();
    bool isAir(int x, int y, int z) const;
};

#endif