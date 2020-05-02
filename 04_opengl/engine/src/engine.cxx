#include "engine.hxx"
#include "glad/glad.h"

#include <SDL.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>

#define OM_GL_CHECK()                                                          \
    {                                                                          \
        const int err = static_cast<int>(glGetError());                        \
        if (err != GL_NO_ERROR)                                                \
        {                                                                      \
            switch (err)                                                       \
            {                                                                  \
                case GL_INVALID_ENUM:                                          \
                    std::cerr << GL_INVALID_ENUM << std::endl;                 \
                    break;                                                     \
                case GL_INVALID_VALUE:                                         \
                    std::cerr << GL_INVALID_VALUE << std::endl;                \
                    break;                                                     \
                case GL_INVALID_OPERATION:                                     \
                    std::cerr << GL_INVALID_OPERATION << std::endl;            \
                    break;                                                     \
                case GL_INVALID_FRAMEBUFFER_OPERATION:                         \
                    std::cerr << GL_INVALID_FRAMEBUFFER_OPERATION              \
                              << std::endl;                                    \
                    break;                                                     \
                case GL_OUT_OF_MEMORY:                                         \
                    std::cerr << GL_OUT_OF_MEMORY << std::endl;                \
                    break;                                                     \
            }                                                                  \
            assert(false);                                                     \
        }                                                                      \
    }

static std::array<char, GL_MAX_DEBUG_MESSAGE_LENGTH> local_log_buff;

static const char* source_to_strv(GLenum source)
{
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
            return "API";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return "SHADER_COMPILER";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return "WINDOW_SYSTEM";
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            return "THIRD_PARTY";
        case GL_DEBUG_SOURCE_APPLICATION:
            return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER:
            return "OTHER";
    }
    return "unknown";
}

static const char* type_to_strv(GLenum type)
{
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PERFORMANCE:
            return "PERFORMANCE";
        case GL_DEBUG_TYPE_PORTABILITY:
            return "PORTABILITY";
        case GL_DEBUG_TYPE_MARKER:
            return "MARKER";
        case GL_DEBUG_TYPE_PUSH_GROUP:
            return "PUSH_GROUP";
        case GL_DEBUG_TYPE_POP_GROUP:
            return "POP_GROUP";
        case GL_DEBUG_TYPE_OTHER:
            return "OTHER";
    }
    return "unknown";
}

static const char* severity_to_strv(GLenum severity)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            return "HIGH";
        case GL_DEBUG_SEVERITY_MEDIUM:
            return "MEDIUM";
        case GL_DEBUG_SEVERITY_LOW:
            return "LOW";
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return "NOTIFICATION";
    }
    return "unknown";
}

static void APIENTRY callback_opengl_debug(
    GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
    const GLchar* message, [[maybe_unused]] const void* userParam)
{
    // The memory formessageis owned and managed by the GL, and should onlybe
    // considered valid for the duration of the function call.The behavior of
    // calling any GL or window system function from within thecallback function
    // is undefined and may lead to program termination.Care must also be taken
    // in securing debug callbacks for use with asynchronousdebug output by
    // multi-threaded GL implementations.  Section 18.8 describes thisin further
    // detail.

    auto& buff{ local_log_buff };
    int   num_chars = std::snprintf(
        buff.data(), buff.size(), "%s %s %d %s %.*s\n", source_to_strv(source),
        type_to_strv(type), id, severity_to_strv(severity), length, message);

    if (num_chars > 0)
    {
        // TODO use https://en.cppreference.com/w/cpp/io/basic_osyncstream
        // to fix possible data races
        // now we use GL_DEBUG_OUTPUT_SYNCHRONOUS to garantie call in main
        // thread
        std::cerr.write(buff.data(), num_chars);
    }
}
static std::ostream& operator<<(std::ostream& out, const SDL_version& v)
{
    out << static_cast<int>(v.major) << ".";
    out << static_cast<int>(v.minor) << ".";
    out << static_cast<int>(v.patch);
    return out;
}

namespace engine
{
std::istream& operator>>(std::istream& is, vertex& v)
{
    is >> v.x;
    is >> v.y;
    is >> v.z;
    is >> v.r;
    is >> v.g;
    is >> v.b;
    return is;
}

std::istream& operator>>(std::istream& is, triangle& t)
{
    is >> t.v[0];
    is >> t.v[1];
    is >> t.v[2];
    return is;
}

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
private:
    SDL_Window*   window      = nullptr;
    SDL_GLContext gl_context  = nullptr;
    GLuint        program_id_ = 0;
    GLuint        VBO         = 0;
    GLuint        VAO         = 0;
    GLuint        shd_proc    = 0;

public:
    ~core_one() final override {}

    void genBuffers()
    {

        glGenBuffers(1, &VBO);
        OM_GL_CHECK()

        glGenVertexArrays(1, &VAO);
        OM_GL_CHECK()
    }

    bool init_my_opengl() final override
    {
        // enable debug

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(callback_opengl_debug, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                              nullptr, GL_TRUE);

        // gen buffers
        genBuffers();
        // vertex shader

        std::string_view vertex_shader_src = R"(
                #version 320 es

                layout(location = 0) in vec3 pos;
                layout(location = 1) in vec3 color;

                mediump out vec3 out_color;

                void main()
                {
                    gl_Position = vec4(pos, 1.0);
                    out_color = color;
                }
                                             )";

        const char* source      = vertex_shader_src.data();
        GLuint      vert_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert_shader, 1, &source, nullptr);
        OM_GL_CHECK()

        glCompileShader(vert_shader);
        OM_GL_CHECK()

        GLint compiled_status = 0;
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &compiled_status);
        OM_GL_CHECK()

        if (compiled_status == 0)
        {
            GLint info_len = 0;
            glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &info_len);
            OM_GL_CHECK()

            std::vector<char> info_chars(static_cast<size_t>(info_len));
            glGetShaderInfoLog(vert_shader, info_len, nullptr,
                               info_chars.data());
            OM_GL_CHECK()

            glDeleteShader(vert_shader);
            OM_GL_CHECK()

            std::string shader_type_name = "vertex";
            std::cerr << "Error compiling shader(vertex)\n"
                      << vertex_shader_src << "\n"
                      << info_chars.data();
            return false;
        }

        // fragment shader

        std::string_view frag_shader_src = R"(
                #version 320 es

                mediump out vec4 FragColor;
                mediump in vec3 out_color;

                void main()
                {
                    FragColor = vec4(out_color, 0.0);
                }
                )";

        source             = frag_shader_src.data();
        GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_shader, 1, &source, nullptr);
        OM_GL_CHECK()

        glCompileShader(frag_shader);
        OM_GL_CHECK()

        compiled_status = 0;
        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &compiled_status);
        OM_GL_CHECK()
        if (compiled_status == 0)
        {
            GLint info_len = 0;
            glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &info_len);
            OM_GL_CHECK()

            std::vector<char> info_chars(static_cast<size_t>(info_len));
            glGetShaderInfoLog(frag_shader, info_len, nullptr,
                               info_chars.data());
            OM_GL_CHECK()

            glDeleteShader(frag_shader);
            OM_GL_CHECK()

            std::cerr << "Error compiling shader(fragment)\n"
                      << vertex_shader_src << "\n"
                      << info_chars.data();
            return false;
        }

        // create shader programm

        shd_proc = glCreateProgram();
        OM_GL_CHECK()
        if (0 == shd_proc)
        {
            std::cerr << "failed to create gl program";
            return false;
        }

        glAttachShader(shd_proc, vert_shader);
        OM_GL_CHECK()

        glAttachShader(shd_proc, frag_shader);
        OM_GL_CHECK()

        glLinkProgram(shd_proc);
        OM_GL_CHECK()

        GLint linked_status = 0;
        glGetProgramiv(shd_proc, GL_LINK_STATUS, &linked_status);
        OM_GL_CHECK()
        if (linked_status == 0)
        {
            GLint infoLen = 0;
            glGetProgramiv(shd_proc, GL_INFO_LOG_LENGTH, &infoLen);
            OM_GL_CHECK()
            std::vector<char> infoLog(static_cast<size_t>(infoLen));
            glGetProgramInfoLog(shd_proc, infoLen, nullptr, infoLog.data());
            OM_GL_CHECK()
            std::cerr << "Error linking program:\n" << infoLog.data();
            glDeleteProgram(shd_proc);
            OM_GL_CHECK()
            return false;
        }
        glUseProgram(shd_proc);
        OM_GL_CHECK()

        glEnable(GL_DEPTH_TEST);
        OM_GL_CHECK()
        return true;
    }

    bool init_opengl()
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(callback_opengl_debug, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                              nullptr, GL_TRUE);
        // init_my_opengl();
        genBuffers();
        GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
        OM_GL_CHECK()
        std::string_view vertex_shader_src = R"(
                                        attribute vec3 a_position;
                                        varying vec4 v_position;

                                        void main()
                                        {
                                            v_position = vec4(a_position, 1.0);
                                            gl_Position = v_position;
                                        }
                                        )";
        const char*      source            = vertex_shader_src.data();
        glShaderSource(vert_shader, 1, &source, nullptr);
        OM_GL_CHECK()

        glCompileShader(vert_shader);
        OM_GL_CHECK()

        GLint compiled_status = 0;
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &compiled_status);
        OM_GL_CHECK()
        if (compiled_status == 0)
        {
            GLint info_len = 0;
            glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &info_len);
            OM_GL_CHECK()
            std::vector<char> info_chars(static_cast<size_t>(info_len));
            glGetShaderInfoLog(vert_shader, info_len, nullptr,
                               info_chars.data());
            OM_GL_CHECK()
            glDeleteShader(vert_shader);
            OM_GL_CHECK()

            std::string shader_type_name = "vertex";
            std::cerr << "Error compiling shader(vertex)\n"
                      << vertex_shader_src << "\n"
                      << info_chars.data();
            return false;
        }
        // fargmemt shader
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        OM_GL_CHECK()
        std::string_view fragment_shader_src = R"(
                          precision mediump float;
                          varying mediump vec4 v_position;

                          void main()
                          {
                              if (v_position.z >= 0.0)
                              {
                                  float light_green = 0.5 + v_position.z / 2.0;
                                  gl_FragColor = vec4(0.0, light_green, 0.0, 1.0);
                              } else
                              {
                                  float color = 0.5 - (v_position.z / -2.0);
                                  gl_FragColor = vec4(color, 0.0, 0.0, 1.0);
                              }
                          }
                          )";
        source                               = fragment_shader_src.data();
        glShaderSource(fragment_shader, 1, &source, nullptr);
        OM_GL_CHECK()

        glCompileShader(fragment_shader);
        OM_GL_CHECK()

        compiled_status = 0;
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled_status);
        OM_GL_CHECK()
        if (compiled_status == 0)
        {
            GLint info_len = 0;
            glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &info_len);
            OM_GL_CHECK()
            std::vector<char> info_chars(static_cast<size_t>(info_len));
            glGetShaderInfoLog(fragment_shader, info_len, nullptr,
                               info_chars.data());
            OM_GL_CHECK()
            glDeleteShader(fragment_shader);
            OM_GL_CHECK()

            std::cerr << "Error compiling shader(fragment)\n"
                      << vertex_shader_src << "\n"
                      << info_chars.data();

            glEnable(GL_DEPTH_TEST);

            return false;
        }

        // now create program and attach vertex and fragment shaders

        program_id_ = glCreateProgram();
        OM_GL_CHECK()
        if (0 == program_id_)
        {
            std::cerr << "failed to create gl program";
            return false;
        }

        glAttachShader(program_id_, vert_shader);
        OM_GL_CHECK()
        glAttachShader(program_id_, fragment_shader);
        OM_GL_CHECK()

        // bind attribute location
        glBindAttribLocation(program_id_, 0, "a_position");
        OM_GL_CHECK()
        // link program after binding attribute locations
        glLinkProgram(program_id_);
        OM_GL_CHECK()
        // Check the link status
        GLint linked_status = 0;
        glGetProgramiv(program_id_, GL_LINK_STATUS, &linked_status);
        OM_GL_CHECK()
        if (linked_status == 0)
        {
            GLint infoLen = 0;
            glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &infoLen);
            OM_GL_CHECK()
            std::vector<char> infoLog(static_cast<size_t>(infoLen));
            glGetProgramInfoLog(program_id_, infoLen, nullptr, infoLog.data());
            OM_GL_CHECK()
            std::cerr << "Error linking program:\n" << infoLog.data();
            glDeleteProgram(program_id_);
            OM_GL_CHECK()
            return false;
        }

        // turn on rendering with just created shader program
        glUseProgram(program_id_);
        OM_GL_CHECK()

        glEnable(GL_DEPTH_TEST);
        // glDisable(GL_DEPTH_TEST);
        return true;
    }

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
            window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED, 640, 480,
                                      SDL_WINDOW_OPENGL);
            if (window == nullptr)
            {
                std::cerr << "create windoe failed sdl_error: "
                          << SDL_GetError() << std::endl;
                return false;
            }
            int gl_major_ver = 3;
            int gl_minor_ver = 2;
            // int gl_context_profile = SDL_GL_CONTEXT_PROFILE_ES;

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                                SDL_GL_CONTEXT_PROFILE_ES);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_ver);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_ver);

            gl_context = SDL_GL_CreateContext(window);

            if (gl_context == nullptr)
            {
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                                    SDL_GL_CONTEXT_PROFILE_CORE);
                gl_context = SDL_GL_CreateContext(window);
            }
            int result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
                                             &gl_major_ver);
            assert(result == 0);
            result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
                                         &gl_minor_ver);
            assert(result == 0);

            if (gl_major_ver != 3 || gl_minor_ver != 2)
            {
                std::clog << "current context opengl version: " << gl_major_ver
                          << '.' << gl_minor_ver << '\n'
                          << "need openg ES version at least: 3.2\n"
                          << std::flush;
                throw std::runtime_error("opengl version too low");
            }

            if (gladLoadGLES2Loader(SDL_GL_GetProcAddress) == 0)
            {
                std::clog << "error: failed to initialize glad" << std::endl;
            }
        }
        return init_my_opengl();
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

    void render() final override
    {

        glClearColor(0.f, 0.f, 1.f, 0.0f);
        OM_GL_CHECK()
        glClear(GL_COLOR_BUFFER_BIT);
        OM_GL_CHECK()
        // TODO continue...
    }

    void swap_buffers() final override
    {
        SDL_GL_SwapWindow(window);

        glClearColor(0.3f, 0.3f, 1.0f, 0.0f);
        OM_GL_CHECK()
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        OM_GL_CHECK()
    }

    void uninitialize() final override
    {
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void render_my_triangle(const triangle& t) final override
    {
        //        float vertices[] = {
        //            -0.5f, -0.5f, 0.0f, // левая вершина
        //            0.5f,  -0.5f, 0.0f, // правая вершина
        //            0.0f,  0.5f,  0.0f  // верхняя вершина
        //        };
        glBindVertexArray(VAO);
        OM_GL_CHECK()

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        OM_GL_CHECK()

        glBufferData(GL_ARRAY_BUFFER, sizeof(t), &t.v, GL_STATIC_DRAW);
        OM_GL_CHECK()

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                              nullptr);
        OM_GL_CHECK()

        glEnableVertexAttribArray(0);
        OM_GL_CHECK()

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                              reinterpret_cast<void*>(sizeof(float) * 3));
        OM_GL_CHECK()

        glEnableVertexAttribArray(1);
        OM_GL_CHECK()

        GLuint shd_proc_value = shd_proc;

        glValidateProgram(shd_proc_value);
        OM_GL_CHECK()
        // Check the validate status
        GLint validate_status = 0;
        glGetProgramiv(shd_proc_value, GL_VALIDATE_STATUS, &validate_status);
        OM_GL_CHECK()

        if (validate_status == GL_FALSE)
        {
            GLint infoLen = 0;
            glGetProgramiv(shd_proc_value, GL_INFO_LOG_LENGTH, &infoLen);
            OM_GL_CHECK()
            std::vector<char> infoLog(static_cast<size_t>(infoLen));
            glGetProgramInfoLog(shd_proc_value, infoLen, nullptr,
                                infoLog.data());
            OM_GL_CHECK()
            std::cerr << "Error linking program:\n" << infoLog.data();
            throw std::runtime_error("error");
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        OM_GL_CHECK()

        glDrawArrays(GL_TRIANGLES, 0, 3);
        OM_GL_CHECK()
    }

    void render_triangle(const triangle& t) final override
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                              &t.v[0]);
        OM_GL_CHECK()
        glEnableVertexAttribArray(0);
        OM_GL_CHECK()
        glValidateProgram(program_id_);
        OM_GL_CHECK()
        // Check the validate status
        GLint validate_status = 0;
        glGetProgramiv(program_id_, GL_VALIDATE_STATUS, &validate_status);
        OM_GL_CHECK()
        if (validate_status == GL_FALSE)
        {
            GLint infoLen = 0;
            glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &infoLen);
            OM_GL_CHECK()
            std::vector<char> infoLog(static_cast<size_t>(infoLen));
            glGetProgramInfoLog(program_id_, infoLen, nullptr, infoLog.data());
            OM_GL_CHECK()
            std::cerr << "Error linking program:\n" << infoLog.data();
            throw std::runtime_error("error");
        }
        glDrawArrays(GL_TRIANGLES, 0, 3);
        OM_GL_CHECK()
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
