#version 330

in vec2 uv_Coord;
// 定义一个2D的纹理
uniform sampler2D texture;
void main(void)
{
    gl_FragColor = texture2D(texture,uv_Coord);
}
