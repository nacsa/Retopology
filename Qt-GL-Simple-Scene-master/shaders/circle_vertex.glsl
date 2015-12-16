#version 330
layout(location=0)in vec3 vPosition;

uniform mat4 projection;
uniform mat4 modelview;

void main(void)
{
    vec4 eyeCoordinatePos = modelview * vec4(vPosition, 1.0);
    gl_Position = projection * eyeCoordinatePos;
}


