#include "texture2d.hxx"
#include "glad/glad.h"
#include "om_gl_check.hxx"
#include "picopng.hxx"
#include "shader.hxx"

#include <string_view>
#include <vector>

namespace engine
{

void texture2d::bind()
{
    GLboolean is_texture = glIsTexture(id);
    assert(is_texture);
    OM_GL_CHECK()
    glBindTexture(GL_TEXTURE_2D, id);
    OM_GL_CHECK();
}

std::uint32_t texture2d::get_width() const
{
    return width;
}

std::uint32_t texture2d::get_height() const
{
    return height;
}

int texture2d::get_id() const
{
    return id;
}

texture2d::~texture2d()
{
    GLuint id_ = id;
    glDeleteTextures(1, &id_);
    OM_GL_CHECK()
}

texture2d::texture2d(std::string_view path_, size_t id_)
{
    id          = id_;
    path        = path_;
    x_pos       = -1;
    y_pos       = -1;
    width       = -1;
    height      = -1;
    coordinates = { -1, -1, -1, -1, -1, -1, -1, -1 };

    shader txt_shader = shader("../../../04_opengl/default_shader.vs",
                               "../../../04_opengl/default_shader.fs");
    // shader my_shd("1_shader.vs", "1_shader.fs");
    // shader my_shd("../../../04_opengl/1_shader.vs",
    //             "../../../04_opengl/default_shader.fs");

    txt_shader.use();
    int shd_proc = txt_shader.id;

    int location = glGetUniformLocation(shd_proc, "ourTxt");
    OM_GL_CHECK()

    assert(-1 != location);
    int texture_unit = id;
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    OM_GL_CHECK()

    if (!load_texture(path))
    {
        std::cerr << "failed load texture\n";
    }
    else
    {
        std::cerr << "texture is loaded" << std::endl;
    }

    glUniform1i(location, 0 + texture_unit);
    OM_GL_CHECK()

    glEnable(GL_BLEND);
    OM_GL_CHECK()

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    OM_GL_CHECK()
}

bool texture2d::load_texture(std::string_view path)
{
    std::vector<std::byte> png_file_in_memory;
    std::ifstream          ifs(path.data(), std::ios_base::binary);
    if (!ifs)
    {
        return false;
    }
    ifs.seekg(0, std::ios_base::end);
    size_t pos_in_file = static_cast<size_t>(ifs.tellg());
    png_file_in_memory.resize(pos_in_file);
    ifs.seekg(0, std::ios_base::beg);
    if (!ifs)
    {
        return false;
    }

    ifs.read(reinterpret_cast<char*>(png_file_in_memory.data()),
             static_cast<std::streamsize>(png_file_in_memory.size()));
    if (!ifs.good())
    {
        return false;
    }

    std::vector<std::byte> image;
    unsigned long          w = 0;
    unsigned long          h = 0;
    int error                = decodePNG(image, w, h, &png_file_in_memory[0],
                          png_file_in_memory.size(), false);

    int width_, height_, nr_channals;

    unsigned char* ptr_data =
        stbi_load(path.data(), &width_, &height_, &nr_channals, 0);

    // if there's an error, display it
    if (error != 0)
    {
        std::cerr << "error: " << error << std::endl;
        return false;
    }

    GLuint tex_handl = id;
    glGenTextures(1, &tex_handl);
    OM_GL_CHECK()
    glBindTexture(GL_TEXTURE_2D, tex_handl);
    OM_GL_CHECK()

    GLint mipmap_level = 0;
    GLint border       = 0;
    // clang-format off
        glTexImage2D(GL_TEXTURE_2D, // Specifies the target texture of the active texture unit
                     mipmap_level,  // Specifies the level-of-detail number. Level 0 is the base image level
                     GL_RGBA,       // Specifies the internal format of the texture
                     static_cast<GLsizei>(w),
                     static_cast<GLsizei>(h),
                     border,        // Specifies the width of the border. Must be 0. For GLES 2.0
                     GL_RGBA,       // Specifies the format of the texel data. Must match internalformat
                     GL_UNSIGNED_BYTE, // Specifies the data type of the texel data
                     ptr_data/*&image[0]*/);    // Specifies a pointer to the image data in memory
    // clang-format on
    OM_GL_CHECK()

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    OM_GL_CHECK()
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    OM_GL_CHECK()
    return true;
}

} // namespace engine
