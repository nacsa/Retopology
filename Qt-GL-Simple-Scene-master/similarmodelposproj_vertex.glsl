#version 330

layout(location=0)in vec3 position;
layout(location=1)in vec3 normal;

out vec4 ouposition;

uniform mat4 projection;
uniform mat4 modelview;

void main(void)
{

   vec4 eyeCoordinatePos = modelview * vec4(position, 1.0);
   ouposition =vec4(position, 1.0);//vec4(position, 1.0);// projection * eyeCoordinatePos;
   
   // set Z to absolute Z
   eyeCoordinatePos.z = abs(eyeCoordinatePos.z);
   gl_Position = projection * eyeCoordinatePos;
}

