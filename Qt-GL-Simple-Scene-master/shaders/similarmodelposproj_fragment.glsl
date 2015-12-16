#version 330

out vec4 outcolor;
in vec4 ouposition;

void main()
{
    outcolor = ouposition;
}
