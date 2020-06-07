#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "engine.hxx"

int main(int /*argc*/, char* /*argv*/[])
{
    // std::vector<uint16_t>       indexes_v;
    // std::vector<engine::vertex> v = make_grid(10, 10, indexes_v);

    std::unique_ptr<engine::core, void (*)(engine::core*)> engine(
        engine::create_engine(), engine::destroy_engine);

    if (!engine->init())
    {
        std::cerr << "init failed" << std::endl;
        return EXIT_FAILURE;
    }

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

        engine::triangle tr1, tr2;
        file >> tr1 >> tr2;

        engine->render_my_triangle(tr1);
        engine->render_my_triangle(tr2);

        engine->render_grid();

        engine->swap_buffers();
    }
    return EXIT_SUCCESS;
}
