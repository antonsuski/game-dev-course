#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "engine.hxx"
#include "shader.hxx"
#include "texture2d.hxx"

int main(int /*argc*/, char* /*argv*/[])
{
    // std::vector<uint16_t>       indexes_v;
    // std::vector<engine::vertex> v = make_grid(10, 10, indexes_v);

    std::unique_ptr<engine::core, void (*)(engine::core*)> engine(
        engine::create_engine(), engine::destroy_engine);

    if (!engine->init(767, 1365))
    {
        std::cerr << "init failed" << std::endl;
        return EXIT_FAILURE;
    }

    engine::shader sh("../../../shaders/grid_shader.vs",
                      "../../../shaders/grid_shader.fs");

    engine::shader tr_sh("../../../04_opengl/default_shader.vs",
                         "../../../04_opengl/default_shader.fs");

    engine::texture2d tank_texture("tank.png", 0);
    engine::texture2d tank_1("tank_1.png", 1);

    bool continue_loop = true;
    while (continue_loop)
    {
        engine::event event;

        while (engine->read_event(event))
        {
            std::cout << event << std::endl;
            switch (event.key)
            {
                case engine::event::turn_off:
                    continue_loop = false;
                    break;
                default:
                    break;
            }
        }

        std::ifstream file("vertexes.txt");
        assert(!!file);

        engine::triangle tr1, tr2, tr3, tr4;
        file >> tr1 >> tr2 >> tr3 >> tr4;

        engine->render_my_triangle(tr1, tr_sh, tank_texture);
        engine->render_my_triangle(tr2, tr_sh, tank_texture);

        engine->render_my_triangle(tr3, tr_sh, tank_1);
        engine->render_my_triangle(tr4, tr_sh, tank_1);

        engine->swap_buffers();
    }
    return EXIT_SUCCESS;
}
