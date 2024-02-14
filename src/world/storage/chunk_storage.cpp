#include "chunk_storage.hpp"

#include <cstring>

ChunkData::ChunkData() {
    
}

Block *ChunkData::get(i32 x, i32 y, i32 z) {
    return Block::getPackedBlock(data[x][y][z]);
}

void ChunkData::setData(Chunk *chunk) {
    std::memcpy(&data[0][0][0], &chunk->blocks[0][0][0], sizeof(chunk->blocks));
}

void ChunkData::writeData(Chunk *chunk) {
    std::memcpy(&chunk->blocks[0][0][0], &data[0][0][0], sizeof(chunk->blocks));
}

void ChunkData::destroy() {
    std::free(data);
}

ChunkData *ChunkRegion::get(i32 x, i32 y) {
    return chunks[x][y];
}

void ChunkRegion::destroy() {
    for(i32 x = 0; x < SIZE; x++) {
        for(i32 y = 0; y < SIZE; y++) {
            chunks[x][y]->destroy();
            delete chunks[x][y];
        }
    }
}

glm::ivec2 ChunkRegion::chunkPosToRegionPos(i32 x, i32 y) {
    glm::ivec2 pos;
    pos.x = floorf((f32) x / (f32) SIZE);
    pos.y = floorf((f32) y / (f32) SIZE);
    pos.x *= Chunk::WIDTH;
    pos.y *= Chunk::DEPTH;
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
            data = new ChunkData();
            data->setData(chunk);
            newRegion.chunks[chunk->pos.x % ChunkRegion::SIZE][chunk->pos.y % ChunkRegion::SIZE] = data;
            newRegion.addedPositions.push_back(glm::ivec2(chunk->pos.x, chunk->pos.y));

            regions.push_back(newRegion);
            return;
        }

        data = new ChunkData();
        data->setData(chunk);
        region->chunks[chunk->pos.x % ChunkRegion::SIZE][chunk->pos.y % ChunkRegion::SIZE] = data;
        region->addedPositions.push_back(glm::ivec2(chunk->pos.x, chunk->pos.y));
        return;
    }

    data->setData(chunk);
}

void ChunkStorage::destroy() {
    for(ChunkRegion &region : regions) {
        region.destroy();
    }
}