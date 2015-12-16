#version 330

out vec4 outcolor;

in vec2 TexCoord;
//in float pId;
in vec4 wpos;

void main()
{
	vec2 tmp = TexCoord - vec2(0.5,0.5);
    if(length(tmp)>0.5){
        discard;
    }
    outcolor = vec4(0.0, 1.0, 0.0, 1.0);
    gl_FragDepth = (wpos.z+0.09)/(wpos.w);
}
