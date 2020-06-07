precision mediump float;
/*mediump in*/varying vec4 out_color;
/*mediump in*/varying vec2 out_txt_pos;

///*mediump out*/varying vec4 FragColor;

uniform sampler2D ourTxt;

void main()
{
    //FragColor = vec4(out_color);
    gl_FragColor = texture2D(ourTxt,out_txt_pos);
}
