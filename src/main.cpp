#include "state.hpp"

#include "world/generation/flat_terrain_generator.hpp"
#include "world/generation/overworld_terrain_generator.hpp"

#include <thread>

State state;

int main(i32 argc, char **argv) {
    if(argc < 2) {
        std::cerr << "Invalid arguments supplied" << std::endl;
        return -1;
    }

    state.resourceDir = std::string(argv[1]);

    state.settings.renderDistance = 8;

    Window window("Minecraft Clone", 854, 480);
    state.window = &window;

    Renderer renderer;
    state.renderer = &renderer;

    BlockManager blockManager;
    state.blockManager = &blockManager;

    World world;
    state.world = &world;

    Player player;
    state.player = &player;

    TerrainGenerator *terrainGenerator = new OverworldTerrainGenerator();
    state.terrainGenerator = terrainGenerator;

    auto chunkMeshFunc = [&]() {
        while(!window.shouldClose()) {
            world.updateChunks();
        }
    };

    //std::thread chunkMeshThread(chunkMeshFunc);

    f32 lastTime = 0.0f;
    f32 timestep;

    world.setBlock(200, 100, 200, blockManager.getBlockByType(Block::LOG));

    while(!window.shouldClose()) {
        f32 currentTime = (f32) glfwGetTime();
        timestep = currentTime - lastTime;
        lastTime = currentTime;

        player.update(timestep);
        renderer.camera.update();

        world.loadChunks();
        world.updateChunks();

        renderer.prepareFrame();

        renderer.renderWorld();

        window.endFrame();
    }

    //chunkMeshThread.join();

    world.destroy();

    delete terrainGenerator;

    return 0;
}