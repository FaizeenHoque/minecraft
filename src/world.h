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

struct Chunk
{
    glm::vec3 position;
    std::vector<Block> blocks;
};

class World
{
public:
    std::vector<Block> blocks;
    std::vector<Chunk> chunks;

    void generate();
    bool isAir(int x, int y, int z) const;

private:
    siv::PerlinNoise perlin{123456u};

    static constexpr int CHUNK_SIZE = 16;
    static constexpr int WORLD_SIZE = CHUNK_SIZE * 3;
    static constexpr int WORLD_HEIGHT = 16;

    std::unordered_set<glm::ivec3, IVec3Hash, IVec3Equal> occupiedBlocks;
};

#endif