attribute vec3 position;
attribute vec3 a_color;
attribute vec2 txt_pos;

varying vec4 v_position;
varying vec4 out_color;
varying vec2 out_txt_pos;

precision mediump float;

uniform vec4 in_uniform;
uniform vec4 in_uniform_txt;

void main() {
    out_txt_pos = txt_pos;
    
    float x = position.x;
    float y = position.y;
    
    float mouse_x = in_uniform.x;
    float mouse_y = in_uniform.y;

    float dx = x - mouse_x;
    float dy = y - mouse_y;
    
    float d = dx * dx + dy * dy;

if(d <= (in_uniform.z * in_uniform.z))
{
    float hyp = sqrt(d);
    
    float tx = dx / hyp;
    float ty = dy / hyp;
    
    float radius_x = tx * in_uniform.z + mouse_x;
    float radius_y = ty * in_uniform.z + mouse_y;

    x = (x + radius_x) / 2.0;
    y = (y + radius_y) / 2.0;
}

    v_position = vec4(x, y, 0.0, 1.0);
    out_color = vec4(in_uniform);
    
    gl_Position = vec4(x, y, 0.0, 1.0);
}
