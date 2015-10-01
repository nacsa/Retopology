#include "topology.h"
#include <algorithm>
Topology::Topology()
{
    activePointId=0;
    //activeEdgeId = 0;
    pointVertexBuffer=0;
    pointNormalBuffer=0;
    pointIdBuffer=0;
    pointActiveBuffer=0;

    edgeVertexBuffer=0;
    edgeNormalBuffer=0;
    edgeIdBuffer=0;
    edgeActiveBuffer=0;
    edgeBorderBuffer=0;

    triangleVertexBuffer = 0;
    triangleNormalBuffer = 0;
    triangleIdBuffer = 0;

    triangleIndicesBuffer=0;
    quadIndicesBuffer=0;
}

/*
void Topology::edgesToLines(){
    lineIndieces.resize(edges.size()*2);

    for(int i = 0; i < newPointsId.size(); i++){
        for(int j = 0; j < edges.size(); j++){
            if(edges[j].index1==(unsigned int)newPointsId[i]){
                lineIndieces[2*j] = i;
            }else if(edges[j].index2==(unsigned int)newPointsId[i]){
                lineIndieces[2*j+1] = i;
            }
        }
    }
}


bool equalEdges(Topology::EdgeIndex a, Topology::EdgeIndex b){
    return (a.index1 == b.index1 && a.index2 == b.index2) ||
           (a.index1 == b.index2 && a.index2 == b.index1);
}
*/

unsigned int Topology::addEdge(TopologyEdge edge)
{
    TopologyEdge tmpEdge = edge;
    unsigned int pointId1 = tmpEdge.pointId1;
    unsigned int pointId2 = tmpEdge.pointId2;

    int index1 = pointIdToIndex(pointId1);
    if(newPoints[index1].isPointIdNeighbor(pointId2)){
        return 0;
    }

    edges.push_back(tmpEdge);
    unsigned int index2 = pointIdToIndex(pointId2);
    newPoints[index1].addNeighborPoint(pointId2);
    newPoints[index1].addNeighborEdge(tmpEdge.getId());
    newPoints[index2].addNeighborPoint(pointId1);
    newPoints[index2].addNeighborEdge(tmpEdge.getId());


    //arrayok frissitese
    newEdgePos.push_back(newPointsPos[index1*3]);
    newEdgePos.push_back(newPointsPos[index1*3+1]);
    newEdgePos.push_back(newPointsPos[index1*3+2]);
    newEdgeNormal.push_back(newPointsNormal[index1*3]);
    newEdgeNormal.push_back(newPointsNormal[index1*3+1]);
    newEdgeNormal.push_back(newPointsNormal[index1*3+2]);

    newEdgePos.push_back(newPointsPos[index2*3]);
    newEdgePos.push_back(newPointsPos[index2*3+1]);
    newEdgePos.push_back(newPointsPos[index2*3+2]);
    newEdgeNormal.push_back(newPointsNormal[index2*3]);
    newEdgeNormal.push_back(newPointsNormal[index2*3+1]);
    newEdgeNormal.push_back(newPointsNormal[index2*3+2]);

    newEdgeId.push_back((float)tmpEdge.getId());
    newEdgeId.push_back((float)tmpEdge.getId());
    newEdgeActive.push_back(0.0);
    newEdgeActive.push_back(0.0);
    newEdgeBorder.push_back(1.0);
    newEdgeBorder.push_back(1.0);


    //edge to id
    edgeId_index[tmpEdge.getId()]=edges.size()-1;

    addBorderEdge(tmpEdge.getId());

    return tmpEdge.getId();

}

unsigned int Topology::addEdge(unsigned int pointId1, unsigned int pointId2){
//    edges.push_back(EdgeIndex(index1, index2));
//    std::unique(edges.begin(),edges.end(), equalEdges);
//    edgesToLines();

    if(pointId1 == pointId2)
        return 0;

    //1. meg kell vizsgálni van-e él...
    unsigned int index1 = pointIdToIndex(pointId1);
    if(newPoints[index1].isPointIdNeighbor(pointId2)){
        return 0;
    }
    TopologyEdge tmpEdge = TopologyEdge(pointId1, pointId2);
    addEdge(tmpEdge);

    edges.push_back(tmpEdge);

    unsigned int index2 = pointIdToIndex(pointId2);
    newPoints[index1].addNeighborPoint(pointId2);
    newPoints[index1].addNeighborEdge(tmpEdge.getId());
    newPoints[index2].addNeighborPoint(pointId1);
    newPoints[index2].addNeighborEdge(tmpEdge.getId());


    //arrayok frissitese
    newEdgePos.push_back(newPointsPos[index1*3]);
    newEdgePos.push_back(newPointsPos[index1*3+1]);
    newEdgePos.push_back(newPointsPos[index1*3+2]);
    newEdgeNormal.push_back(newPointsNormal[index1*3]);
    newEdgeNormal.push_back(newPointsNormal[index1*3+1]);
    newEdgeNormal.push_back(newPointsNormal[index1*3+2]);

    newEdgePos.push_back(newPointsPos[index2*3]);
    newEdgePos.push_back(newPointsPos[index2*3+1]);
    newEdgePos.push_back(newPointsPos[index2*3+2]);
    newEdgeNormal.push_back(newPointsNormal[index2*3]);
    newEdgeNormal.push_back(newPointsNormal[index2*3+1]);
    newEdgeNormal.push_back(newPointsNormal[index2*3+2]);

    newEdgeId.push_back((float)tmpEdge.getId());
    newEdgeId.push_back((float)tmpEdge.getId());
    newEdgeActive.push_back(0.0);
    newEdgeActive.push_back(0.0);
    newEdgeBorder.push_back(1.0);
    newEdgeBorder.push_back(1.0);


    //edge to id
    edgeId_index[tmpEdge.getId()]=edges.size()-1;

    addBorderEdge(tmpEdge.getId());

    return tmpEdge.getId();

}

void Topology::removeEdge(unsigned int edgeId, unsigned int pointId){
    int edgeIndex = edgeIdToIndex(edgeId);
    unsigned int index1 = pointIdToIndex(edges[edgeIndex].pointId1);
    unsigned int index2 = pointIdToIndex(edges[edgeIndex].pointId2);

    if(isEdgeActive(edgeId))
        removeActiveEdge(edgeId);
    if(isEdgeBorder(edgeId))
        removeBorderEdge(edgeId);

    //ha aktiv volt kiszedjuk ;;feljebb
    //if(activeEdgeId == edgeId){
    //    activeEdgeId=0;
    //}

    //pontok karbantartasa
    if(pointId !=edges[edgeIndex].pointId1){
        newPoints[index1].removeNeighborPoint(edges[edgeIndex].pointId2);
        newPoints[index1].removeNeighborEdge(edgeId);
    }
    if(pointId !=edges[edgeIndex].pointId2){
        newPoints[index2].removeNeighborPoint(edges[edgeIndex].pointId1);
        newPoints[index2].removeNeighborEdge(edgeId);
    }
    if(!edges[edgeIndex].neighborTriangles.empty()){
        for(std::list<unsigned int>::iterator it = edges[edgeIndex].neighborTriangles.begin(); it != edges[edgeIndex].neighborTriangles.end(); it++){
            removeTriangle((*it), edgeId);
        }
        edges[edgeIndex].neighborTriangles.clear();
    }
    if(!edges[edgeIndex].neighborQuads.empty()){
        for(std::list<unsigned int>::iterator it = edges[edgeIndex].neighborQuads.begin(); it != edges[edgeIndex].neighborQuads.end(); it++){
            removeQuad((*it), edgeId);
        }
        edges[edgeIndex].neighborQuads.clear();
    }
    //arrayok karbantartasa
    newEdgePos.erase(newEdgePos.begin()+edgeIndex*6,newEdgePos.begin()+edgeIndex*6+6);
    newEdgeNormal.erase(newEdgeNormal.begin()+edgeIndex*6,newEdgeNormal.begin()+edgeIndex*6+6);
    newEdgeId.erase(newEdgeId.begin()+edgeIndex*2,newEdgeId.begin()+edgeIndex*2+2);
    newEdgeActive.erase(newEdgeActive.begin()+edgeIndex*2,newEdgeActive.begin()+edgeIndex*2+2);
    newEdgeBorder.erase(newEdgeBorder.begin()+edgeIndex*2,newEdgeBorder.begin()+edgeIndex*2+2);
    edges.erase(edges.begin()+edgeIndex);

    //id to index
    edgeId_index.clear();
    for(int i = 0; i < edges.size(); i++){
        edgeId_index[edges[i].getId()]=i;
    }

}

void Topology::setActiveEdge(unsigned int id)
{
    /*
    if(activeEdgeId == id){
        return;
    }
*/
    clearActiveEdge();

    //activeEdgeId=id;
    addActiveEdge(id);
    //arrayok karbantartása

}

unsigned int Topology::addPoint(TopologyPoint p)
{
    //arrayok karbantartasa
    newPoints.push_back(p);
    newPointsPos.push_back(p.getPosition().x);
    newPointsPos.push_back(p.getPosition().y);
    newPointsPos.push_back(p.getPosition().z);

    newPointsNormal.push_back(p.getNormal().x);
    newPointsNormal.push_back(p.getNormal().y);
    newPointsNormal.push_back(p.getNormal().z);

    newPointsId.push_back((float)p.getId());
    newPointsActive.push_back(0.0);

    pointId_index[p.getId()]=newPoints.size()-1;

    return p.getId();
}

void Topology::removePoint(unsigned int id)
{
    int pointIndex = pointIdToIndex(id);
    //minden kimeno el torlese
    for(std::list<unsigned int>::iterator it = newPoints[pointIndex].neighborEdges.begin(); it != newPoints[pointIndex].neighborEdges.end(); it++){
        removeEdge(*it,id);
    }
    //ha aktiv volt kiszedjuk
    //if(activePointId == id){
    //    activePointId=0;
    //}
    removeActivePoint(id);

    //arrayok karbantartasa
    newPointsPos.erase(newPointsPos.begin()+pointIndex*3,newPointsPos.begin()+pointIndex*3+3);
    newPointsNormal.erase(newPointsNormal.begin()+pointIndex*3,newPointsNormal.begin()+pointIndex*3+3);
    newPointsId.erase(newPointsId.begin()+pointIndex);
    newPointsActive.erase(newPointsActive.begin()+pointIndex);
    newPoints.erase(newPoints.begin()+pointIndex);

    //id to index
    pointId_index.clear();
    for(int i = 0; i < newPoints.size(); i++){
        pointId_index[newPoints[i].getId()]=i;
    }

/*
    triangleIndices.clear();
    triangleIndices.resize(triangles.size()*3);
    for(int i = 0; i < triangles.size(); i++){
        triangleIndices[i*3] = pointIdToIndex(triangles[i].pointId1);
        triangleIndices[i*3+1] = pointIdToIndex(triangles[i].pointId2);
        triangleIndices[i*3+2] = pointIdToIndex(triangles[i].pointId3);
    }*/
    quadIndices.clear();
    quadIndices.resize(quads.size()*4);
    for(int i = 0; i < quads.size(); i++){
        quadIndices[i*4] = pointIdToIndex(quads[i].pointId1);
        quadIndices[i*4+1] = pointIdToIndex(quads[i].pointId2);
        quadIndices[i*4+2] = pointIdToIndex(quads[i].pointId4);
        quadIndices[i*4+3] = pointIdToIndex(quads[i].pointId3);
    }

}

void Topology::setActivePoint(unsigned int id)
{
    /*
    if(activePointId == id){
        return;
    }
    unsigned int oldActiveId = activePointId;
    int pointIndex = pointIdToIndex(id);

    activePointId=id;

    //arrayok karbantartása
    newPointsActive[pointIndex] = 1.0;
    if(oldActiveId != 0){
        newPointsActive[pointIdToIndex(oldActiveId)] = 0.0;
    }
    //clear active edge
    clearActiveEdge();
    //if(activeEdgeId!=0){
    //    newEdgeActive[edgeIdToIndex(activeEdgeId)*2] = 0.0;
    //    newEdgeActive[edgeIdToIndex(activeEdgeId)*2+1] = 0.0;
    //    activeEdgeId = 0;
    //}
    */

    clearActivePoint();
    addActivePoint(id);
}

void Topology::movePoint(unsigned int id, float posX, float posY, float posZ, float normX, float normY, float normZ)
{
    int index = pointIdToIndex(id);
    newPoints[index].setPosition(posX,posY,posZ);
    newPointsPos[index*3] = posX;
    newPointsPos[index*3+1] = posY;
    newPointsPos[index*3+2] = posZ;

    newPointsNormal[index*3] = normX;
    newPointsNormal[index*3+1] = normY;
    newPointsNormal[index*3+2] = normZ;

    int edgeIx;
    int triangleIx;
    std::list<unsigned int>::iterator edgeIt;
    for(std::list<unsigned int>::iterator it = newPoints[index].neighborEdges.begin(); it != newPoints[index].neighborEdges.end(); it++){
        edgeIx = edgeIdToIndex(*it);
        if(edges[edgeIx].pointId1 == id)
        {
            newEdgePos[edgeIx*6] = posX;
            newEdgePos[edgeIx*6+1] = posY;
            newEdgePos[edgeIx*6+2] = posZ;
            newEdgeNormal[edgeIx*6] = normX;
            newEdgeNormal[edgeIx*6+1] = normY;
            newEdgeNormal[edgeIx*6+2] = normZ;
        }else if(edges[edgeIx].pointId2 == id){
            newEdgePos[edgeIx*6+3] = posX;
            newEdgePos[edgeIx*6+4] = posY;
            newEdgePos[edgeIx*6+5] = posZ;
            newEdgeNormal[edgeIx*6+3] = normX;
            newEdgeNormal[edgeIx*6+4] = normY;
            newEdgeNormal[edgeIx*6+5] = normZ;
        }
        for(edgeIt = edges[edgeIx].neighborTriangles.begin(); edgeIt != edges[edgeIx].neighborTriangles.end();edgeIt++){
            triangleIx = triangleIdToIndex(*edgeIt);
            if(triangles[triangleIx].pointId1 == id)
            {
                trianglePos[triangleIx*9] = posX;
                trianglePos[triangleIx*9+1] = posY;
                trianglePos[triangleIx*9+2] = posZ;
                triangleNormal[triangleIx*9] = normX;
                triangleNormal[triangleIx*9+1] = normY;
                triangleNormal[triangleIx*9+2] = normZ;
            }else if(triangles[triangleIx].pointId2 == id)
            {
                trianglePos[triangleIx*9+3] = posX;
                trianglePos[triangleIx*9+4] = posY;
                trianglePos[triangleIx*9+5] = posZ;
                triangleNormal[triangleIx*9+3] = normX;
                triangleNormal[triangleIx*9+4] = normY;
                triangleNormal[triangleIx*9+5] = normZ;
            } else if(triangles[triangleIx].pointId3 == id)
            {
                trianglePos[triangleIx*9+6] = posX;
                trianglePos[triangleIx*9+7] = posY;
                trianglePos[triangleIx*9+8] = posZ;
                triangleNormal[triangleIx*9+6] = normX;
                triangleNormal[triangleIx*9+7] = normY;
                triangleNormal[triangleIx*9+8] = normZ;
            }
        }
    }




}

int Topology::pointIdToIndex(unsigned int id)
{
    return pointId_index[id];
}

int Topology::triangleIdToIndex(unsigned int id)
{
    return triangleId_index[id];
}

int Topology::quadIdToIndex(unsigned int id)
{
    return quadId_index[id];

}

int Topology::edgeIdToIndex(unsigned int id)
{
    return edgeId_index[id];
}


void Topology::testEdgeBorder(unsigned int edgeId)
{
    int innerEdgeIndex = edgeIdToIndex(edgeId);

    if(edges[innerEdgeIndex].neighborTriangles.size() + edges[innerEdgeIndex].neighborQuads.size() > 1){
        removeBorderEdge(edgeId);
        //newEdgeBorder[innerEdgeIndex*2] = 0.0;
        //newEdgeBorder[innerEdgeIndex*2+1] = 0.0;
    }else{
        addBorderEdge(edgeId);
        //newEdgeBorder[innerEdgeIndex*2] = 1.0;
        //newEdgeBorder[innerEdgeIndex*2+1] = 1.0;
    }
}

void Topology::addTriangle(unsigned int edgeId1, unsigned int pointId1, unsigned int pointId2, unsigned int pointId3)
{
    int edgeIndex = edgeIdToIndex(edgeId1);
    TopologyHelperTriangle triangle = TopologyHelperTriangle(pointId1, pointId2, pointId3);
    triangle.edgeId1 =edgeId1;
    int index1 = pointIdToIndex(pointId1);
    int index2 = pointIdToIndex(pointId2);
    int index3 = pointIdToIndex(pointId3);

    //triangleIndices.push_back(index1);
    //triangleIndices.push_back(index2);
    //triangleIndices.push_back(index3);


    edges[edgeIndex].addNeighborTriangle(triangle.getId());
    //TODO osszes edgere ami kozoskiveve magara

    int edgeIx;
    bool first = true;
    for(std::list<unsigned int>::iterator it = newPoints[index3].neighborEdges.begin(); it != newPoints[index3].neighborEdges.end(); it++){
        edgeIx = edgeIdToIndex(*it);
        if(edges[edgeIx].pointId1 == pointId1 || edges[edgeIx].pointId2 == pointId1 ||
                edges[edgeIx].pointId1 == pointId2 ||edges[edgeIx].pointId2 == pointId2)
        {
            if(first){
                first = false;
                triangle.edgeId2 = (*it);
            }else{
                triangle.edgeId3 = (*it);
            }
            edges[edgeIx].addNeighborTriangle(triangle.getId());
        }
    }


     // TODO EZ IGY HANYINGER POINTOK IS TAROLJAK QUAD+TRIANGLE VAGY DUNNO
    unsigned int removeQuadId = 0;

    int edgeIndex2 = edgeIdToIndex(triangle.edgeId2);
    int edgeIndex3 = edgeIdToIndex(triangle.edgeId3);
    std::list<unsigned int>::iterator findIter;
    for(std::list<unsigned int>::iterator it = edges[edgeIndex2].neighborQuads.begin(); it != edges[edgeIndex2].neighborQuads.end(); it++){
        findIter = std::find(edges[edgeIndex3].neighborQuads.begin(), edges[edgeIndex3].neighborQuads.end(), (*it));
        if(findIter != edges[edgeIndex3].neighborQuads.end()){ // van ilyen
            removeQuadId = (*it);
            break;
        }
    }/*
    if(removeQuadId == 0){
        for(std::list<unsigned int>::iterator it = edges[edgeIndex].neighborQuads.begin(); it != edges[edgeIndex].neighborQuads.end(); it++){
            findIter = std::find(edges[edgeIndex2].neighborQuads.begin(), edges[edgeIndex2].neighborQuads.end(), (*it));
            if(findIter != edges[edgeIndex2].neighborQuads.end()){ // van ilyen
                removeQuadId = (*it);
                break;
            }
        }
    }*/

    removeQuad(removeQuadId,0);


    trianglePos.push_back(newPointsPos[index1*3]);
    trianglePos.push_back(newPointsPos[index1*3+1]);
    trianglePos.push_back(newPointsPos[index1*3+2]);
    triangleNormal.push_back(newPointsNormal[index1*3]);
    triangleNormal.push_back(newPointsNormal[index1*3+1]);
    triangleNormal.push_back(newPointsNormal[index1*3+2]);

    trianglePos.push_back(newPointsPos[index2*3]);
    trianglePos.push_back(newPointsPos[index2*3+1]);
    trianglePos.push_back(newPointsPos[index2*3+2]);
    triangleNormal.push_back(newPointsNormal[index2*3]);
    triangleNormal.push_back(newPointsNormal[index2*3+1]);
    triangleNormal.push_back(newPointsNormal[index2*3+2]);

    trianglePos.push_back(newPointsPos[index3*3]);
    trianglePos.push_back(newPointsPos[index3*3+1]);
    trianglePos.push_back(newPointsPos[index3*3+2]);
    triangleNormal.push_back(newPointsNormal[index3*3]);
    triangleNormal.push_back(newPointsNormal[index3*3+1]);
    triangleNormal.push_back(newPointsNormal[index3*3+2]);



    triangleId.push_back((float)triangle.getId());
    triangleId.push_back((float)triangle.getId());
    triangleId.push_back((float)triangle.getId());

    testEdgeBorder(triangle.edgeId1);
    testEdgeBorder(triangle.edgeId2);
    testEdgeBorder(triangle.edgeId3);

    triangles.push_back(triangle);
    triangleId_index[triangle.getId()]=triangles.size()-1;

}

void Topology::removeTriangle(unsigned int id, unsigned int edgeId)
{
    int triangleIndex = triangleIdToIndex(id);
    unsigned int edgeId1 = triangles[triangleIndex].edgeId1;
    unsigned int edgeId2 = triangles[triangleIndex].edgeId2;
    unsigned int edgeId3 = triangles[triangleIndex].edgeId3;

    int edgeIndex1 = edgeIdToIndex(edgeId1);
    int edgeIndex2 = edgeIdToIndex(edgeId2);
    int edgeIndex3 = edgeIdToIndex(edgeId3);



    //edgek karbantartasa
    if(edgeId !=triangles[triangleIndex].edgeId1){
        edges[edgeIndex1].removeNeighborTriangle(id);
    }
    if(edgeId !=triangles[triangleIndex].edgeId2){
        edges[edgeIndex2].removeNeighborTriangle(id);
    }
    if(edgeId !=triangles[triangleIndex].edgeId3){
        edges[edgeIndex3].removeNeighborTriangle(id);
    }


    testEdgeBorder(edgeId1);
    testEdgeBorder(edgeId2);
    testEdgeBorder(edgeId3);

    //arrayok karbantartasa
    //arrayok karbantartasa
    trianglePos.erase(trianglePos.begin()+triangleIndex*9,trianglePos.begin()+triangleIndex*9+9);
    triangleNormal.erase(triangleNormal.begin()+triangleIndex*9,triangleNormal.begin()+triangleIndex*9+9);
    triangleId.erase(triangleId.begin()+triangleIndex*3,triangleId.begin()+triangleIndex*3+3);
    //triangleIndices.erase(triangleIndices.begin()+triangleIndex*3, triangleIndices.begin()+triangleIndex*3+3);
    triangles.erase(triangles.begin()+triangleIndex);

    //id to index
    triangleId_index.clear();
    for(int i = 0; i < triangles.size(); i++){
        triangleId_index[triangles[i].getId()]=i;
    }
}

bool Topology::addQuad(unsigned int edgeId1, unsigned int pointId1, unsigned int pointId2, unsigned int pointId3, unsigned int pointId4)
{
    int edgeIndex = edgeIdToIndex(edgeId1);
    TopologyHelperQuad quad = TopologyHelperQuad(pointId1, pointId2, pointId3, pointId4);
    quad.edgeId1 =edgeId1;
    int index1 = pointIdToIndex(pointId1);
    int index2 = pointIdToIndex(pointId2);
    int index3 = pointIdToIndex(pointId3);
    int index4 = pointIdToIndex(pointId4);





    //TODO osszes edgere ami kozoskiveve magara

    int edgeIx;
    bool first = true;
    for(std::list<unsigned int>::iterator it = newPoints[index3].neighborEdges.begin(); it != newPoints[index3].neighborEdges.end(); it++){
        edgeIx = edgeIdToIndex(*it);
        if(edges[edgeIx].pointId1 == pointId1 || edges[edgeIx].pointId2 == pointId1 ||
                edges[edgeIx].pointId1 == pointId4 ||edges[edgeIx].pointId2 == pointId4)
        {
            if(first){
                first = false;
                quad.edgeId2 = (*it);
            }else{
                quad.edgeId3 = (*it);
            }
            //edges[edgeIx].addNeighborQuad(quad.getId());
        }
    }

    for(std::list<unsigned int>::iterator it = newPoints[index4].neighborEdges.begin(); it != newPoints[index4].neighborEdges.end(); it++){
        edgeIx = edgeIdToIndex(*it);
        if(edges[edgeIx].pointId1 == pointId2 || edges[edgeIx].pointId2 == pointId2)
        {
            quad.edgeId4 = (*it);
            //edges[edgeIx].addNeighborQuad(quad.getId());
        }
    }


    int edgeIndex2 = edgeIdToIndex(quad.edgeId2);
    int edgeIndex3 = edgeIdToIndex(quad.edgeId3);
    int edgeIndex4 = edgeIdToIndex(quad.edgeId4);

    std::list<unsigned int>::iterator findIter;

    //ha 2 oldal a 4ből azonos 3szögben vagy 4szögben van akkor nem vesszük fel.
    for(std::list<unsigned int>::iterator it = edges[edgeIndex].neighborTriangles.begin(); it != edges[edgeIndex].neighborTriangles.end(); it++){
        findIter = std::find(edges[edgeIndex2].neighborTriangles.begin(), edges[edgeIndex2].neighborTriangles.end(), (*it));
        if(findIter != edges[edgeIndex2].neighborTriangles.end()){ // van ilyen
            //Add triangle
            return false;
        }
        findIter = std::find(edges[edgeIndex3].neighborTriangles.begin(), edges[edgeIndex3].neighborTriangles.end(), (*it));
        if(findIter != edges[edgeIndex3].neighborTriangles.end()){ // van ilyen
            //Add triangle
            return false;
        }
    }
    for(std::list<unsigned int>::iterator it = edges[edgeIndex4].neighborTriangles.begin(); it != edges[edgeIndex4].neighborTriangles.end(); it++){
        findIter = std::find(edges[edgeIndex2].neighborTriangles.begin(), edges[edgeIndex2].neighborTriangles.end(), (*it));
        if(findIter != edges[edgeIndex2].neighborTriangles.end()){ // van ilyen
            //Add triangle
            return false;
        }
        findIter = std::find(edges[edgeIndex3].neighborTriangles.begin(), edges[edgeIndex3].neighborTriangles.end(), (*it));
        if(findIter != edges[edgeIndex3].neighborTriangles.end()){ // van ilyen
            //Add triangle
            return false;
        }
    }
    for(std::list<unsigned int>::iterator it = edges[edgeIndex4].neighborTriangles.begin(); it != edges[edgeIndex4].neighborTriangles.end(); it++){
       findIter = std::find(edges[edgeIndex].neighborTriangles.begin(), edges[edgeIndex].neighborTriangles.end(), (*it));
        if(findIter != edges[edgeIndex].neighborTriangles.end()){ // van ilyen
            //Add triangle
            return false;
        }
    }





    //ha minden ok

    quads.push_back(quad);

    edges[edgeIndex].addNeighborQuad(quad.getId());
    edges[edgeIndex2].addNeighborQuad(quad.getId());
    edges[edgeIndex3].addNeighborQuad(quad.getId());
    edges[edgeIndex4].addNeighborQuad(quad.getId());
    quadIndices.push_back(index1);
    quadIndices.push_back(index2);
    quadIndices.push_back(index4);
    quadIndices.push_back(index3);

    testEdgeBorder(quad.edgeId1);
    testEdgeBorder(quad.edgeId2);
    testEdgeBorder(quad.edgeId3);
    testEdgeBorder(quad.edgeId4);

    quadId_index[quad.getId()]=quads.size()-1;
    return true;
}

void Topology::removeQuad(unsigned int id, unsigned int edgeId)
{
    if(id==0)
        return;
    int quadIndex = quadIdToIndex(id);
    unsigned int edgeId1 = quads[quadIndex].edgeId1;
    unsigned int edgeId2 = quads[quadIndex].edgeId2;
    unsigned int edgeId3 = quads[quadIndex].edgeId3;
    unsigned int edgeId4 = quads[quadIndex].edgeId4;

    int edgeIndex1 = edgeIdToIndex(edgeId1);
    int edgeIndex2 = edgeIdToIndex(edgeId2);
    int edgeIndex3 = edgeIdToIndex(edgeId3);
    int edgeIndex4 = edgeIdToIndex(edgeId4);



    //edgek karbantartasa
    if(edgeId !=quads[quadIndex].edgeId1){
        edges[edgeIndex1].removeNeighborQuad(id);
    }
    if(edgeId !=quads[quadIndex].edgeId2){
        edges[edgeIndex2].removeNeighborQuad(id);
    }
    if(edgeId !=quads[quadIndex].edgeId3){
        edges[edgeIndex3].removeNeighborQuad(id);
    }
    if(edgeId !=quads[quadIndex].edgeId4){
        edges[edgeIndex4].removeNeighborQuad(id);
    }


    testEdgeBorder(edgeId1);
    testEdgeBorder(edgeId2);
    testEdgeBorder(edgeId3);
    testEdgeBorder(edgeId4);

    //arrayok karbantartasa
    quadIndices.erase(quadIndices.begin()+quadIndex*4, quadIndices.begin()+quadIndex*4+4);
    quads.erase(quads.begin()+quadIndex);

    //id to index
    quadId_index.clear();
    for(int i = 0; i < quads.size(); i++){
        quadId_index[quads[i].getId()]=i;
    }
}

bool Topology::testTriangulation(unsigned int edgeId1)
{
    int edgeIndex = edgeIdToIndex(edgeId1);
    int index1 = pointIdToIndex(edges[edgeIndex].pointId1);
    int index2 = pointIdToIndex(edges[edgeIndex].pointId2);
    bool success = false;
    newPoints[index1];
    for(std::list<unsigned int>::iterator it = newPoints[index1].neighborPoints.begin(); it != newPoints[index1].neighborPoints.end(); it++){
        std::list<unsigned int>::iterator findIter = std::find(newPoints[index2].neighborPoints.begin(), newPoints[index2].neighborPoints.end(), (*it));
        if(findIter != newPoints[index2].neighborPoints.end()){ // van ilyen
            //Add triangle
            addTriangle(edgeId1, edges[edgeIndex].pointId1, edges[edgeIndex].pointId2, *findIter);
            success = true;
            //TODO: feltölteni gpura
        }
    }

    if(success){

    }

    return success;
}

bool Topology::testQuadration(unsigned int edgeId)
{
    int edgeIndex = edgeIdToIndex(edgeId);
    int index1 = pointIdToIndex(edges[edgeIndex].pointId1);
    int index2 = pointIdToIndex(edges[edgeIndex].pointId2);
    bool success = false;
    //newPoints[index1];
    TopologyPoint longPoint;
    TopologyPoint shortPoint;

    if(newPoints[index1].neighborPoints.size() > newPoints[index2].neighborPoints.size()){
        longPoint = newPoints[index1];
        shortPoint = newPoints[index2];
    }else{
        longPoint = newPoints[index2];
        shortPoint = newPoints[index1];
    }

    std::list<unsigned int>::iterator it;
    std::list<unsigned int>::iterator innerIt;
    std::list<unsigned int>::iterator findIter;

    for(it = longPoint.neighborPoints.begin(); it != longPoint.neighborPoints.end(); it++){
        if((*it) == shortPoint.getId()){
            continue;
        }
        int pointIndex = pointIdToIndex(*it);
        for(innerIt = newPoints[pointIndex].neighborPoints.begin(); innerIt != newPoints[pointIndex].neighborPoints.end(); innerIt++)
        {
            if((*innerIt) == longPoint.getId()){
                continue;
            }
            findIter = std::find(shortPoint.neighborPoints.begin(), shortPoint.neighborPoints.end(), (*innerIt));
            if(findIter != shortPoint.neighborPoints.end()){ // van ilyen
                //Add quad
                //addTriangle(edgeId1, edges[edgeIndex].pointId1, edges[edgeIndex].pointId2, *findIter);
                addQuad(edgeId, longPoint.getId(), shortPoint.getId(), *it, *findIter);
                success = true;
                //TODO: feltölteni gpura
            }
        }

    }

    if(success){

    }

    return success;
    //return false;
}

ExtrudeEdge Topology::extrudeEdge(unsigned int edgeId, glm::mat4 modelviewMatrix, glm::mat4 projectionMatrix, Camera camera)
{
    TopologyEdge edge = edges[edgeIdToIndex(edgeId)];
    int pointId3 = 0;

    //if triangle
    if(!edge.neighborTriangles.empty()){
        TopologyHelperTriangle triangle = triangles[triangleIdToIndex(edge.neighborTriangles.back())];
        //unsigned int pointId3 = 0;
        if(triangle.pointId1 == edge.pointId1 && triangle.pointId2 == edge.pointId2 ||
            triangle.pointId1 == edge.pointId2 && triangle.pointId2 == edge.pointId1){
            pointId3 = triangle.pointId3;
        }else if(triangle.pointId3 == edge.pointId1 && triangle.pointId2 == edge.pointId2 ||
                 triangle.pointId3 == edge.pointId2 && triangle.pointId2 == edge.pointId1){
            pointId3 = triangle.pointId1;
        }else{
            pointId3 =triangle.pointId2;
        }

    }else if(!edge.neighborQuads.empty())
    {
        TopologyHelperQuad quad = quads[quadIdToIndex(edge.neighborQuads.back())];
        if(quad.pointId1 == edge.pointId1 && quad.pointId2 == edge.pointId2 ||
            quad.pointId1 == edge.pointId2 && quad.pointId2 == edge.pointId1){
            pointId3 = quad.pointId3;
        }else if(quad.pointId3 == edge.pointId1 && quad.pointId2 == edge.pointId2 ||
                 quad.pointId3 == edge.pointId2 && quad.pointId2 == edge.pointId1){
            pointId3 = quad.pointId1;
        }else if(quad.pointId4 == edge.pointId1 && quad.pointId2 == edge.pointId2 ||
                 quad.pointId4 == edge.pointId2 && quad.pointId2 == edge.pointId1){
            pointId3 = quad.pointId1;
        }else{
            pointId3 =quad.pointId2;
        }
    }

    if(pointId3 == 0){

        //nem franko valami
    }



    TopologyPoint point1 = newPoints[pointIdToIndex(edge.pointId1)];
    TopologyPoint point2 = newPoints[pointIdToIndex(edge.pointId2)];
    TopologyPoint point3 = newPoints[pointIdToIndex(pointId3)];

    glm::vec3 pos1 = point1.getPosition();
    glm::vec3 pos2 = point2.getPosition();
    glm::vec3 pos3 = point3.getPosition();

    glm::vec4 cameraPosW1 = projectionMatrix * (modelviewMatrix * glm::vec4(pos1,1.0));
    glm::vec3 cameraPos1 = glm::vec3(cameraPosW1 * (1.0f / cameraPosW1.w));
    cameraPos1.z = 0;
    glm::vec4 cameraPosW2 = projectionMatrix * (modelviewMatrix * glm::vec4(pos2,1.0));
    glm::vec3 cameraPos2 = glm::vec3(cameraPosW2 * (1.0f / cameraPosW2.w));
    cameraPos2.z = 0;
    glm::vec4 cameraPosW3 = projectionMatrix * (modelviewMatrix * glm::vec4(pos3,1.0));
    glm::vec3 cameraPos3 = glm::vec3(cameraPosW3 * (1.0f / cameraPosW3.w));
    cameraPos3.z = 0;

    glm::vec3 innerDir = cameraPos3-cameraPos1;

    glm::vec3 orto = cameraPos2 - cameraPos1;
    glm::vec3 dir = glm::normalize(glm::cross(glm::vec3(0.0,0.0,1.0), orto));

    if(glm::dot(dir,innerDir) > 0){
        dir = dir * (-1.0f);
    }

    //glm::vec3 newCameraPos1 = cameraPos1 + (dir * 0.05f);
    //glm::vec3 newCameraPos2 = cameraPos2 + (dir * 0.05f);

    /*
    glm::vec3 origiPos1 = glm::vec3(projectionMatrix * (modelviewMatrix * glm::vec4(cameraPos1, 1)));


    glm::vec3 origotr(0,0,0);
    glm::vec4 newOriW = projectionMatrix * (modelviewMatrix * glm::vec4(origotr, 1));
    glm::vec3 newOri = glm::vec3(newOriW * (1.0f/newOriW.w));
*/


    ExtrudeEdge tmpEdge;
    tmpEdge.edgeId = edge.getId();
    tmpEdge.extrudeDir = glm::vec2(dir);
    tmpEdge.cameraPos1 = glm::vec2(cameraPos1);
    tmpEdge.cameraPos2 = glm::vec2(cameraPos2);
    tmpEdge.pointId1 = edge.pointId1;
    tmpEdge.pointId2 = edge.pointId2;

    /*
    glm::vec4 retValue;
    retValue.x = newCameraPos1.x;
    retValue.y = newCameraPos1.y;
    retValue.z = newCameraPos2.x;
    retValue.w = newCameraPos2.y;
*/
    return tmpEdge;
}

glm::vec3 Topology::getSideEdgeDirection(unsigned int edgeId)
{
    int edgeIndex = edgeIdToIndex(edgeId);
    TopologyEdge edge = edges[edgeIndex];
    unsigned int pointId3 = 0;

    //if triangle
    if(!edge.neighborTriangles.empty()){
        TopologyHelperTriangle triangle = triangles[triangleIdToIndex(edge.neighborTriangles.back())];
        //unsigned int pointId3 = 0;
        if(triangle.pointId1 == edge.pointId1 && triangle.pointId2 == edge.pointId2 ||
            triangle.pointId1 == edge.pointId2 && triangle.pointId2 == edge.pointId1){
            pointId3 = triangle.pointId3;
        }else if(triangle.pointId3 == edge.pointId1 && triangle.pointId2 == edge.pointId2 ||
                 triangle.pointId3 == edge.pointId2 && triangle.pointId2 == edge.pointId1){
            pointId3 = triangle.pointId1;
        }else{
            pointId3 =triangle.pointId2;
        }

    }else if(!edge.neighborQuads.empty())
    {
        TopologyHelperQuad quad = quads[quadIdToIndex(edge.neighborQuads.back())];
        if(quad.pointId1 == edge.pointId1 && quad.pointId2 == edge.pointId2 ||
            quad.pointId1 == edge.pointId2 && quad.pointId2 == edge.pointId1){
            pointId3 = quad.pointId3;
        }else if(quad.pointId3 == edge.pointId1 && quad.pointId2 == edge.pointId2 ||
                 quad.pointId3 == edge.pointId2 && quad.pointId2 == edge.pointId1){
            pointId3 = quad.pointId1;
        }else{
            pointId3 =quad.pointId2;
        }
    }

    if(pointId3 == 0){

        //nem franko valami
    }

    int innerPointIndex1 = pointIdToIndex(edge.pointId1);
    int innerPointIndex2 = pointIdToIndex(pointId3);

    glm::vec3 innerDir = glm::vec3(newPointsPos[innerPointIndex1*3] - newPointsPos[innerPointIndex2*3],
                                    newPointsPos[innerPointIndex1*3+1] - newPointsPos[innerPointIndex2*3+1],
                                    newPointsPos[innerPointIndex1*3+2] - newPointsPos[innerPointIndex2*3+2]);



    int pointIndex1 = pointIdToIndex(edge.pointId1);
    int pointIndex2 = pointIdToIndex(edge.pointId2);

    //float normX = newEdgeNormal[edgeIndex*3];
    glm::vec3 normal = glm::vec3(newEdgeNormal[edgeIndex*6], newEdgeNormal[edgeIndex*6+1], newEdgeNormal[edgeIndex*6+2]);

    glm::vec3 orto = glm::vec3(newPointsPos[pointIndex1*3] - newPointsPos[pointIndex2*3],
                                newPointsPos[pointIndex1*3+1] - newPointsPos[pointIndex2*3+1],
                                newPointsPos[pointIndex1*3+2] - newPointsPos[pointIndex2*3+2]);

    glm::vec3 dir = glm::normalize(glm::cross(normal, orto));

    if(glm::dot(dir,innerDir) > 0){
        return dir * (-1.0f);
    }else{
        return dir;
    }

    //if quad


}




void Topology::addTQuad()
{

}
void Topology::removeTQuad()
{

}
void Topology::addDQuad()
{

}
void Topology::removeDQuad()
{


}

void Topology::addActivePoint(unsigned int pointId)
{
    if(!activeEdges.empty()){
        clearActiveEdge();
    }

    //activeEdgeId = edgeId;

    activePoints.insert(pointId);
    newPointsActive[pointIdToIndex(pointId)] = 1.0;

}

void Topology::removeActivePoint(unsigned int pointId)
{
    activePoints.erase(pointId);
    newPointsActive[pointIdToIndex(pointId)] = 0.0;
}

void Topology::clearActivePoint()
{
    //int pointIx;
    for(auto pointId : activePoints){
        newPointsActive[pointIdToIndex(pointId)] = 0.0;
    }
    activePoints.clear();
}

bool Topology::isPointActive(unsigned int pointId)
{
    return activePoints.find(pointId) != activePoints.end();
}

std::set<unsigned int> Topology::getActivePoints()
{
    return activePoints;
}

unsigned int Topology::getRandomActivePoint()
{
    if(activePoints.empty())
        return 0;
    return (*activePoints.begin());
}

void Topology::addActiveEdge(unsigned int edgeId)
{
    //if(activePointId!=0){
    //    newPointsActive[pointIdToIndex(activePointId)] = 0.0;
    //    activePointId = 0;
    //}
    if(!activePoints.empty()){
        clearActivePoint();
    }


    //activeEdgeId = edgeId;

    activeEdges.insert(edgeId);
    int edgeIx = edgeIdToIndex(edgeId);

    newEdgeActive[edgeIx*2] = 1.0;
    newEdgeActive[edgeIx*2+1] = 1.0;
}

void Topology::removeActiveEdge(unsigned int edgeId)
{
    activeEdges.erase(edgeId);
    int edgeIx = edgeIdToIndex(edgeId);

    newEdgeActive[edgeIx*2] = 0.0;
    newEdgeActive[edgeIx*2+1] = 0.0;
}

void Topology::clearActiveEdge()
{
    int edgeIx;
    for(std::set<unsigned int>::iterator it = activeEdges.begin(); it != activeEdges.end(); it++){
        edgeIx = edgeIdToIndex(*it);
        newEdgeActive[edgeIx*2] = 0.0;
        newEdgeActive[edgeIx*2+1] = 0.0;
    }
    activeEdges.clear();
}

bool Topology::isEdgeActive(unsigned int edgeId)
{
    return activeEdges.find(edgeId) != activeEdges.end();
}

std::set<unsigned int> Topology::getActiveEdges()
{
    return activeEdges;
}

std::list<unsigned int> Topology::getBorderEdgeLine(unsigned int edgeId)
{

    unsigned int nextEdgeId = edgeId;
    unsigned int nextPointId = edges[edgeIdToIndex(edgeId)].pointId1;
    unsigned int* data;
    std::list<unsigned int> edgeLine;


    while(nextEdgeId != 0){
        edgeLine.push_back(nextEdgeId);
        data = nextValidBorderEdge(edgeId,nextEdgeId, nextPointId);
        nextEdgeId = data[0];
        nextPointId = data[1];

        if(nextEdgeId == edgeId)
            break;

    }

    return edgeLine;
}

unsigned int* Topology::nextValidBorderEdge(unsigned int firstEdge, unsigned int previousEdge, unsigned int previousPoint)
{
    int prevEdgeIx = edgeIdToIndex(previousEdge);
    unsigned int nextPointId;
    unsigned int nextEdgeId = 0;
    unsigned int nextEdgeIx = 0;



    unsigned int ret[2];



    int prevPointIx = pointIdToIndex(previousPoint);



    for(std::list<unsigned int>::iterator it = newPoints[prevPointIx].neighborEdges.begin(); it!= newPoints[prevPointIx].neighborEdges.end(); it++){
        if(previousEdge != (*it) && isEdgeBorder(*it)){
            nextEdgeId = *it;
        }
    }
    if(nextEdgeId == 0)
        return ret;
    nextEdgeIx = edgeIdToIndex(nextEdgeId);
    if(previousPoint == edges[nextEdgeIx].pointId1)
        nextPointId = edges[nextEdgeIx].pointId2;
    else
        nextPointId = edges[nextEdgeIx].pointId1;


    ret[0] = nextEdgeId;
    ret[1] = nextPointId;
    return ret;

}

void Topology::addBorderEdge(unsigned int edgeId)
{
    borderEdges.insert(edgeId);
    int edgeIx = edgeIdToIndex(edgeId);

    newEdgeBorder[edgeIx*2] = 1.0;
    newEdgeBorder[edgeIx*2+1] = 1.0;
}

void Topology::removeBorderEdge(unsigned int edgeId)
{
    borderEdges.erase(edgeId);
    int edgeIx = edgeIdToIndex(edgeId);

    newEdgeBorder[edgeIx*2] = 0.0;
    newEdgeBorder[edgeIx*2+1] = 0.0;
}

bool Topology::isEdgeBorder(unsigned int edgeId)
{
    if(newEdgeBorder[edgeIdToIndex(edgeId)*2] > 0.5)
        return true;
    return false;
}

void Topology::setBorderEdgesActive()
{
    clearActiveEdge();
    activeEdges = borderEdges;

    int edgeIx;
    for(std::set<unsigned int>::iterator it = activeEdges.begin(); it != activeEdges.end(); it++){
        edgeIx = edgeIdToIndex(*it);
        newEdgeActive[edgeIx*2] = 1.0;
        newEdgeActive[edgeIx*2+1] = 1.0;
    }

}




/*******************************
 *
 * GPU Management
 *
 *
 ********************************/


void Topology::bindPointBuffers(GLuint vaoHandle){
    if(newPointsPos.empty() && pointVertexBuffer!=0){
            clearPointBuffers();
            return;
    }else if(newPointsPos.empty()){
        return;
    }

    glBindVertexArray(vaoHandle);

    GLuint vertexSize = 3 * sizeof(GLfloat);
    if(pointVertexBuffer==0){
        glGenBuffers(1, &pointVertexBuffer);
    }
    glBindBuffer(GL_ARRAY_BUFFER, pointVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, newPointsPos.size() * sizeof(float), &newPointsPos[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); //pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

    if(pointNormalBuffer == 0)
        glGenBuffers(1, &pointNormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, pointNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, newPointsNormal.size() * sizeof(float), &newPointsNormal[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1); //normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

    if(pointIdBuffer == 0)
        glGenBuffers(1, &pointIdBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, pointIdBuffer);
    glBufferData(GL_ARRAY_BUFFER, newPointsId.size() * sizeof(float), &newPointsId[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2); //id
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT), 0);

    if(pointActiveBuffer == 0)
        glGenBuffers(1, &pointActiveBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, pointActiveBuffer);
    glBufferData(GL_ARRAY_BUFFER, newPointsActive.size() * sizeof(float), &newPointsActive[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(3); //active
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT), 0);

    glBindVertexArray(0);
}

void Topology::bindEdgeBuffers(GLuint vaoHandle){
    if(newEdgePos.empty() && edgeVertexBuffer!=0){
            clearEdgeBuffers();
            return;
    }else if(newEdgePos.empty()){
        return;
    }

    glBindVertexArray(vaoHandle);
    GLuint vertexSize = 3 * sizeof(GLfloat);
    if(edgeVertexBuffer==0)
        glGenBuffers(1, &edgeVertexBuffer);
    else if(newEdgePos.empty()){
        clearEdgeBuffers();
        glBindVertexArray(0);
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, edgeVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, newEdgePos.size() * sizeof(float), &newEdgePos[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); //pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

    if(edgeNormalBuffer == 0)
        glGenBuffers(1, &edgeNormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, edgeNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, newEdgeNormal.size() * sizeof(float), &newEdgeNormal[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1); //normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

    if(edgeIdBuffer == 0)
        glGenBuffers(1, &edgeIdBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, edgeIdBuffer);
    glBufferData(GL_ARRAY_BUFFER, newEdgeId.size() * sizeof(float), &newEdgeId[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2); //id
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT), 0);

    if(edgeActiveBuffer == 0)
        glGenBuffers(1, &edgeActiveBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, edgeActiveBuffer);
    glBufferData(GL_ARRAY_BUFFER, newEdgeActive.size() * sizeof(float), &newEdgeActive[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(3); //active
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT), 0);

    if(edgeBorderBuffer == 0)
        glGenBuffers(1, &edgeBorderBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, edgeBorderBuffer);
    glBufferData(GL_ARRAY_BUFFER, newEdgeBorder.size() * sizeof(float), &newEdgeBorder[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(4); //Border
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT), 0);

    glBindVertexArray(0);
}

void Topology::bindTriangleBuffers(GLuint vaoHandle)
{
    if(trianglePos.empty() && triangleVertexBuffer!=0){
            clearTriangleBuffers();
            return;
    }else if(trianglePos.empty()){
        return;
    }
    glBindVertexArray(vaoHandle);
    GLuint vertexSize = 3 * sizeof(GLfloat);
    if(triangleVertexBuffer==0)
        glGenBuffers(1, &triangleVertexBuffer);
    else if(trianglePos.empty()){
        clearTriangleBuffers();
        glBindVertexArray(0);
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, triangleVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, trianglePos.size() * sizeof(float), &trianglePos[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); //pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

    if(triangleNormalBuffer == 0)
        glGenBuffers(1, &triangleNormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, triangleNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, triangleNormal.size() * sizeof(float), &triangleNormal[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1); //normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

    if(triangleIdBuffer == 0)
        glGenBuffers(1, &triangleIdBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, triangleIdBuffer);
    glBufferData(GL_ARRAY_BUFFER, triangleId.size() * sizeof(float), &triangleId[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2); //id
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT), 0);

    /*
    if(triangleIndicesBuffer==0)
        glGenBuffers(1, &triangleIndicesBuffer);
    if(!triangleIndices.empty()){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndicesBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleIndices.size() * sizeof(unsigned int), &triangleIndices[0], GL_STATIC_DRAW);
    }*/

    glBindVertexArray(0);
}


void Topology::bindQuadBuffers(GLuint vaoHandle){
    if(newPointsPos.empty() && pointVertexBuffer!=0){
        clearQuadBuffers();
        return;
    }else if(newPointsPos.empty()){
        return;
    }



    glBindVertexArray(vaoHandle);
    GLuint vertexSize = 3 * sizeof(GLfloat);
    if(pointVertexBuffer==0)
        glGenBuffers(1, &pointVertexBuffer);
    else if(newPointsPos.empty()){
        clearQuadBuffers();
        glBindVertexArray(0);
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, pointVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, newPointsPos.size() * sizeof(float), &newPointsPos[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); //pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

    if(pointNormalBuffer == 0)
        glGenBuffers(1, &pointNormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, pointNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, newPointsNormal.size() * sizeof(float), &newPointsNormal[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1); //normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

    if(quadIndicesBuffer==0)
        glGenBuffers(1, &quadIndicesBuffer);
    if(!quadIndices.empty()){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIndicesBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadIndices.size() * sizeof(unsigned int), &quadIndices[0], GL_STATIC_DRAW);
    }else{
        clearQuadBuffers();
        glBindVertexArray(0);
        return;
    }

    glBindVertexArray(0);
}



void Topology::clearPointBuffers()
{
    glDeleteBuffers(1,&pointVertexBuffer);
    pointVertexBuffer=0;
    glDeleteBuffers(1,&pointNormalBuffer);
    pointNormalBuffer=0;
    glDeleteBuffers(1,&pointIdBuffer);
    pointIdBuffer=0;
    glDeleteBuffers(1,&pointActiveBuffer);
    pointActiveBuffer=0;
}

void Topology::clearEdgeBuffers(){

    glDeleteBuffers(1,&edgeVertexBuffer);
    edgeVertexBuffer=0;
    glDeleteBuffers(1,&edgeNormalBuffer);
    edgeNormalBuffer=0;
    glDeleteBuffers(1,&edgeIdBuffer);
    edgeIdBuffer=0;
    glDeleteBuffers(1,&edgeActiveBuffer);
    edgeActiveBuffer=0;
    glDeleteBuffers(1,&edgeBorderBuffer);
    edgeBorderBuffer=0;
}

void Topology::clearTriangleBuffers(){
    glDeleteBuffers(1,&triangleVertexBuffer);
    triangleVertexBuffer=0;
    glDeleteBuffers(1,&triangleNormalBuffer);
    triangleNormalBuffer=0;
    glDeleteBuffers(1,&triangleIdBuffer);
    triangleIdBuffer=0;
}

void Topology::clearQuadBuffers(){

    glDeleteBuffers(1,&quadIndicesBuffer);
    quadIndicesBuffer=0;
}




void Topology::resetTopology()
{
    pointId_index.clear();
    edgeId_index.clear();

    activePointId=0;
    clearActivePoint();
    newPoints.clear();
    newPointsPos.clear();
    newPointsNormal.clear();
    newPointsId.clear();
    newPointsActive.clear();

   // activeEdgeId=0;
    clearActiveEdge();
    edges.clear();
    newEdgePos.clear();
    newEdgeNormal.clear();
    newEdgeId.clear();
    newEdgeActive.clear();
    newEdgeBorder.clear();

    triangles.clear();
    triangleId_index.clear();
    quads.clear();
    quadId_index.clear();
    quadIndices.clear();

    clearPointBuffers();
    clearEdgeBuffers();
    clearTriangleBuffers();
    clearQuadBuffers();

}
