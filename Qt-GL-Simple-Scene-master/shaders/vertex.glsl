#version 330
/*
layout(location=0)in vec3 vPosition;
layout(location=1)in vec3 vColor;

out vec3 varyingColor;

uniform float twist;

void main()
{
  float angle = radians(twist);
  float cosLength = cos(angle);
  float sinLength = sin(angle);

  varyingColor = vColor;

  gl_Position = vec4(cosLength * vPosition.x +
                     -sinLength * vPosition.y,
                     sinLength * vPosition.x +
                     cosLength * vPosition.y,
                     0, 1);
}
*/

layout(location=0)in vec3 vPosition;
layout(location=1)in vec3 normal;

out vec3 vNormal;
out vec3 vLightDir;


uniform mat4 projection;
uniform mat4 modelview;
uniform mat3 normalmatrix;
uniform vec3 lightPosition;

//texturába renderelés, (half(jobb-e? mekkora hiba?))float text. poziciokat + flag belenyomni, ebből 1 px visszaolvas ami az egér alatt van mouseposból.
//rekurziv alg.ok gpgpu programozás

void main(void)
{
    vec4 eyeCoordinatePos = modelview * vec4(vPosition, 1.0);
    //vNormal = normalize(gl_NormalMatrix*vNormal);
   //vPosition = vec3(modelview * vec4(vPosition, 1.0));
   //vNormal = normalize(gl_NormalMatrix*vNormal);
   //varyingColor = vec3(0.8, 0.5, 0.3);

   vNormal = normalize(modelview*vec4(normal.xyz,0)).xyz; //normalmatrix*normal);

   vLightDir = normalize((lightPosition - eyeCoordinatePos.xyz));
   gl_Position = projection * eyeCoordinatePos;
}


