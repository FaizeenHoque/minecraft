#include "world.h"

void World::generate()
{
    for (int x = 0; x < WORLD_SIZE; x++)
    {
        for (int z = 0; z < WORLD_SIZE; z++)
        {
            double noise = perlin.octave2D_01(
                x * 0.05,
                z * 0.05,
                4);

            int terrainHeight = 20 + noise * 30;

            for (int y = 0; y < terrainHeight; y++)
            {
                Block block;
                block.position = glm::ivec3(x, y, z);

                if (y == 0)
                    block.type = BlockType::Bedrock;

                else if (y == terrainHeight - 1)
                    block.type = BlockType::Grass;

                else if (y >= terrainHeight - 4)
                    block.type = BlockType::Dirt;

                else
                    block.type = BlockType::Stone;

                blocks.push_back(block);
                occupiedBlocks.insert(block.position);
            }
        }
    }
}

bool World::isAir(int x, int y, int z) const
{
    if (x < 0 || x >= WORLD_SIZE ||
        y < 0 || y >= WORLD_HEIGHT ||
        z < 0 || z >= WORLD_SIZE)
        return true;

    return occupiedBlocks.find(glm::ivec3(x, y, z)) == occupiedBlocks.end();
}