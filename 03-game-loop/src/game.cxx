#include <engine.hxx>
#include <iostream>
#include <memory>

int main()
{
    std::unique_ptr<engine::core, void (*)(engine::core*)> engine(
        engine::create_engine(), engine::destroy_engine);

    if (!engine->init())
    {
        return EXIT_FAILURE;
    }

    bool continue_loop = true;
    while (continue_loop)
    {
        engine::event event;

        while (engine->handle_event(event))
        {
            std::cout << event << std::endl;
            switch (event)
            {
                case engine::event::shut_down:
                    continue_loop = false;
                    break;
                default:
                    break;
            }
        }
    }

    return EXIT_SUCCESS;
}
