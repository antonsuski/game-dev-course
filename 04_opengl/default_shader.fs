#version 320 es

mediump out vec4 FragColor;
mediump in vec4 out_color;

void main()
{
    FragColor = vec4(out_color);
}
