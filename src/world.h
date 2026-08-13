#ifndef WORLD_CLASS_H
#define WORLD_CLASS_H

#include "block.h"
#include <vector>
#include "perlin_noise.hpp"
#include <unordered_set>

struct IVec3Hash
{
    std::size_t operator()(const glm::ivec3 &position) const noexcept
    {
        std::size_t hash = std::hash<int>{}(position.x);
        hash ^= std::hash<int>{}(position.y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= std::hash<int>{}(position.z) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    }
};

struct IVec3Equal
{
    bool operator()(const glm::ivec3 &a, const glm::ivec3 &b) const noexcept
    {
        return a.x == b.x &&
               a.y == b.y &&
               a.z == b.z;
    }
};

class World
{
public:
    std::vector<Block> blocks;

    void generate();
    bool isAir(int x, int y, int z) const;

private:
    siv::PerlinNoise perlin{123456u};

    static constexpr int WORLD_SIZE = 50;
    static constexpr int WORLD_HEIGHT = 60;

    std::unordered_set<glm::ivec3, IVec3Hash, IVec3Equal> occupiedBlocks;
};

#endif