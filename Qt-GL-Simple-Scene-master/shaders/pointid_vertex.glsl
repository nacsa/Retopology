#version 330

layout(location=0)in vec3 position;
layout(location=2)in float inId;

out vec4 ouId;

uniform mat4 projection;
uniform mat4 modelview;

void main(void)
{

   vec4 eyeCoordinatePos = modelview * vec4(position, 1.0);
   ouId = vec4(0,0,inId,1);
   gl_Position = projection * eyeCoordinatePos;
}

