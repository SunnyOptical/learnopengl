#version 330
attribute vec4 qt_Vertex;
attribute vec3 qt_Color;
out vec3 colors;

void main(void)
{
    gl_Position = vec4(qt_Vertex.xyz,1.0);
    colors = qt_Color;
}
