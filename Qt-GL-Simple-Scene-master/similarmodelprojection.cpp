#include "similarmodelprojection.h"
#include <glm/gtc/matrix_transform.hpp>

SimilarModelProjection::SimilarModelProjection()
{
    projectionDistance = 0.3f;
    nearPlane = 0.01f;
    farPlane = projectionDistance * 2.0f;

    projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -projectionDistance, projectionDistance);
    // TODO: ORHTO vetítés, **done
    // 1x1 FB elég kell hogy legyen, ORTHO MÉRETEKET átírni megfelelő kicsire
    // vertex shader z -> |Z| **done
    // 0,0,0 -ba ne menjen csak ha tényleg oda kell
    // vertex island kiválasztása
    // elforgatás, skálázás elforgatás
    // több fázisú eszköznek impl. alul felül látszik távolságok
    // több fázisú henger vetítés impl.

    // Értékelés összehasonlítani más eszközökkel. interaktivitás, gyorsaság. Nagyfelbontású geometria beolvasás.
    // Kisfelbontású modell vertexszám növelésének hatása.

    // felület kipofozása! Okosan megtervezni a felületet, triviálos dolgokat lehet, mit - hova - miért?
    // egyes arc részleteket nagyfelbontású modelből szétszabdalni

    //jövőbeni fejlesztése: ruha szim kényszerek integrálás
}

void SimilarModelProjection::projectToModel(std::vector<glm::vec3> pointPositions, std::vector<glm::vec3> pointNormals, Topology &topology)
{

}

glm::mat4 SimilarModelProjection::getProjectionMatrix()
{
    return projectionMatrix;
}

glm::mat4 SimilarModelProjection::getViewMatrix(TopologyPoint point)
{
    glm::vec3 pointNormal = point.getNormal();

    glm::vec3 eye = point.getPosition();
    glm::vec3 center = eye - pointNormal;
    glm::vec3 upDir = glm::cross(pointNormal, glm::vec3(1,0,0));
    if(upDir.x == 0 && upDir.y == 0 && upDir.z == 0){
        upDir = glm::cross(pointNormal, glm::vec3(0,1,0));
    }
    return glm::lookAt(eye, center, upDir);
}
