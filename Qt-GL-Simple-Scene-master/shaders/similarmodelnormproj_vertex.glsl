#version 330
layout(location=0)in vec3 vPosition;
layout(location=1)in vec3 normal;

out vec4 vNormal;


uniform mat4 projection;
uniform mat4 modelview;


void main(void)
{
   vec4 eyeCoordinatePos = modelview * vec4(vPosition, 1.0);
   vNormal = vec4(normal, 0);//vec4(normalize(modelview*vec4(normal.xyz,0)).xyz, 1.0);
   
   // set Z to absolute Z
   eyeCoordinatePos.z = abs(eyeCoordinatePos.z);
   
   gl_Position = projection * eyeCoordinatePos;
}

