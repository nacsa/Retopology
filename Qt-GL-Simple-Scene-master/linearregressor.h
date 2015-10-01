#ifndef LINEARREGRESSOR_H
#define LINEARREGRESSOR_H

#include <glm/glm.hpp>
#include <list>

class LinearRegressor
{
private:
    static float sum(float* x, int nX);
    static float sum(float* x1, float* x2, int nX);
    static float sumMinus(float* x1, float* x2, int nX);
public:
    LinearRegressor();
    static float *pointsToLineRegression(std::list<glm::vec3> points);
};

#endif // LINEARREGRESSOR_H
