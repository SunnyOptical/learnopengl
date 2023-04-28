#version 330

//定义顶点位置坐标属性信息和颜色信息
//对于属性在VAO中的访问可以使用index或者name,index用layout(location=x)表示
attribute vec4 qt_Vertex;

//接收输入的纹理坐标
attribute vec2 qt_Coord;

//定义一个vec3类型的向量，输出到下一级着色器，也就是片段着色器中,out代表输出型变量，in代表输入型变量

//输出到下一级的纹理坐标
out vec2 uv_Coord;

void main(void)
{
    //GLSL中内置的顶点位置属性变量,取vertex的xyz和1组合成一个vec4
    gl_Position = vec4(qt_Vertex.xyz,1.0);

    uv_Coord = qt_Coord;
}
