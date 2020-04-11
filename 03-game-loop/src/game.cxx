#include <iostream>
#include <memory>

#include "engine.hxx"

int main(int /*argc*/, char* /*argv*/[])
{
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
    }
    return EXIT_SUCCESS;
}
