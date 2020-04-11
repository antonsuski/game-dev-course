#include "engine.hxx"
#include <SDL.h>
#include <algorithm>
#include <array>
#include <iostream>

static std::ostream& operator<<(std::ostream& out, const SDL_version& v)
{
    out << static_cast<int>(v.major) << ".";
    out << static_cast<int>(v.minor) << ".";
    out << static_cast<int>(v.patch);
    return out;
}

namespace engine
{
std::ostream& operator<<(std::ostream& stream, const event& e)
{
    std::uint32_t value   = static_cast<std::uint32_t>(e.key);
    std::uint32_t minimal = static_cast<std::uint32_t>(event::up);
    std::uint32_t maximal = static_cast<std::uint32_t>(event::turn_off);
    if (value >= minimal && value <= maximal)
    {
        if (e.is_running)
        {
            stream << e.name;
            stream << "_running";
            return stream;
        }
        stream << e.name;
        stream << "_is_finished";
        return stream;
    }
    else
    {
        throw std::runtime_error("too big event value");
    }
}

struct bind
{
    bind(SDL_Keycode bind_key, event bind_event)
        : key(bind_key)
        , e(bind_event)
    {
    }

    SDL_Keycode key;
    event       e;
};

const std::array<bind, 8> keys{ {
    { SDLK_w, { event::up, "up", false } },
    { SDLK_a, { event::left, "left", false } },
    { SDLK_s, { event::down, "down", false } },
    { SDLK_d, { event::right, "right", false } },
    { SDLK_q, { event::button_1, "button_1", false } },
    { SDLK_e, { event::button_2, "button_2", false } },
    { SDLK_ESCAPE, { event::select, "select", false } },
    { SDLK_RETURN, { event::start, "start", false } },
} };

static bool check_event(const SDL_Event& sdl_event, const bind*& key)
{
    using namespace std;

    const auto it = find_if(begin(keys), end(keys), [&](const bind& b) {
        return b.key == sdl_event.key.keysym.sym;
    });

    if (it != end(keys))
    {
        key = &(*it);
        return true;
    }
    return false;
}

class core_one final : public core
{
public:
    ~core_one() final override {}
    bool init() final override
    {
        SDL_version compiled = { 0, 0, 0 };
        SDL_version linked   = { 0, 0, 0 };

        SDL_VERSION(&compiled)
        SDL_GetVersion(&linked);

        if (SDL_COMPILEDVERSION !=
            SDL_VERSIONNUM(linked.major, linked.minor, linked.patch))
        {
            std::cerr << "warning: SDL2 compiled and linked version mismatch: "
                      << compiled << " " << linked << std::endl;
        }

        if (SDL_Init(SDL_INIT_EVERYTHING))
        {
            std::cerr << "init failed sdl_error: " << SDL_GetError()
                      << std::endl;
            return false;
        }
        else
        {
            SDL_Window* const window = SDL_CreateWindow(
                "title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320,
                240, SDL_WINDOW_SHOWN);
            if (window == nullptr)
            {
                std::cerr << "create windoe failed sdl_error: "
                          << SDL_GetError() << std::endl;
                return false;
            }
        }
        return true;
    }
    bool read_event(event& e) final override
    {
        SDL_Event sdl_event;
        if (SDL_PollEvent(&sdl_event))
        {
            const bind* key = nullptr;
            if (sdl_event.type == SDL_QUIT)
            {
                e.key  = event::turn_off;
                e.name = "turn_off";
                return true;
            }
            if (sdl_event.type == SDL_KEYDOWN)
            {
                if (check_event(sdl_event, key))
                {
                    e.key        = key->e.key;
                    e.name       = key->e.name;
                    e.is_running = true;
                    return true;
                }
            }
            if (sdl_event.type == SDL_KEYUP)
            {
                if (check_event(sdl_event, key))
                {
                    e.key        = key->e.key;
                    e.name       = key->e.name;
                    e.is_running = false;
                    return true;
                }
            }
        }
        return false;
    }
};

static bool already_exist = false;

core* create_engine()
{
    if (already_exist)
    {
        throw std::runtime_error("engine already exist");
    }
    core* result  = new core_one();
    already_exist = true;
    return result;
}

void destroy_engine(core* e)
{
    if (already_exist == false)
    {
        throw std::runtime_error("engine not created");
    }
    if (nullptr == e)
    {
        throw std::runtime_error("e is nullptr");
    }
    delete e;
}

core::~core(){};
} // namespace engine
