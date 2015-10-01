#include "linearregressor.h"

#define M_PI 3.14159265

float LinearRegressor::sum(float *x, int nX)
{
    float sum = 0;
    for(int i = 0; i < nX; i++){
        sum += x[i];
    }
    return sum;
}

float LinearRegressor::sum(float *x1, float *x2, int nX)
{
    float sum = 0;
    for(int i = 0; i < nX; i++){
        sum += x1[i]*x2[i];
    }
    return sum;
}

float LinearRegressor::sumMinus(float *x1, float *x2, int nX)
{
    float sum = 0;
    for(int i = 0; i < nX; i++){
        sum += (x1[i]-x2[i])*(x1[i]-x2[i]);
    }
    return sum;
}

LinearRegressor::LinearRegressor()
{
}

float* LinearRegressor::pointsToLineRegression(std::list<glm::vec3> points)
{
    int n = points.size();
    float n_f = (float)n;
    float* x_k = new float[n];
    float* y_k = new float[n];
    float* z_k = new float[n];

    int i = 0;
    for(auto point : points){
        x_k[i] = point.x;
        y_k[i] = point.y;
        z_k[i] = point.z;
        i++;
    }
    float dx = sumMinus(x_k, x_k+1, n-1);
    float dy = sumMinus(y_k, y_k+1, n-1);
    float dz = sumMinus(z_k, z_k+1, n-1);

    float* tmpPointer;

    int xi = 0;
    int yi = 1;
    int zi = 2;
    if(dx > dz && dx > dy){
        tmpPointer = x_k;
        x_k = z_k;
        z_k = tmpPointer;
        xi=2;
        zi=0;
    }else if(dy > dz){
        tmpPointer = y_k;
        y_k = z_k;
        z_k = tmpPointer;
        yi=2;
        zi=1;
    }


    float x_m = sum(x_k, n) / n_f;
    float y_m = sum(y_k, n) / n_f;
    float z_m = sum(z_k, n) / n_f;

    float s_xx = - x_m * x_m + sum(x_k, x_k, n) / n_f;
    float s_yy = - y_m * y_m + sum(y_k, y_k, n) / n_f;
    float s_zz = - z_m * z_m + sum(z_k, z_k, n) / n_f;
    float s_xy = - x_m * y_m + sum(x_k, y_k, n) / n_f;
    float s_xz = - x_m * z_m + sum(x_k, z_k, n) / n_f;
    float s_yz = - y_m * z_m + sum(y_k, z_k, n) / n_f;

    float teta = glm::atan((2.0 * s_xy) / (s_xx - s_yy)) / 2.0;

    float cosTeta = glm::cos(teta);
    float sinTeta = glm::sin(teta);

    float k_11 =  (s_yy + s_zz) * cosTeta * cosTeta
                + (s_xx + s_zz) * sinTeta * sinTeta
                - 2.0 * s_xy * cosTeta * sinTeta;

    float k_22 =  (s_yy + s_zz) * sinTeta * sinTeta
                + (s_xx + s_zz) * cosTeta * cosTeta
                + 2.0 * s_xy * cosTeta * sinTeta;

    float k_12 = - s_xy * (cosTeta * cosTeta - sinTeta * sinTeta)
                + (s_xx - s_yy) * cosTeta * sinTeta; // k_12 should be 0

    float k_10 = s_xz * cosTeta + s_yz * sinTeta;

    float k_01 = -s_xz * sinTeta + s_yz * cosTeta;

    float k_00 = s_xx + s_yy;


    float c2 = -k_00 - k_11 - k_22;
    float c1 = k_00 * k_11 + k_00 * k_22 + k_11 * k_22 - k_01 * k_01 - k_10 * k_10;
    float c0 = k_01 * k_01 * k_11 + k_10 * k_10 * k_22 - k_00 * k_11 * k_22;

    float p = c1 - (c2*c2)/3.0;
    float q = (2.0 * c2*c2*c2)/27.0 - (c1*c2)/3.0 + c0;
    float r = (q*q)/4.0 + (p*p*p)/27.0;

    float deltaM2;

    if(r > 0.0){
        float val1 = -q/2.0 + glm::sqrt(r);
        float val2 = -q/2.0 - glm::sqrt(r);
        deltaM2 = -c2/3.0 + std::pow(val1, 1/3.) + std::pow(val2, 1/3.);
    }else{
        float g = glm::sqrt(-(p*p*p)/27.0);
        float omega = glm::acos(-q / (2.0 * g));

        float min_val1 = -c2/3.0 + 2.0 * std::pow(g, 1/3.) * glm::cos(omega / 3.0);
        float min_val2 = -c2/3.0 + 2.0 * std::pow(g, 1/3.) * glm::cos((omega + 2.0 * M_PI)/ 3.0);
        float min_val3 = -c2/3.0 + 2.0 * std::pow(g, 1/3.) * glm::cos((omega + 4.0 * M_PI) / 3.0);

        float tmpMinVal = std::min(min_val1, min_val2);
        deltaM2 = std::min(tmpMinVal, min_val3);
    }

    float a = (-k_10 * cosTeta) / (k_11 - deltaM2*deltaM2) + (k_01 * sinTeta) / (k_22 - deltaM2*deltaM2);

    float b = (-k_10 * sinTeta) / (k_11 - deltaM2*deltaM2) + (-k_01 * cosTeta) / (k_22 - deltaM2*deltaM2);

    float coeff = 1 + a * a + b * b;

    float u = ((1 + b*b) * x_m - a * b * y_m + a * z_m) / coeff;
    float v = (-a * b * x_m + (1 + a*a) * y_m + b * z_m) / coeff;
    float w = (a * x_m + b * y_m + (a*a + b*b) * z_m) / coeff;

    float retValue[6];

    retValue[xi] = x_m;
    retValue[yi] = y_m;
    retValue[zi] = z_m;
    retValue[xi+3] = u;
    retValue[yi+3] = v;
    retValue[zi+3] = w;

    delete[] x_k;
    delete[] y_k;
    delete[] z_k;

    return retValue;
}
