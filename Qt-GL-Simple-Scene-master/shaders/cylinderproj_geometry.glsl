#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices=6) out;

in vec4 ouposition[];
in vec3 datas[];
out vec4 gscolor;

void main()
{
    bool needToExtendAngle=false;
    int i;
    float angle;
    float pi = 3.14159265;


    /*if(distance(datas[0].x, datas[1].x) > distance(datas[0].x, -datas[1].x) ||
        distance(datas[0].x, datas[2].x) > distance(datas[0].x, -datas[2].x) ||
        distance(datas[2].x, datas[1].x) > distance(datas[2].x, -datas[1].x) )

        needToExtendAngle = true;

*/
    /*
    if((datas[0].x >= 0.0 && datas[1].x < 0.0 && distance(datas[0].x, datas[1].x) > distance(datas[0].x, datas[1].x+2.0*pi)) ||
        (datas[0].x < 0.0 && datas[1].x >= 0.0 && distance(datas[0].x, datas[1].x) > distance(datas[0].x+2.0*pi, datas[1].x)) ||
        (datas[0].x >= 0.0 && datas[2].x < 0.0 && distance(datas[0].x, datas[2].x) > distance(datas[0].x, datas[2].x+2.0*pi)) ||
        (datas[0].x < 0.0 && datas[2].x >= 0.0 && distance(datas[0].x, datas[2].x) > distance(datas[0].x+2.0*pi, datas[2].x)))
    {
        needToExtendAngle = true;
    }
    */

    if((datas[0].x >= 0.0 && datas[1].x < 0.0 && (datas[0].x - datas[1].x >  datas[1].x+2.0*pi - datas[0].x)) ||
        (datas[0].x < 0.0 && datas[1].x >= 0.0 && (datas[1].x - datas[0].x >  datas[0].x+2.0*pi - datas[1].x)) ||
        (datas[0].x >= 0.0 && datas[2].x < 0.0 && (datas[0].x - datas[2].x >  datas[2].x+2.0*pi - datas[0].x)) ||
        (datas[0].x < 0.0 && datas[2].x >= 0.0 && (datas[2].x - datas[0].x >  datas[0].x+2.0*pi - datas[2].x)))
    {
        needToExtendAngle = true;
    }

    //needToExtendAngle = true;

    if(needToExtendAngle){
        for(i=0; i<3; i++){
            if(datas[i].x > 0.0){
                angle = datas[i].x;
            }else{
                angle = datas[i].x + 2.0*pi;
            }

            angle = angle / pi;
            gl_Position = vec4(angle, datas[i].y, datas[i].z, 1.0);
            gscolor = ouposition[i];
            EmitVertex();
        }
        EndPrimitive();

        for(i=0; i<3; i++){
            if(datas[i].x > 0.0){
                angle = datas[i].x - 2.0*pi;
            }else{
                angle = datas[i].x;
            }

            angle = angle / pi;
            gl_Position = vec4(angle, datas[i].y, datas[i].z, 1.0);
            gscolor = ouposition[i];
            EmitVertex();
        }
        EndPrimitive();


    }
    else{
        for(i=0; i<3; i++){
            angle = datas[i].x / pi;
            gl_Position = vec4(angle, datas[i].y, datas[i].z, 1.0);
            gscolor = ouposition[i];
            EmitVertex();
        }
        EndPrimitive();
    }
}
