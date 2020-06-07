precision mediump float;

varying vec4 v_position;
varying vec4 out_color;
varying vec2 out_txt_pos;

uniform vec4 in_uniform;

void main() {
    vec3 def_color = vec3(1.0, 0.0, 1.0);

    float x = v_position.x;
    float y = v_position.y;
    float r = out_color.r;
    float g = out_color.g;
    float b = out_color.b;

    float mouse_x = in_uniform.x;
    float mouse_y = in_uniform.y;

    float dx = x - mouse_x;
    float dy = y - mouse_y;

    float d = dx * dx + dy * dy;


        r = def_color.r + (r - def_color.r);
        g = def_color.g + (g - def_color.g);
        b = def_color.b + (b - def_color.b);


    gl_FragColor = vec4(r, g, b, 1.0);
}
