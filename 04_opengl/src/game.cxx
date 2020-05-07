#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "engine.hxx"

std::vector<engine::vertex> make_grid(size_t x, size_t y);

int main(int /*argc*/, char* /*argv*/[])
{
    // engine::triangle grid = make_grid(10, 10);

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

        engine::triangle tr;
        file >> tr;

        // engine->render_triangle(tr);
        engine->render_my_triangle(tr);

        file >> tr;
        // std::cout << tr << std::endl;
        // engine->render_triangle(tr);
        engine->render_my_triangle(tr);

        engine->swap_buffers();
    }
    return EXIT_SUCCESS;
}

std::vector<engine::vertex> make_grid(size_t x, size_t y)
{
    engine::triangle            tr;
    std::vector<engine::vertex> vert_arr;

    const size_t x_offset = 640 / x;
    const size_t y_offset = 380 / y;

    for (size_t iterator_y = 0; iterator_y < y; ++iterator_y)
    {
        for (size_t iterator_x = 0; iterator_x < x; ++iterator_x)
        {
            engine::vertex tmp(iterator_x * x_offset, iterator_y * y_offset,
                               0.f, 1.f, 1.f, 1.f);
            vert_arr.push_back(tmp);
        }
    }

    std::cout << tr << std::endl;

    return vert_arr;
}
