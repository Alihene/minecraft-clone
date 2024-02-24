#include "chunk_storage.hpp"

#include <cstring>

Block *ChunkData::get(i32 x, i32 y, i32 z) {
    return Block::getPackedBlock(data[x][y][z]);
}

void ChunkData::setData(Chunk *chunk) {
    std::memcpy(&data[0][0][0], &chunk->blocks[0][0][0], sizeof(chunk->blocks));
}

void ChunkData::writeData(Chunk *chunk) {
    std::memcpy(&chunk->blocks[0][0][0], &data[0][0][0], sizeof(chunk->blocks));
}

void ChunkRegion::remove(i32 x, i32 y) {
    for(i32 i = 0; i < addedPositions.size(); i++) {
        glm::ivec2 pos = glm::ivec2(x, y);
        if(pos == glm::ivec2(x + this->pos.x, y + this->pos.y)) {
            addedPositions.erase(addedPositions.begin() + i);
            return;
        }
    }
}

void ChunkRegion::destroy() {
    for(i32 x = 0; x < SIZE; x++) {
        for(i32 y = 0; y < SIZE; y++) {
            ChunkData *data = chunks[x][y];

            bool added = false;
            for(auto pos : addedPositions) {
                if(pos == glm::ivec2(x + this->pos.x, y + this->pos.y)) {
                    added = true;
                    break;
                }
            }

            if(added) {
                delete data;
            }
        }
    }
}

glm::ivec2 ChunkRegion::chunkPosToRegionPos(i32 x, i32 y) {
    glm::ivec2 pos;
    pos.x = floorf((f32) x / (f32) SIZE);
    pos.y = floorf((f32) y / (f32) SIZE);
    pos *= SIZE;

    return pos;
}

ChunkData *ChunkStorage::getChunkData(i32 x, i32 y) {
    ChunkRegion *region = nullptr;

    for(ChunkRegion &r : regions) {
        if(r.pos == ChunkRegion::chunkPosToRegionPos(x, y)) {
            region = &r;
            break;
        }
    }

    if(!region) {
        return nullptr;
    }

    bool added = false;
    for(auto pos : region->addedPositions) {
        if(pos == glm::ivec2(x, y)) {
            added = true;
            break;
        }
    }

    if(!added) {
        return nullptr;
    }

    return region->chunks[x % ChunkRegion::SIZE][y % ChunkRegion::SIZE];
}

void ChunkStorage::setChunk(Chunk *chunk) {
    ChunkData *data = getChunkData(chunk->pos.x, chunk->pos.y);

    if(!data) {
        ChunkRegion *region = nullptr;

        for(ChunkRegion &r : regions) {
            if(r.pos == ChunkRegion::chunkPosToRegionPos(chunk->pos.x, chunk->pos.y)) {
                region = &r;
                break;
            }
        }

        if(!region) {
            ChunkRegion newRegion;

            newRegion.pos = ChunkRegion::chunkPosToRegionPos(chunk->pos.x, chunk->pos.y);
            ChunkData *newData = new ChunkData();
            newData->setData(chunk);
            newRegion.chunks[chunk->pos.x % ChunkRegion::SIZE][chunk->pos.y % ChunkRegion::SIZE] = newData;
            newRegion.addedPositions.push_back(chunk->pos);

            regions.push_back(newRegion);
            return;
        }

        ChunkData *newData = new ChunkData();
        newData->setData(chunk);
        region->chunks[chunk->pos.x % ChunkRegion::SIZE][chunk->pos.y % ChunkRegion::SIZE] = newData;
        region->addedPositions.push_back(chunk->pos);
        return;
    }

    data->setData(chunk);
}

void ChunkStorage::addData(ChunkData *data, i32 x, i32 y) {
    ChunkRegion *region = nullptr;

    for(ChunkRegion &r : regions) {
        if(r.pos == ChunkRegion::chunkPosToRegionPos(x, y)) {
            region = &r;
            break;
        }
    }

    if(!region) {
        ChunkRegion newRegion;
        newRegion.pos = ChunkRegion::chunkPosToRegionPos(x, y);
        newRegion.chunks[x % ChunkRegion::SIZE][y % ChunkRegion::SIZE] = data;
        newRegion.addedPositions.push_back(glm::ivec2(x, y));

        regions.push_back(newRegion);
        return;
    }

    region->chunks[x % ChunkRegion::SIZE][y % ChunkRegion::SIZE] = data;
    region->addedPositions.push_back(glm::ivec2(x, y));
}

void ChunkStorage::destroy() {
    for(ChunkRegion &region : regions) {
        region.destroy();
    }
}