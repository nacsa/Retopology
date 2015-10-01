#version 330
out vec4 fragColor;

in float seeAble;
in vec4 wpos;



void main (void)
{

    fragColor.rgb = vec3(0.5, 0.5, 0.5);

    fragColor.a = seeAble;

    //fragColor = vec4(1.0,0.0,0.0,1.0);

    gl_FragDepth = (wpos.z+0.09)/(wpos.w);
}
