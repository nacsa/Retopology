#ifndef SIMPLESCENE_H
#define SIMPLESCENE_H

#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl3w.h>
#include <vector>

#include "scene.h"
#include "glslprogram.h"
#include "shader.hpp"
#include "framebuffer.hpp"
#include "topology.h"
#include "camera.h"
#include "extrudeedge.h"
#include "projectmanager.h"
#include "cylinderprojection.h"
#include "linearregressor.h"
#include "selectionrectangle.h"
#include "similarmodelprojection.h"

class SimpleScene : public Scene
{
public:
    struct ExtrudePoint{
        unsigned int pointId;
        int poindIndex; // speed
        glm::vec2 extrudeDir;
        glm::vec2 cameraPos;

    };


private:
    ProjectManager projectManager;
    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint indicesBuffer;
    int width, height, nVertex;
    GLuint vboHandles[3];
    GLuint vaoHandle[6];
    GLuint texture[2];
    GLSLProgram shader;
    Shader* altshader;
    Shader* topology_shader;
    Shader* newpoint_shader;
    Shader* pointid_shader;
    Shader* edgeid_shader;
    Shader* pointnormal_shader;
    Shader* edge_shader;
    Shader* quad_shader;
    Shader* triangle_shader;
    Shader* triangleid_shader;
    Shader* cylinderProjection_shader;
    Shader* similarModelProjectionPosition_shader;
    Shader* similarModelProjectionNormal_shader;
    glm::mat4 projection, modelview, rotationMatrix;
    glm::mat3 normalmatrix;
    glm::vec3 lightPosition;

    Framebuffer* cylinderPositionFrameBuffer;
    Framebuffer* cylinderNormalFrameBuffer;
    Framebuffer* similarPositionFrameBuffer;
    Framebuffer* similarNormalFrameBuffer;

    Framebuffer* positionBuffer;
    Framebuffer* positionFrameBuffer;
    Framebuffer* positionBackFaceFrameBuffer;
    Framebuffer* normalFrameBuffer;
    Framebuffer* pointIdFrameBuffer;
    Framebuffer* pointIdSnapFrameBuffer;
    Framebuffer* edgeIdFrameBuffer;
    Framebuffer* triangleIdFrameBuffer;
    bool sUpdatePositionFB;
    bool sUpdatePositionBackFaceFB;
    bool sUpdateNormalFB;
    bool sUpdatePointIdFB;
    bool sUpdatePointIdSnapFB;
    bool sUpdateEdgeIdFB;
    bool sUpdateTriangleIdFB;

    Topology topology;
    CylinderProjection cylinderProj;
    SimilarModelProjection similarProj;
    SelectionRectangle selectionRectangle;


    glm::vec3 cameraPos;
    glm::vec3 cameraDir;
    glm::vec3 cameraUp;
    Camera camera;
    glm::vec3 centerPos;
    float rotAngleX;
    float rotAngleY;
    glm::vec3 rotX;
    glm::vec3 rotY;

    std::map<unsigned int, ExtrudePoint> extrudePointMap;
    int extrudeInitX;
    int extrudeInitY;

    std::set<unsigned int> pointIslandSelectionPoints;

    bool snapping;
    unsigned int snappingPointId;

    bool drawing;
    std::list<glm::vec3> drawedPoints;
    std::list<glm::vec3> drawedBackFacePoints;

    float zoomValue;
    //return the ID of the Point on the x,y koord, or 0 if there is no Point
    unsigned int getSelectedPointId(int x, int y);
    std::set<unsigned int> getSelectedPointsIds();
    unsigned int getSelectedEdgeId(int x, int y);
    unsigned int getSelectedTriangleId(int x, int y);
    glm::vec3 getPointNormal(int x, int y);
    float* getPointPosition(int x, int y);
    glm::vec3 getBackFacePointPosition(int x, int y);
    glm::vec3 getPointPositionWithMatrix(int x, int y, glm::mat4& modelMatrix);
    glm::vec3 getPointNormalWithMatrix(int x, int y, glm::mat4& modelMatrix);
    glm::vec3 getPointPositionForSimilarProjection(int x, int y, glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
    glm::vec3 getPointNormalForSimilarProjection(int x, int y, glm::mat4& viewMatrix, glm::mat4& projectionMatrix);
    std::vector<glm::vec3> getPointDataViaCylinderShader( bool outputType);

    std::vector<glm::vec3> getLineStartAndEndPoint(std::list<glm::vec3> points);

    void deleteFrameBuffers();
    void createFrameBuffers(int width, int height);
    void updatePositionFB(bool force);
    void updatePositionBackFaceFB(bool force);
    void updateNormalFB(bool force);
    void updatePointIdFB(bool force);
    void updatePointIdSnapFB(bool force);
    void updateEdgeIdFB(bool force);
    void updateTriangleIdFB(bool force);

    void setAllsUpdateFB(bool update);

    void updatePointBuffers();
    void updateEdgeBuffers();
    void updateTriangleBuffers();
    void updateQuadBuffers();

public:
    bool qActive;
    bool tActive;
    SimpleScene();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
    void changeScene(int);
    void rotateScene(float, float);
    void zoomSlide(int);
    void onClickEvent(int, int);
    void moveModel(float, float);

    void loadModelFromFile(const char* fileName);
    void saveProject(const char* fileName);
    void openProject(const char* fileName);

    void makeNewPoint(int x, int y);

    bool setActivePoint(int x, int y);
    bool unsetActivePoint(int x, int y);
    bool addActivePoint(int x, int y);

    bool setActiveEdge(int x, int y);
    bool unsetActiveEdge(int x, int y);
    bool addActiveEdge(int x, int y);

    void makeEdgeBeetweenPoints(int x, int y);
    void removeActivePoint();
    void removeActiveEdge();
    void divideTriangle(int x, int y);
    void divideEdge(int x, int y);

    void moveActivePoint(int x, int y);

    bool extrudeEdge(int x, int y);
    void tmpExtrudeFirstTriangle(int x, int y);
    void initExtrude(std::list<unsigned int> edgeIds, int initX, int initY);
    glm::vec2 vectorHelper(glm::vec2 inpVec);
    unsigned int pointHax(unsigned int pointId, glm::vec2 dir, glm::vec2 cameraPos);

    void stopExtrude();

    void moveExtrudablePoint(int x, int y);

    void selectPointIsland(int x, int y);
    void selectPointIslandAroundPoint(unsigned int pointId);

    void selectBorderEdges();
    void selectBorderEdgeLine(int x, int y);
    void clearEdgeSelection();

    void projectToSimilarModel();

    //haxx


    void addTQuad();
    void addDQuad();
    void removeTQuad();
    void removeDQuad();
    bool getSnapping() const;
    void setSnapping(bool value);
    void indicateSnappingPoint(int x, int y);
    void projectTMPCylinder();
    void projectTMPCylinderViaShader();



    void startDrawingOnSurface(int x, int y);
    void stopDrawingOnSurface(int x, int y);
    void drawingOnSurface(int x, int y);

    void linearRegression();


    void startRectangleSelection(int x, int y);
    void stopRectangleSelection(int x, int y);
    void rectangleSelection(int x, int y);



};

#endif
