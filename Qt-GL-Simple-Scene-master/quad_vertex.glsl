#version 330
layout(location=0)in vec3 vPosition;
layout(location=1)in vec3 vNormal;

out float seeAble;
out vec4 wpos;

uniform mat4 projection;
uniform mat4 modelview;


void main(void)
{
    vec3 norm = normalize(modelview*vec4(vNormal.xyz,0)).xyz;
    float dotProd = dot(norm, vec3(0,0,-1.0));
    if(dotProd > 0){
        seeAble =0.0;
    }else{
        seeAble = 0.5;
    }
    vec4 eyeCoordinatePos = modelview * vec4(vPosition, 1.0);
    gl_Position = projection * eyeCoordinatePos;
    wpos = gl_Position;
}

