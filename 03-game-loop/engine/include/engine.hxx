#pragma once

#include <array>
#include <iostream>
#include <string_view>

#ifndef ENGINE_DECLSPEC
#define ENGINE_DECLSPEC
#endif

namespace engine
{
class event
{
public:
    enum key
    {
        up,
        down,
        left,
        right,
        select,
        start,
        button_1,
        button_2,
        turn_off
    } key;
    std::string_view name;
    bool             is_running;

    event(enum key event_key, std::string_view event_name,
          bool is_event_running)
        : key(event_key)
        , name(event_name)
        , is_running(is_event_running)
    {
    }
    event() {}
};

ENGINE_DECLSPEC std::ostream& operator<<(std::ostream& stream, const event& e);

class ENGINE_DECLSPEC core
{
public:
    virtual ~core()                 = 0;
    virtual bool init()             = 0;
    virtual bool read_event(event&) = 0;
};
ENGINE_DECLSPEC engine::core* create_engine();
ENGINE_DECLSPEC void          destroy_engine(engine::core* e);
} // namespace engine
