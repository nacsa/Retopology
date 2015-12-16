#version 130
#extension GL_EXT_geometry_shader4 : enable

out vec4 nodedata;

void main(void)
{
   // vec4 vertexdata = gl_PositionIn[0];
    nodedata = vec4(1.0,0.0,0.0,1.0);//vec4(vertexdata.xyz,1);
    //EmitVertex();

}
