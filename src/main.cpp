#include "state.hpp"

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

    world.setBlock(0, 0, 0, blockManager.getBlockByType(Block::STONE));

    while(!window.shouldClose()) {
        player.update();
        renderer.camera.update();

        world.loadChunks();
        world.updateChunks();

        renderer.prepareFrame();

        renderer.renderWorld();

        window.endFrame();
    }

    world.destroy();

    return 0;
}