#version 330
in vec3 colors;

void main(void)
{
    gl_FragColor = vec4(colors.xyz,1.0);
}
