#version 330

out vec4 fragColor;

in vec2 TexCoord;
in float active;
in float seeAble;
in float pId;
in float constraintProj;
in vec4 wpos;
//uniform vec3 pColor;

uniform float snappingId;
uniform float activeConstraintId;
uniform sampler2D gTexNormal;
uniform sampler2D gTexActive;

void main (void)
{
    vec2 tmp = TexCoord - vec2(0.5,0.5);
    if(length(tmp)>0.5){
        discard;
    }
    if(pId < snappingId + 0.5f && pId > snappingId - 0.5f){
        fragColor = vec4(0.9,0.4,0.4,1.0);
    }else if(pId < activeConstraintId + 0.5f && pId > activeConstraintId - 0.5f){
        fragColor = vec4(0.9,0.4,0.4,1.0);
    }else if (constraintProj > 0.5){
        fragColor = vec4(0.4,0.9,0.4,1.0);
	}else if(active < 0.5){
        fragColor = vec4(0.9,0.9,0.0,1.0);
    }else{
        fragColor = vec4(1.0,1.0,1.0, 1.0);
    }
    if(seeAble < 0.5){
        discard;
    }

    gl_FragDepth = (wpos.z+0.09)/(wpos.w);
    //fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
