#version 320 es

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

mediump out vec4 out_color;
uniform vec4 in_uniform;

void main()
{
    gl_Position = vec4(pos, 1.0);
    out_color = vec4(in_uniform);
}
