#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "engine.hxx"

std::vector<engine::vertex> make_grid(size_t x, size_t y,
                                      std::vector<uint16_t>& indexes_v);

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

        engine::triangle tr;
        file >> tr;

        // engine->render_triangle(tr);
        // engine->render_grid();
        engine->render_my_triangle(tr);

        // file >> tr;
        // std::cout << tr << std::endl;
        // engine->render_triangle(tr);
        // engine->render_my_triangle(tr);

        engine->swap_buffers();
    }
    return EXIT_SUCCESS;
}

std::vector<engine::vertex> make_grid(size_t x, size_t y,
                                      std::vector<uint16_t>& indexes_v)
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

    for (size_t j = 0; j < y - 1; ++j)
    {
        for (size_t i = 0; i < x - 1; ++i)
        {
            uint16_t v0 = j * x + i;
            uint16_t v1 = v0 + 1;
            uint16_t v2 = v0 + x;
            uint16_t v3 = v2 + 1;

            // add two triangles
            //  v0-----v1
            //  |     /|
            //  |    / |
            //  |   /  |
            //  |  /   |
            //  | /    |
            //  v2-----v3
            // we want only cells without internal color fill
            // so generate "triangle" for every age

            indexes_v.insert(end(indexes_v), { v0, v1, v2 });
            indexes_v.insert(end(indexes_v), { v2, v1, v3 });
        }
    }

    for (auto i : vert_arr)
    {
        std::cout << "pos: " << i.x << " " << i.y << " " << i.z << '\n';
        std::cout << "col: " << i.r << " " << i.g << " " << i.b << " "
                  << std::endl;
    }
    int counter = 0;
    for (auto i : indexes_v)
    {
        ++counter;
        std::cout << i << " ";
        if (counter == 3)
        {
            counter = 0;
            std::cout << std::endl;
        }
    }

    return vert_arr;
}
