#version 330
out vec4 fragColor;

in vec3 vNormal;
in vec3 vLightDir;


void main (void)
{
    float specPow = 100.0;
    vec3 Kd = vec3(0.82, 0.71, 0.55);
    vec3 nNormal = normalize(vNormal);
    vec3 nLightDir = normalize(vLightDir);

    float diff = max(0, dot(nLightDir, nNormal));

    vec3 vReflection = normalize(reflect(-nLightDir, nNormal));
    float spec = max(0, dot(nNormal, vReflection));



    fragColor.rgb = Kd * diff ;//+ vec3(pow(spec, specPow));//ambient + diffuse + spec;
    //fragColor.rgb = vec3(0.5, 0.7, 0.3);
    fragColor.a = 1.0;
}
