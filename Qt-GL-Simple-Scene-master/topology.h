#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include "topologyedge.h"
#include "topologypoint.h"
#include "topologyhelperquad.h"
#include "topologyhelpertriangle.h"
#include "camera.h"
#include "extrudeedge.h"

#include <GL/gl3w.h>
#include <vector>
#include <list>
#include <set>
#include <map>


class Topology
{    
private:
    //buffers
     GLuint pointVertexBuffer;
     GLuint pointNormalBuffer;
     GLuint pointIdBuffer;
     GLuint pointActiveBuffer;

     GLuint edgeVertexBuffer;
     GLuint edgeNormalBuffer;
     GLuint edgeIdBuffer;
     GLuint edgeActiveBuffer;
     GLuint edgeBorderBuffer;


     GLuint triangleVertexBuffer;
     GLuint triangleNormalBuffer;
     GLuint triangleIdBuffer;
     GLuint triangleActiveBuffer;

     //GLuint quadIndices;


     //not used
     GLuint quadIndicesBuffer;
     GLuint triangleIndicesBuffer;


    //ID index to Buffer index
    //void edgesToLines();
    //bool equalEdges(EdgeIndex a, EdgeIndex b);

    //Id->index
    std::map<unsigned int, int> pointId_index;
    std::map<unsigned int, int> edgeId_index;
    std::map<unsigned int, int> quadId_index;
    std::map<unsigned int, int> triangleId_index;

public:
    unsigned int activePointId;
    std::vector<TopologyPoint> newPoints;
    std::vector<float> newPointsPos;
    std::vector<float> newPointsNormal;
    std::vector<float> newPointsId;
    std::vector<float> newPointsActive;

    std::vector<TopologyHelperQuad> quads;
    std::vector<unsigned int> quadIndices;

    std::vector<TopologyHelperTriangle> triangles;
    //std::vector<unsigned int> triangleIndices;
    std::vector<float> trianglePos;
    std::vector<float> triangleNormal;
    std::vector<float> triangleId;

   // unsigned int activeEdgeId;
    std::vector<TopologyEdge> edges;
    std::vector<float> newEdgePos;
    std::vector<float> newEdgeNormal;
    std::vector<float> newEdgeId;
    std::vector<float> newEdgeActive;
    std::vector<float> newEdgeBorder;

    //bufferhez indexek
    std::vector<unsigned int> lineIndieces;

    //ID indexek


    std::set<unsigned int> activePoints;

    std::set<unsigned int> activeEdges;
    std::set<unsigned int> borderEdges;


    Topology();
    void bindPointBuffers(GLuint vaoHandle);
    void bindEdgeBuffers(GLuint vaoHandle);
    void bindTriangleBuffers(GLuint vaoHandle);
    void bindQuadBuffers(GLuint vaoHandle);

    void clearPointBuffers();
    void clearEdgeBuffers();
    void clearTriangleBuffers();
    void clearQuadBuffers();

    unsigned int addEdge(unsigned int pointId1, unsigned int pointId2);
    void removeEdge(unsigned int edgeId, unsigned int pointId=0);
    void setActiveEdge(unsigned int id);
    //void edge

    unsigned int addPoint(TopologyPoint p);
    void removePoint(unsigned int id);
    void setActivePoint(unsigned int id);
    void movePoint(unsigned int id, float posX, float posY, float posZ, float normX, float normY, float normZ);
    int pointIdToIndex(unsigned int id);
    int edgeIdToIndex(unsigned int id);
    int triangleIdToIndex(unsigned int id);
    int quadIdToIndex(unsigned int id);

    void resetTopology();

    void testEdgeBorder(unsigned int edgeId);
    void addTriangle(unsigned int edgeId, unsigned int pointId1, unsigned int pointId2, unsigned int pointId3);
    void removeTriangle(unsigned int id, unsigned int edgeId);
    bool addQuad(unsigned int edgeId1, unsigned int pointId1, unsigned int pointId2, unsigned int pointId3, unsigned int pointId4);
    void removeQuad(unsigned int id, unsigned int edgeId);


    bool testTriangulation(unsigned int edgeId1);
    bool testQuadration(unsigned int edgeId);

    ExtrudeEdge extrudeEdge(unsigned int edgeId, glm::mat4 modelviewMatrix, glm::mat4 projectionMatrix, Camera camera);
    glm::vec3 getSideEdgeDirection(unsigned int edgeId);

    //haxx
    void addTQuad();
    void addDQuad();
    void removeTQuad();
    void removeDQuad();


    void addActivePoint(unsigned int pointId);
    void removeActivePoint(unsigned int pointId);
    void clearActivePoint();
    bool isPointActive(unsigned int pointId);
    std::set<unsigned int> getActivePoints();
    unsigned int getRandomActivePoint();


    void addActiveEdge(unsigned int edgeId);
    void removeActiveEdge(unsigned int edgeId);
    void clearActiveEdge();
    bool isEdgeActive(unsigned int edgeId);
    std::set<unsigned int> getActiveEdges();
    std::list<unsigned int> getBorderEdgeLine(unsigned int edgeId);
    unsigned int* nextValidBorderEdge(unsigned int firstEdge, unsigned int previousEdge, unsigned int previousPoint);


    void addBorderEdge(unsigned int edgeId);
    void removeBorderEdge(unsigned int edgeId);
    bool isEdgeBorder(unsigned int edgeId);

    void setBorderEdgesActive();

};

#endif // TOPOLOGY_H