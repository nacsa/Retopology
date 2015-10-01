#version 330

out vec4 outcolor;
in vec4 ouId;

void main()
{
    outcolor = ouId;//;vec4(0,0,ouId,1);
}
