#include "shader.hxx"

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

    const char* vert_shader_code = vert_shader_src.c_str();
    const char* frag_shader_code = frag_shader_src.c_str();
}
} // namespace engine
