/*layout(location = 0) in*/attribute vec3 pos;
/*layout(location = 1) in*/attribute vec3 color;
/*layout(location = 2) in*/attribute vec2 txt_pos;

/*mediump out*/varying vec4 out_color;
/*mediump out*/varying vec2 out_txt_pos;
uniform vec4 in_uniform;

void main()
{
    gl_Position = vec4(pos, 1.0);
    out_color = vec4(in_uniform);
    out_txt_pos = txt_pos;
}
