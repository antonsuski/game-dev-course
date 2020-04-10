#include <SDL.h>
#include <algorithm>
#include <array>
#include <engine.hxx>
#include <string_view>

namespace engine
{

static std::array<std::string_view, 17> event_names = {
    { "shut_down", "up_pushed", "down_pushed", "left_pushed", "right_pushed",
      "start_pushed", "select_pushed", "button_1_pushed", "button_2_pushed",
      "up_released", "down_released", "left_released", "right_released",
      "start_released", "select_released", "button_1_released",
      "button_2_released" }
};

std::ostream& operator<<(std::ostream& stream, const event e)
{
    std::uint32_t value   = static_cast<std::uint32_t>(e);
    std::uint32_t minimal = static_cast<std::uint32_t>(event::shut_down);
    std::uint32_t maximal =
        static_cast<std::uint32_t>(event::button_2_released);
    if (value >= minimal && value <= maximal)
    {
        stream << event_names[value];
        return stream;
    }
    else
    {
        throw std::runtime_error("too big event value");
    }
}

struct bind
{
    bind(SDL_Keycode k, std::string_view s, event pushed, event released)
        : key(k)
        , name(s)
        , event_pushed(pushed)
        , event_released(released)
    {
    }

    SDL_Keycode      key;
    std::string_view name;
    event            event_pushed;
    event            event_released;
};

const std::array<bind, 8> keys{
    { { SDLK_w, "up", event::up_pushed, event::up_released },
      { SDLK_a, "left", event::left_pushed, event::left_released },
      { SDLK_s, "down", event::down_pushed, event::down_released },
      { SDLK_d, "right", event::right_pushed, event::right_released },
      { SDLK_LCTRL, "button1", event::button_1_pushed,
        event::button_1_released },
      { SDLK_SPACE, "button2", event::button_2_pushed,
        event::button_2_released },
      { SDLK_ESCAPE, "select", event::select_pushed, event::select_released },
      { SDLK_RETURN, "start", event::start_pushed, event::start_released } }
};

static bool check_event(const SDL_Event& e, const bind*& result)
{
    using namespace std;
    const auto it = find_if(begin(keys), end(keys), [&](const bind& b) {
        return b.key == e.key.keysym.sym;
    });

    if (it != end(keys))
    {
        result = &(*it);
        return true;
    }
    return false;
}

class core_one final : public core
{
public:
    SDL_Window*  Window  = nullptr;
    SDL_Surface* Surface = nullptr;

    bool init() final override
    {
        if (SDL_Init(SDL_INIT_EVERYTHING))
        {
            std::cerr << "init failed sdl_error: " << SDL_GetError()
                      << std::endl;
            return false;
        }
        Window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, 640, 480,
                                  SDL_WINDOW_SHOWN);
        if (Window == nullptr)
        {
            std::cerr << "create window faled sdl_error: " << SDL_GetError()
                      << std::endl;
            return false;
        }
        else
        {
            Surface = SDL_GetWindowSurface(Window);
            if (Surface == nullptr)
            {
                std::cerr << "get surface faled sdl_error: " << SDL_GetError();
            }
        }
        return true;
    }
    bool handle_event(event& e) final override
    {
        SDL_Event sdl_event;
        if (SDL_PollEvent(&sdl_event))
        {
            const bind* binding = nullptr;
            if (sdl_event.type == SDL_QUIT)
            {
                e = event::shut_down;
                return true;
            }
            else if (sdl_event.type == SDL_KEYDOWN)
            {
                if (check_event(sdl_event, binding))
                {
                    e = binding->event_pushed;
                    return true;
                }
            }
            else if (sdl_event.type == SDL_KEYUP)
            {
                if (check_event(sdl_event, binding))
                {
                    e = binding->event_released;
                    return true;
                }
            }
        }
        return false;
    }

    ~core_one() final override {}
}; // namespace engine

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
engine::core::~core() {}
} // namespace engine
