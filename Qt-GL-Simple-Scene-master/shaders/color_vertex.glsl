#version 330

layout(location=0)in vec3 vPosition;
layout(location=1)in vec3 vNormal;
layout(location=2)in float vpId;
layout(location=3)in float iActive;
layout(location=4)in float iConstraintProj;

out float active_gs;
out float seeAble_gs;
out float pId_gs;
out float constraintProj_gs;

uniform mat4 projection;
uniform mat4 modelview;


void main(void)
{
    active_gs = iActive;
    pId_gs = vpId;
	constraintProj_gs = iConstraintProj;
    vec3 norm = normalize(modelview*vec4(vNormal.xyz,0)).xyz;
    float dotProd = dot(norm, vec3(0,0,-1.0));
    if(dotProd > 0){
        seeAble_gs =0.0;
    }else{
        seeAble_gs = 1.0;
    }
    vec4 eyeCoordinatePos = modelview * vec4(vPosition, 1.0);
    gl_Position = projection * eyeCoordinatePos;
}


