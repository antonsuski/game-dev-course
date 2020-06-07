precision mediump float;

attribute vec3 pos;
attribute vec3 color;
attribute vec2 txt_pos;

varying vec4 out_color;
varying vec2 out_txt_pos;

uniform vec4 in_uniform;

void main()
{
    gl_Position = vec4(pos, 1.0);
    out_color = vec4(color, 0.0);
    out_txt_pos = txt_pos;
}
