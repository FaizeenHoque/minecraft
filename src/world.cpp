#include "world.h"

void World::generate()
{
    for (int x = 0; x < 10; x++)
    {
        for (int z = 0; z < 10; z++)
        {
            for (int y = 0; y < 60; y++)
            {
                Block block;

                if (y == 59)
                    block.type = BlockType::Grass;
                else if (y >= 54)
                    block.type = BlockType::Dirt;
                else if (y >= 3)
                    block.type = BlockType::Stone;
                else
                    block.type = BlockType::Bedrock;

                block.position = glm::ivec3(x, y, z);

                blocks.push_back(block);
            }
        }
    }
}

bool World::isAir(int x, int y, int z) const
{
    return x < 0 || x >= 10 ||
           y < 0 || y >= 60 ||
           z < 0 || z >= 10;
}