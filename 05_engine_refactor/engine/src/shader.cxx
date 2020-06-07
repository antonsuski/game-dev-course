#include "shader.hxx"
#include "om_gl_check.hxx"

#include <vector>

namespace engine
{
shader::shader(const char* vert_shader_path, const char* frag_shader_path)
{
    std::string   vert_shader_src;
    std::string   frag_shader_src;
    std::ifstream vert_shader_file;
    std::ifstream frag_shader_file;

    vert_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    frag_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vert_shader_file.open(vert_shader_path);
        frag_shader_file.open(frag_shader_path);
        std::stringstream vert_shader_stream, frag_shader_stream;

        vert_shader_stream << vert_shader_file.rdbuf();
        frag_shader_stream << frag_shader_file.rdbuf();

        vert_shader_file.close();
        frag_shader_file.close();

        vert_shader_src = vert_shader_stream.str();
        frag_shader_src = frag_shader_stream.str();
    }
    catch (std::ifstream::failure ex)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vert_shader_code = vert_shader_src.data();
    const char* frag_shader_code = frag_shader_src.c_str();

    // vertex shader compile

    GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_shader_code, nullptr);
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
        glGetShaderInfoLog(vert_shader, info_len, nullptr, info_chars.data());
        OM_GL_CHECK()

        glDeleteShader(vert_shader);
        OM_GL_CHECK()

        std::string shader_type_name = "vertex";
        std::cerr << "Error compiling shader(vertex)\n"
                  << vert_shader_code << "\n"
                  << info_chars.data();
    }

    // fragment shader compile

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_shader_code, nullptr);
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
        glGetShaderInfoLog(frag_shader, info_len, nullptr, info_chars.data());
        OM_GL_CHECK()

        glDeleteShader(frag_shader);
        OM_GL_CHECK()

        std::cerr << "Error compiling shader(fragment)\n"
                  << frag_shader_code << "\n"
                  << info_chars.data();
    }

    // gxf programm

    id = glCreateProgram();
    OM_GL_CHECK()
    if (0 == id)
    {
        std::cerr << "failed to create gl program";
    }

    glAttachShader(id, vert_shader);
    OM_GL_CHECK()

    glAttachShader(id, frag_shader);
    OM_GL_CHECK()

    glBindAttribLocation(id, 0, "pos");
    OM_GL_CHECK()
    glBindAttribLocation(id, 1, "color");
    OM_GL_CHECK()
    glBindAttribLocation(id, 2, "txt_pos");
    OM_GL_CHECK()

    glLinkProgram(id);
    OM_GL_CHECK()

    GLint linked_status = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &linked_status);
    OM_GL_CHECK()
    if (linked_status == 0)
    {
        GLint infoLen = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLen);
        OM_GL_CHECK()
        std::vector<char> infoLog(static_cast<size_t>(infoLen));
        glGetProgramInfoLog(id, infoLen, nullptr, infoLog.data());
        OM_GL_CHECK()
        std::cerr << "Error linking program:\n" << infoLog.data();
        glDeleteProgram(id);
        OM_GL_CHECK()
    }

    glEnable(GL_DEPTH_TEST);
    OM_GL_CHECK()

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
}

void shader::use()
{
    glUseProgram(id);
    OM_GL_CHECK()
}

void shader::set_uniform() {}

} // namespace engine
