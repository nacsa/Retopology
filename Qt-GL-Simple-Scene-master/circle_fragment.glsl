#version 330
out vec4 fragColor;
uniform vec3 outColor;

void main (void)
{
    fragColor = vec4(outColor, 0.5);
}
