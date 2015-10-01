#version 330

layout(points)in;
layout(triangle_strip)out;
layout(max_vertices=4)out;

in float active_gs[];
in float seeAble_gs[];
in float pId_gs[];

out float active;
out float seeAble;
out float pId;

out vec2 TexCoord;
out vec4 wpos;


uniform mat4 projection;
uniform mat4 modelview;


void main(void)
{
    active = active_gs[0];
    seeAble = seeAble_gs[0];
    pId = pId_gs[0];

    vec3 Pos = gl_in[0].gl_Position.xyz;
    float w = gl_in[0].gl_Position.w;

    gl_Position = vec4(Pos+vec3(-0.01, -0.01, 0.0)*w, w);
    TexCoord = vec2(0.0, 0.0);
    wpos = gl_Position;
    EmitVertex();

    gl_Position = vec4(Pos+vec3(-0.01, 0.01, 0.0)*w, w);
    TexCoord = vec2(0.0, 1.0);
    wpos = gl_Position;
    EmitVertex();

    gl_Position = vec4(Pos+vec3(0.01, -0.01, 0.0)*w, w);
    TexCoord = vec2(1.0, 0.0);
    wpos = gl_Position;
    EmitVertex();

    gl_Position = vec4(Pos+vec3(0.01, 0.01, 0.0)*w, w);
    TexCoord = vec2(1.0, 1.0);
    wpos = gl_Position;
    EmitVertex();

    EndPrimitive();
}


