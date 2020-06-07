precision mediump float;

varying vec4 out_color;
varying vec2 out_txt_pos;

uniform sampler2D ourTxt;

void main()
{
    gl_FragColor = texture2D(ourTxt,out_txt_pos) + out_color;
}
