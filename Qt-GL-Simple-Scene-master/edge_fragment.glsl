#version 330
out vec4 fragColor;
in float active;
in float seeAble;
in float border;
in vec4 wpos;



void main (void)
{
    if(active < 0.5){
        if(border > 0.5f){
            fragColor.rgb = vec3(0.2, 0.2, 0.1);
        }else{
            fragColor.rgb = vec3(0.5, 0.5, 0.3);
        }
    }else{
        fragColor.rgb = vec3(1.0, 1.0, 1.0);
    }
    fragColor.a = seeAble;

    //fragColor = vec4(1.0,0.0,0.0,1.0);

    gl_FragDepth = (wpos.z+0.09)/(wpos.w);
}
