#version 330
layout(location=0)in vec3 vPosition;
layout(location=1)in vec3 vNormal;
layout(location=3)in float iActive;
layout(location=4)in float iBorder;

out float active;
out float seeAble;
out float border;
out vec4 wpos;

uniform mat4 projection;
uniform mat4 modelview;


void main(void)
{
    active = iActive;
    border = iBorder;
    vec3 norm = normalize(modelview*vec4(vNormal.xyz,0)).xyz;
    float dotProd = dot(norm, vec3(0,0,-1.0));
    if(dotProd > 0){
        seeAble =0.0;
    }else{
        seeAble = 1.0;
    }
    vec4 eyeCoordinatePos = modelview * vec4(vPosition, 1.0);
    gl_Position = projection * eyeCoordinatePos;
    wpos = gl_Position;
}

