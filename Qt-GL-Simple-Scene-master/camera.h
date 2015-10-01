#ifndef CAMERA_H
#define CAMERA_H

#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 up;

    Camera();
};

#endif // CAMERA_H
