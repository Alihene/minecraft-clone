#include "world.hpp"

#include "state.hpp"

void World::addChunk(glm::ivec2 pos) {
    Chunk *chunk = new Chunk();
    chunk->pos = pos;
    chunk->blockPos = pos * 16;

    std::vector<i32> blockSetIndices;

    for(i32 i = 0; i < pendingBlockChanges.size(); i++) {
        BlockSetData &data = pendingBlockChanges[i];
        if(data.chunkPos == pos) {
            chunk->set(data.blockPos.x, data.blockPos.y, data.blockPos.z, data.block);
            blockSetIndices.push_back(i);
        }
    }

    for(i32 i = blockSetIndices.size() - 1; i >= 0; i--) {
        pendingBlockChanges.erase(pendingBlockChanges.begin() + blockSetIndices[i]);
    }

    chunks.push_back(chunk);
}

void World::setBlock(i32 x, i32 y, i32 z, Block *block) {
    if(y > Chunk::HEIGHT - 1) {
        return;
    }

    i32 chunkPosX = (i32) floorf((f32) x / 16.0f);
    i32 chunkPosZ = (i32) floorf((f32) z / 16.0f);

    Chunk *chunk = nullptr;
    for(Chunk *c : chunks) {
        if(c->pos.x == chunkPosX && c->pos.y == chunkPosZ) {
            chunk = c;
            break;
        }
    }

    glm::uvec3 posInChunk = glm::uvec3(x % 16, y, z % 16);

    if(chunk == nullptr) {
        // Chunk isn't loaded, block needs to be set when this chunk loads
        pendingBlockChanges.push_back((BlockSetData) {
            .chunkPos = glm::ivec2(chunkPosX, chunkPosZ),
            .blockPos = posInChunk,
            .block = block
        });
        return;
    }

    chunk->set(posInChunk.x, posInChunk.y, posInChunk.z, block);
}

void World::setBlock(glm::ivec3 pos, Block *block) {
    setBlock(pos.x, pos.y, pos.z, block);
}

Block *World::getBlock(i32 x, i32 y, i32 z) {
    i32 chunkPosX = (i32) floorf((f32) x / 16.0f);
    i32 chunkPosZ = (i32) floorf((f32) z / 16.0f);

    Chunk *chunk = nullptr;
    for(Chunk *c : chunks) {
        if(c->pos.x == chunkPosX && c->pos.y == chunkPosZ) {
            chunk = c;
            break;
        }
    }

    if(chunk == nullptr) {
        // Chunk is not loaded
        return nullptr;
    }

    return chunk->get(x, y, z);
}

Block *World::getBlock(glm::ivec3 pos) {
    return getBlock(pos.x, pos.y, pos.z);
}

void World::updateChunks() {
    for(Chunk *chunk : chunks) {
        ChunkMesh *mesh = chunk->mesh;

        if(mesh->shouldMesh) {
            mesh->mesh();
            return;
        }
    }
}

void World::loadChunks() {
    i32 renderDistance = state.settings.renderDistance;

    std::vector<i32> chunksToRemove;

    i32 chunkPosX = (i32) floorf(state.player->pos.x / 16.0f);
    i32 chunkPosZ = (i32) floorf(state.player->pos.z / 16.0f);

    for(i32 i = 0; i < chunks.size(); i++) {
        Chunk *chunk = chunks[i];

        if(abs(chunk->pos.x - chunkPosX) > renderDistance) {
            chunksToRemove.push_back(i);
        } else if(abs(chunk->pos.y - chunkPosZ) > renderDistance) {
            chunksToRemove.push_back(i);
        }
    }

    for(i32 i = chunksToRemove.size() - 1; i >= 0; i--) {
        chunks[chunksToRemove[i]]->destroy();
        delete chunks[chunksToRemove[i]];
        chunks.erase(chunks.begin() + chunksToRemove[i]);
    }

    for(i32 x = -renderDistance; x <= renderDistance; x++) {
        for(i32 z = -renderDistance; z <= renderDistance; z++) {
            bool chunkExists = false;

            for(Chunk *chunk : chunks) {
                if(chunk->pos.x == x + chunkPosX && chunk->pos.y == z + chunkPosZ) {
                    chunkExists = true;
                }
            }

            if(!chunkExists) {
                addChunk({x + chunkPosX, z + chunkPosZ});
            }
        }
    }
}

Chunk *World::getChunk(glm::ivec2 pos) {
    for(Chunk *chunk : chunks) {
        if(chunk->pos == pos) {
            return chunk;
        }
    }

    return nullptr;
}

glm::ivec3 World::pointToBlockPos(glm::vec3 point) {
    return glm::ivec3((i32) floorf(point.x), (i32) floorf(point.y), (i32) floorf(point.z));
}

void World::destroy() {
    for(Chunk *chunk : chunks) {
        delete chunk;
    }
}