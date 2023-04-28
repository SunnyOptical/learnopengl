#version 330

in vec2 uv_Coord;
uniform sampler2D texture;
void main(void)
{
    gl_FragColor = texture2D(texture,uv_Coord);
}
