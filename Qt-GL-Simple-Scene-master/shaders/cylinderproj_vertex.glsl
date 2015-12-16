#version 330

layout(location=0)in vec3 position;
layout(location=1)in vec3 normal;

out vec4 ouposition;
out vec3 datas;


uniform vec3 cylinderBasePoint;
uniform vec3 cylinderTopPoint;
uniform vec3 cylinderUpDir;
uniform vec3 cylinderRightDir;
uniform float cylinderHeight;
uniform float cylinderRadius;

uniform bool outputType;


void main(void)
{
        vec3 baseToTop = cylinderTopPoint - cylinderBasePoint;
        vec3 baseToPoint = position - cylinderBasePoint;
	float distFromCenter = 0;
	float distFromBase = 0;
        float distFromTop = 0;
        vec3 pointProjectionToLine = cylinderBasePoint + dot(baseToPoint,baseToTop) / dot(baseToTop, baseToTop) * baseToTop;
        bool invalidheight = false;
        float heightSign = 1.0;


	distFromBase = distance(pointProjectionToLine, cylinderBasePoint);
        distFromTop = distance(pointProjectionToLine, cylinderTopPoint);
        if(distFromBase > cylinderHeight ||  distFromTop > cylinderHeight){
                //shouldDiscard = 1.0;
                //ouposition = vec4(0);
                //gl_Position = vec4(2000,2000,2000,1.0);
                //return;
            if(distFromBase < distFromTop)
                heightSign = -1.0;
            invalidheight = true;
        }
	
	distFromCenter = distance(pointProjectionToLine, position);
	if(distFromCenter > cylinderRadius){
                //shouldDiscard = 1.0;
                //ouposition = vec4(0);
                //gl_Position = vec4(2000,2000,2000,1.0);
                //return;
	}
        vec3 pointDir = position - pointProjectionToLine;
        pointDir = normalize(pointDir);
        float angleSign = sign(dot(cylinderUpDir, cross(cylinderRightDir, pointDir)));
        //#1 solve
        //float angle = acos(dot(cylinderRightDir, pointDir) / distFromCenter);
        //float angle = acos(dot(cylinderRightDir, pointDir));

        //#2 solve
        float s = length(cross(cylinderRightDir, pointDir));
        float c = dot(cylinderRightDir, pointDir);
        float angle = atan(s,c);

        float signedAngle = angleSign * angle;
       // float signedAngle = angle;
       // if(angleSign < 0.0){
       //     signedAngle = 2.0 * 3.14159265 - angle;
       // }
        // normalize values, and change z

        distFromBase = (heightSign * (distFromBase / cylinderHeight) * 2.0) - 1.0;
        //signedAngle = signedAngle/(3.14159265);// - 1.0;
        //signedAngle = 0;
        distFromCenter = (cylinderRadius - distFromCenter) / cylinderRadius;
        //distFromCenter = (distFromCenter) / cylinderRadius;

   if(outputType){
        ouposition = vec4(position, 1.0);//vec4(signedAngle, distFromBase, distFromCenter, 1.0);//vec4(position, 1.0);// projection * eyeCoordinatePos;
    }
   else{
       ouposition = vec4(normal, 1.0);
   }
   //gl_Position = vec4(signedAngle, distFromBase, distFromCenter, 1.0);
    datas = vec3(signedAngle, distFromBase, distFromCenter);
}

