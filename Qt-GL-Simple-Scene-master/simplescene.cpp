#include "simplescene.h"
#include "modelimporter.h"
#include <iostream>
#include <Assimp/include/assimp/Importer.hpp>




/*
    TODO
    * élek megkülönböztetése ID-val !!fontos ++pipa
    * élek törlése ++pipa
    * él behuzása két meglévő pont közé ++pipa
    * pontok törlése ++pipa
    *
    * quadok megcsinálása (csak megjelenítés) --felejtős
    *
    * közelítés, távolítás ++pipa;
    * workitem arréb mozgatása ++ valami;
    *
    * háromszögesítés(vagy saját formátum)->exportálás !!fontos
*/


SimpleScene::SimpleScene()
{
}

void SimpleScene::initScene()
{
    qActive = false;
    tActive = false;
    snappingPointId = 0;
    snapping = false;
    drawing = false;
    glEnable(GL_DEPTH_TEST);
    rotAngleX = 0;
    rotAngleY = 0;
    rotX = glm::vec3(1,0,0);
    rotY = glm::vec3(0,1,0);
    camera.pos = glm::vec3(0.0,0.0,-5.0);
    //cameraPos = glm::vec3(0.0,0.0,-10.0);
    camera.up = glm::vec3(0.0, 1.0, 0.0);
    //cameraUp = glm::vec3(0.0, 1.0, 0.0);
    camera.dir = glm::vec3(0.0,0.0,1.0);
    //cameraDir = glm::vec3(0.0,0.0,1.0);
    centerPos = glm::vec3(0.0,0.0,0.0);
    //modelview = glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,-10.0));
    modelview = glm::lookAt(camera.pos, centerPos, camera.up);
    normalmatrix = glm::mat3(modelview._inverse());
    rotationMatrix = glm::mat4(1.0);
    lightPosition = glm::vec3(0.0, 1.0, 19.0);

    altshader = new Shader("D:\\msconlab\\Qt-GL-Simple-Scene-master\\vertex.glsl",
                           "D:\\msconlab\\Qt-GL-Simple-Scene-master\\fragment.glsl");
    newpoint_shader = new Shader("D:\\msconlab\\Qt-GL-Simple-Scene-master\\newpoint_vertex.glsl",
                                 "D:\\msconlab\\Qt-GL-Simple-Scene-master\\newpoint_fragment.glsl");

    topology_shader = new Shader("D:\\msconlab\\Qt-GL-Simple-Scene-master\\color_vertex.glsl",
                                 "D:\\msconlab\\Qt-GL-Simple-Scene-master\\color_fragment.glsl",
                                 "D:\\msconlab\\Qt-GL-Simple-Scene-master\\color_geometry.glsl");

    pointid_shader = new Shader("D:\\msconlab\\Qt-GL-Simple-Scene-master\\pointid_vertex.glsl",
                                "D:\\msconlab\\Qt-GL-Simple-Scene-master\\pointid_fragment.glsl");
    edgeid_shader = new Shader("D:\\msconlab\\Qt-GL-Simple-Scene-master\\edgeid_vertex.glsl",
                                "D:\\msconlab\\Qt-GL-Simple-Scene-master\\edgeid_fragment.glsl");
    pointnormal_shader = new Shader("D:\\msconlab\\Qt-GL-Simple-Scene-master\\pointnormal_vertex.glsl",
                                "D:\\msconlab\\Qt-GL-Simple-Scene-master\\pointnormal_fragment.glsl");
    edge_shader = new Shader("D:\\msconlab\\Qt-GL-Simple-Scene-master\\edge_vertex.glsl",
                                "D:\\msconlab\\Qt-GL-Simple-Scene-master\\edge_fragment.glsl");
    quad_shader = new Shader("D:\\msconlab\\Qt-GL-Simple-Scene-master\\quad_vertex.glsl",
                                "D:\\msconlab\\Qt-GL-Simple-Scene-master\\quad_fragment.glsl");
    triangle_shader = new Shader("D:\\msconlab\\Qt-GL-Simple-Scene-master\\triangle_vertex.glsl",
                                "D:\\msconlab\\Qt-GL-Simple-Scene-master\\triangle_fragment.glsl");
    triangleid_shader = new Shader("D:\\msconlab\\Qt-GL-Simple-Scene-master\\triangleid_vertex.glsl",
                                "D:\\msconlab\\Qt-GL-Simple-Scene-master\\triangleid_fragment.glsl");

    cylinderProjection_shader = new Shader("D:\\msconlab\\Qt-GL-Simple-Scene-master\\cylinderproj_vertex.glsl",
                                "D:\\msconlab\\Qt-GL-Simple-Scene-master\\cylinderproj_fragment.glsl",
                                "D:\\msconlab\\Qt-GL-Simple-Scene-master\\cylinderproj_geometry.glsl");


    //width = 890;
    //height = 800;
    zoomValue = 0;
    //positionBuffer = new Framebuffer(width, height, 1, false, true);

    cylinderNormalFrameBuffer = 0;
    cylinderPositionFrameBuffer = 0;
    similarNormalFrameBuffer = 0;
    similarPositionFrameBuffer = 0;

    positionFrameBuffer = 0;
    positionBackFaceFrameBuffer = 0;
    normalFrameBuffer = 0;
    pointIdFrameBuffer = 0;
    pointIdSnapFrameBuffer = 0;
    edgeIdFrameBuffer = 0;
    triangleIdFrameBuffer = 0;

    sUpdatePositionFB = true;
    sUpdatePositionBackFaceFB = true;
    sUpdateNormalFB = true;
    sUpdatePointIdFB = true;
    sUpdatePointIdSnapFB = true;
    sUpdateEdgeIdFB = true;
    sUpdateTriangleIdFB = true;

    // Z order buffering
    //glEnable(GL_DEPTH_TEST);
   // glDepthFunc(GL_LESS);
   // glDisable(GL_CULL_FACE);


    // Vertex Array Object
/*    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    program = Program::Load( vert, NULL, frag );
    mvpUniform = glGetUniformLocation(program, "mvp");
    GLuint modelMatrixUniform = glGetUniformLocation(program, "modelMatrix");
    GLuint viewMatrixUniform = glGetUniformLocation(program, "viewMatrix");
    GLuint lightPositionUniform = glGetUniformLocation(program, "lightPosition_worldspace");

    GLuint texture = loadImage();
    GLuint textureSamplerUniform = glGetUniformLocation(program, "textureSampler");*/




    glGenVertexArrays(6, vaoHandle);

    //newPoints = new float[nVertex*3];

    selectionRectangle.init(vaoHandle[5]);


    glGenBuffers(1, &vertexBuffer);


    glGenBuffers(1, &normalBuffer);


    glGenBuffers(1, &indicesBuffer);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


/*
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &uvBuffer);
    glDeleteBuffers(1, &normalBuffer);*/
    //glDeleteTextures(1, &texture);
    //glDeleteVertexArrays(1, &vertexArrayObject);

}

void SimpleScene::loadModelFromFile(const char *fileName)
{
    GLuint vertexSize = 3 * sizeof(GLfloat);

    ModelImporter importer;
    std::vector<unsigned int> indices;
    std::vector<float> vertices;
    std::vector<float> normals;
    bool successfulImport = importer.loadOBJ(fileName, indices, vertices, normals);

    if(successfulImport){
        projectManager.setModelPath(fileName);
        topology.resetTopology();
        nVertex = indices.size();

        glBindVertexArray(vaoHandle[0]);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0); //pos
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1); //normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

}

void SimpleScene::saveProject(const char *fileName)
{
    projectManager.saveProject(fileName, topology);
}

void SimpleScene::openProject(const char *fileName)
{
    projectManager.openProject(fileName, topology);

    updatePointBuffers();
    updateEdgeBuffers();
    updateTriangleBuffers();
    updateQuadBuffers();

    printf("\n point size: %d", topology.newPoints.size());
}

void SimpleScene::update(float t)
{
    modelview = glm::rotate(modelview, t, glm::vec3(1.0,0.0,0.0) );
    normalmatrix = glm::mat3(modelview._inverse());

//    modelview = glm::rotate(modelview, t, glm::vec3(1.0, 0.0, 0.0));
//    static float angle = 0.0f;
//    angle += t;
//    shader.setUniform("twist", angle);
}

void SimpleScene::changeScene(int handle)
{
    /*activeBuffer = handle;
    glBindVertexArray(vaoHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandles[activeBuffer]);
    glBindVertexArray(0);*/
}

void SimpleScene::zoomSlide(int zoom)
{
    //glm::vec3 zoomVec(0.0,0.0,-0.03*(float)zoom);
   // glm::vec3 zoomVec = glm::vec3(modelview * glm::vec4( glm::vec3(0.0,0.0,-0.03*(float)zoom),0.0));
    //modelview = glm::translate(modelview, zoomVec);

    //projection = glm::translate(projection, zoomVec);

    camera.pos = camera.pos - camera.dir * (0.03f * (float) zoom);
    modelview = glm::lookAt(camera.pos, centerPos, camera.up);
    setAllsUpdateFB(true);
}

void SimpleScene::rotateScene(float x, float y){

    float angle = glm::length(glm::vec2(x,y))*100.0f;
    //glm::vec3 zoomVec = glm::vec3(projection*(modelview * glm::vec4( glm::vec3(x,y,0),1.0)));
    glm::vec3 rot1 = glm::vec3(projection*(modelview*vec4(1,0,0,0)));
    glm::vec3 rot2 = glm::vec3(projection*(modelview*vec4(0,1,0,0)));
    modelview = glm::rotate(modelview, x*100, rot2 );
    modelview = glm::rotate(modelview, y*100, rot1 );
    normalmatrix = glm::mat3(modelview._inverse());

    rotX = glm::normalize(glm::vec3(rotationMatrix*glm::vec4(rotX,1)));
    rotY = glm::normalize(glm::vec3(rotationMatrix*glm::vec4(rotY,1)));


    glm::vec3 yminusx = glm::normalize(rotX * x + rotY * y);
    camera.dir = glm::normalize(centerPos - camera.pos);

    glm::vec3 rotAxis = glm::normalize(glm::cross(yminusx, camera.dir));

    glm::mat4 tmpRotationMatrix = glm::mat4(1.0);
    rotationMatrix = glm::mat4(1.0);
    rotationMatrix = glm::rotate(rotationMatrix, angle, rotAxis);

    //rotationMatrix =  tmpRotationMatrix * rotationMatrix ;

    camera.pos = glm::vec3( rotationMatrix * glm::vec4(camera.pos, 1.0) );
    camera.up = glm::vec3( rotationMatrix * glm::vec4(camera.up, 1.0) );


    /*
    rotationMatrix = glm::mat4(1.0);
    rotationMatrix = glm::rotate(rotationMatrix, x*100, rot2 );
    cameraPos = glm::vec3( rotationMatrix * glm::vec4(cameraPos, 1.0) );
    cameraUp = glm::vec3( rotationMatrix * glm::vec4(cameraUp, 1.0) );

    rotationMatrix = glm::mat4(1.0);
    rotationMatrix = glm::rotate(rotationMatrix, y*100, rot1 );
    cameraPos = glm::vec3( rotationMatrix * glm::vec4(cameraPos, 1.0) );
    cameraUp = glm::vec3( rotationMatrix * glm::vec4(cameraUp, 1.0) );
*/

    /*
    float angleX = x/width * 1000.0;
    float angleY = y/height * 1000.0;

    rotAngleX = angleX;
    rotAngleY = angleY;
    float d = glm::length(cameraPos-centerPos);
    float camX = cameraPos.x;
    float camY = cameraPos.y;
    float camZ = cameraPos.z;

    cameraPos.x = camX * glm::cos(rotAngleX) - camZ * glm::sin(rotAngleX);
    cameraPos.z = camZ * glm::cos(rotAngleX) + camX * glm::sin(rotAngleX);

    camZ = cameraPos.z;

    cameraPos.y = camY * glm::cos(rotAngleY) - camZ * glm::sin(rotAngleY);
    cameraPos.z = camZ * glm::cos(rotAngleY) + camY * glm::sin(rotAngleY);

    float UpX = cameraUp.x;
    float UpY = cameraUp.y;
    float UpZ = cameraUp.z;

    cameraUp.x = UpX * glm::cos(rotAngleX) - UpZ * glm::sin(rotAngleX);
    cameraUp.z = UpZ * glm::cos(rotAngleX) + UpX * glm::sin(rotAngleX);

    UpZ = cameraUp.z;

    cameraUp.y = UpY * glm::cos(rotAngleY) - UpZ * glm::sin(rotAngleY);
    cameraUp.z = UpZ * glm::cos(rotAngleY) + UpY * glm::sin(rotAngleY);
*/


    /*
    cameraPos.x = d * glm::cos(rotAngleX) * glm::cos(rotAngleY);
    cameraPos.y = d * glm::cos(rotAngleX) * glm::sin(rotAngleY);
    cameraPos.z = d * glm::sin(rotAngleX);

    cameraUp.x = glm::cos(rotAngleX) * glm::cos(rotAngleY);
    cameraUp.y = glm::cos(rotAngleX) * glm::sin(rotAngleY);
    cameraUp.z = glm::sin(rotAngleX);

    */
    //cameraUp.x += d * glm::cos(angleX) * glm::cos(angleY);
    //cameraUp.y += d * glm::cos(angleX) * glm::sin(angleY);
    //cameraUp.z += d * glm::sin(angleX);



    modelview = glm::lookAt(camera.pos, centerPos, camera.up);
    setAllsUpdateFB(true);
    //glm::vec4 tmp = glm::vec4(lightPosition, 0);
    // lightPosition = vec3(modelview._inverse() * tmp);
    //lightPosition = lightPosition - angle*100.f*glm::vec3(x,y,0.0);
}

void SimpleScene::onClickEvent(int x, int y){
    makeNewPoint(x, y);
}

void SimpleScene::moveModel(float x, float y)
{
    /* //old
    printf("moveModel");
    glm::vec3 projVec(x,-y,0.0);
    projection = glm::translate(projection, projVec);
    */

    glm::vec3 cameraRight = glm::cross(camera.up, camera.dir);

    glm::vec3 moveUp = camera.up * y;
    glm::vec3 moveRight = cameraRight * x;

    camera.pos = camera.pos + moveUp + moveRight;
    centerPos = centerPos + moveUp + moveRight;

    modelview = glm::lookAt(camera.pos, centerPos, camera.up);
    setAllsUpdateFB(true);


}



void SimpleScene::makeNewPoint(int x, int y){
    int glY = height - y;
   printf("\n mousepos x: %d  -  y: %d \n",x, glY);
/*
    printf("\n mousepos x: %d  -  y: %d",x, glY);
    positionBuffer->setRenderTarget();
    newpoint_shader->enable();

    newpoint_shader->bindUniformMatrixMat4("projection",projection);
    newpoint_shader->bindUniformMatrixMat4("modelview", modelview);

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vaoHandle[0]);

    glDrawElements(GL_TRIANGLES, nVertex, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);


    glFlush();

    //ez gyorsabb ! sokkal
    static float* positionsk = new float[4];
    glReadPixels(x,glY,1,1, GL_RGBA, GL_FLOAT, positionsk);



    static float* positions = new float[width*height*4];
    glBindTexture(GL_TEXTURE_2D, positionBuffer->getColorBuffer(0));
    glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_FLOAT,positions);
    printf("\n new vetrex x: %f  -  y: %f  - z: %f",positionsk[0],positionsk[1], positionsk[2]);


    int koord = (width*glY + x)*4;
    printf("\n new vetrex x: %f  -  y: %f  - z: %f",positions[koord],positions[koord+1], positions[koord+2]);
    */
    float* positionsk = getPointPosition(x, glY);

    if(positionsk[3] > 0.5 && getSelectedPointId(x, glY)==0){

       unsigned int oldActiveID = topology.getRandomActivePoint();
        //bool hasActivePointBefore = !topology.activeEdges.empty();


        for(int i = 0; i < topology.newPoints.size(); i++){
            topology.newPoints[i].setActive(false);
        }
        glm::vec3 newNormal = getPointNormal(x, glY);
        TopologyPoint tmpPoint(positionsk[0],positionsk[1], positionsk[2]);
        tmpPoint.setActive(true);
        tmpPoint.setNormal(newNormal.x, newNormal.y, newNormal.z);

        topology.addPoint(tmpPoint);

        if(oldActiveID != 0){
            topology.addEdge(topology.getRandomActivePoint(), tmpPoint.getId());
            updateEdgeBuffers();
        }
        topology.setActivePoint(tmpPoint.getId());
        //bufferek bindolasa
        updatePointBuffers();

        //todo felküldeni melyik aktiv szinezés miatt
        //todo megnézni mit csinál ha unsigned int-et küldesz fel?
        // todo id-t kiíratni shader-rel ha az megvagyon beolvasni glpointsize-val növelt cuccal
        //amelik közelebb, az nyer kb korrekt.
        //kattra id kiolvas és akkor örülünk mint majom a farkának
        // be kell állítani melyik aktív
        // új pont mindjárt szomszédja aktivnak, ha van aktiv.
        // ha pontra kattintunk, akkor nem kell új pont, hanem összekötjük aktívval
        //      ha nincs aktív akkor nem csinálunk semmit
        //ctrl+click aktív kiválasztása...
        // új pont mindig aktív, vagy amelyikhez kötöttük. gg wp :)))))

        //billboard-dal javítani pontokon!

    }else{
        makeEdgeBeetweenPoints(x, glY);
    }

    printf("/n pontok szama: %d", topology.newPoints.size());
    positionBuffer->disableRenderTarget();
    //gl polygon offset megnézni


    //tesztelni hogyha intet floattá castolva felnyomunk mi jön vissza.
    newpoint_shader->disable();

}




bool SimpleScene::setActivePoint(int x, int y){
    int glY = height - y;
    unsigned int selectedID = getSelectedPointId(x, glY);
    if(selectedID == 0)
        return false;
    topology.setActivePoint(selectedID);
    updatePointBuffers();
    updateEdgeBuffers();
    return true;
}

bool SimpleScene::unsetActivePoint(int x, int y)
{
    int glY = height - y;
    unsigned int selectedID = getSelectedEdgeId(x, glY);
    if(selectedID == 0)
        return false;

    if(topology.isPointActive(selectedID)){
        topology.removeActivePoint(selectedID);
        updatePointBuffers();
    }
    return true;
}

bool SimpleScene::addActivePoint(int x, int y)
{
    int glY = height - y;
    unsigned int selectedID = getSelectedEdgeId(x, glY);
    if(selectedID == 0)
        return false;

    if(!topology.isPointActive(selectedID)){
        topology.addActivePoint(selectedID);
        updatePointBuffers();
        updateEdgeBuffers();
    }
    return true;
}

bool SimpleScene::setActiveEdge(int x, int y)
{
    int glY = height - y;
    unsigned int selectedID = getSelectedEdgeId(x, glY);
    if(selectedID == 0)
        return false;

    topology.setActiveEdge(selectedID);
    updatePointBuffers();
    updateEdgeBuffers();

    return true;
}

bool SimpleScene::unsetActiveEdge(int x, int y)
{
    int glY = height - y;
    unsigned int selectedID = getSelectedEdgeId(x, glY);
    if(selectedID == 0)
        return false;

    if(topology.isEdgeActive(selectedID)){
        topology.removeActiveEdge(selectedID);
        updateEdgeBuffers();
    }
    return true;
}

bool SimpleScene::addActiveEdge(int x, int y)
{
    int glY = height - y;
    unsigned int selectedID = getSelectedEdgeId(x, glY);
    if(selectedID == 0)
        return false;

    if(!topology.isEdgeActive(selectedID)){
        topology.addActiveEdge(selectedID);
        updatePointBuffers();
        updateEdgeBuffers();
    }
    return true;

}

void SimpleScene::makeEdgeBeetweenPoints(int x, int y)
{
    if(topology.activePoints.empty()){
        return;
    }

    unsigned int selectedId = getSelectedPointId(x, y);
    if(selectedId == 0){
        return;
    }

    unsigned int edgeId = topology.addEdge(topology.getRandomActivePoint(), selectedId);
    if(edgeId == 0){
        return;
    }
    if(topology.testTriangulation(edgeId)){
        //updateTriangleBuffers();
    }
    if(topology.testQuadration(edgeId)){
        //updateQuadBuffers();
    }
    updateTriangleBuffers();
    updateQuadBuffers();
    updateEdgeBuffers();

}

void SimpleScene::removeActivePoint()
{
    //old
    /*
    if(topology.activePointId==0)
        return;
    topology.removePoint(topology.activePointId);
    */

    if(topology.activePoints.empty())
        return;

    std::list<unsigned int> shouldRemove;
    for(auto pointId : topology.activePoints){
        shouldRemove.push_back(pointId);
    }

    for(std::list<unsigned int>::iterator it = shouldRemove.begin(); it != shouldRemove.end(); it++){
        topology.removePoint(*it);
    }

    updatePointBuffers();
    updateEdgeBuffers();
    updateQuadBuffers();
    updateTriangleBuffers();
}

void SimpleScene::removeActiveEdge()
{
    if(topology.activeEdges.empty())
        return;

    std::list<unsigned int> shouldRemove;
    for(std::set<unsigned int>::iterator it = topology.activeEdges.begin(); it != topology.activeEdges.end(); it++){
        shouldRemove.push_back(*it);
    }

    for(std::list<unsigned int>::iterator it = shouldRemove.begin(); it != shouldRemove.end(); it++){
        topology.removeEdge(*it);
    }

    updateEdgeBuffers();
    updateQuadBuffers();
    updateTriangleBuffers();
}

void SimpleScene::divideTriangle(int x, int y)
{
    float glY = height - y;
    unsigned int triangleId = getSelectedTriangleId(x,glY);
    if(triangleId == 0){
        return;
    }
    //topology.activePointId = 0;




    float* newPos = getPointPosition(x, glY);
    //glm::vec3 newNormal = getPointNormal(x,y);
    if(newPos[3] < 0.5)
        return;

    //pont felvétele
    for(int i = 0; i < topology.newPoints.size(); i++){
        topology.newPoints[i].setActive(false);
    }
    glm::vec3 newNormal = getPointNormal(x, glY);
    TopologyPoint tmpPoint(newPos[0],newPos[1], newPos[2]);
    tmpPoint.setActive(true);
    tmpPoint.setNormal(newNormal.x, newNormal.y, newNormal.z);

    topology.addPoint(tmpPoint);

    topology.setActivePoint(tmpPoint.getId());



    int triangleIndex = topology.triangleIdToIndex(triangleId);
    TopologyHelperTriangle oldTriangle = topology.triangles[triangleIndex];

    topology.addEdge(tmpPoint.getId(), oldTriangle.pointId1);
    topology.addEdge(tmpPoint.getId(), oldTriangle.pointId2);
    topology.addEdge(tmpPoint.getId(), oldTriangle.pointId3);


    topology.addTriangle(oldTriangle.edgeId1, oldTriangle.pointId1, oldTriangle.pointId2,tmpPoint.getId());
    topology.addTriangle(oldTriangle.edgeId2, oldTriangle.pointId2, oldTriangle.pointId3,tmpPoint.getId());
    topology.addTriangle(oldTriangle.edgeId3, oldTriangle.pointId3, oldTriangle.pointId1,tmpPoint.getId());

    topology.removeTriangle(triangleId, 0);

    //bufferek bindolasa
    updatePointBuffers();
    updateEdgeBuffers();
    updateTriangleBuffers();

}



void SimpleScene::divideEdge(int x, int y)
{
    int glY = height - y;
    unsigned int edgeId = getSelectedEdgeId(x,glY);
    if(edgeId == 0){
        return;
    }
    //topology.activePointId = 0;




    float* newPos = getPointPosition(x, glY);
    //glm::vec3 newNormal = getPointNormal(x,y);
    if(newPos[3] < 0.5)
        return;

    //pont felvétele
    for(int i = 0; i < topology.newPoints.size(); i++){
        topology.newPoints[i].setActive(false);
    }
    glm::vec3 newNormal = getPointNormal(x, glY);
    TopologyPoint tmpPoint(newPos[0],newPos[1], newPos[2]);
    tmpPoint.setActive(true);
    tmpPoint.setNormal(newNormal.x, newNormal.y, newNormal.z);

    topology.addPoint(tmpPoint);

    topology.setActivePoint(tmpPoint.getId());



    int edgeIndex = topology.edgeIdToIndex(edgeId);
    TopologyEdge oldEdge = topology.edges[edgeIndex];

    topology.removeEdge(edgeId);

    unsigned int edgeId1 = topology.addEdge(oldEdge.pointId1, tmpPoint.getId());
    unsigned int edgeId2 = topology.addEdge(tmpPoint.getId(), oldEdge.pointId2);



    /*
    if(topology.testTriangulation(edgeId1)){
        updateTriangleBuffers();
    }
    if(topology.testTriangulation(edgeId2)){
        updateTriangleBuffers();
    }*/

    if(topology.testQuadration(edgeId1)){
        updateQuadBuffers();
    }
    updateQuadBuffers();


    //bufferek bindolasa
    updatePointBuffers();
    updateEdgeBuffers();
    updateTriangleBuffers();
}

void SimpleScene::moveActivePoint(int x, int y)
{
    int glY = height - y;
    if(topology.activePoints.empty())
        return;
    float* newPos = getPointPosition(x, glY);
    glm::vec3 newNormal = getPointNormal(x,y);

    if(newPos[3] < 0.5)
        return;
    topology.movePoint(topology.getRandomActivePoint(), newPos[0], newPos[1], newPos[2], newNormal.x, newNormal.y, newNormal.z);

    updatePointBuffers();
    updateEdgeBuffers();
    updateTriangleBuffers();

}

bool SimpleScene::extrudeEdge(int x, int y)
{

    int glY = height - y;
    long edgeId = getSelectedEdgeId(x, glY);
    if (edgeId == 0)
        return false;
    if(!topology.isEdgeBorder(edgeId))
        return false;

    //glm::vec4 newXY = topology.extrudeEdge(edgeId,modelview, projection, camera);

    std::list<unsigned int> edgeIds;
    edgeIds.push_back(edgeId);

    initExtrude(edgeIds, x, glY);

/*
    int x1 = (newXY.x + 1.0f)/2.0f * width;
    int y1 = (newXY.y + 1.0f)/2.0f * height;
    int x2 = (newXY.z + 1.0f)/2.0f * width;
    int y2 = (newXY.w + 1.0f)/2.0f * height;

    //pont felvétele

    glm::vec3 newNormal = getPointNormal(x1, y1);
    float* newPos = getPointPosition(x1, y1);
    TopologyPoint tmpPoint(newPos[0],newPos[1], newPos[2]);
   // tmpPoint.setActive(true);
    tmpPoint.setNormal(newNormal.x, newNormal.y, newNormal.z);

    topology.addPoint(tmpPoint);

    //topology.setActivePoint(tmpPoint.getId());
*/

    updatePointBuffers();
    updateEdgeBuffers();

    //updateTriangleBuffers();
    updateQuadBuffers();

    /*
    glm::vec3 dir = topology.getSideEdgeDirection(edgeId);
    glm::vec3 cameraViewDir =glm::normalize(glm::cross(camera.dir, glm::cross(dir, camera.dir)));

    TopologyEdge edge = topology.edges[topology.edgeIdToIndex(edgeId)];
    edge.pointId1;


    int a = 2;
    int b = a+4;


    int translatedX = x + cameraViewDir.x * 10;
    int translatedY = glY + cameraViewDir.y * 10;

    float* newPos = getPointPosition(translatedX, translatedY);
    //glm::vec3 newNormal = getPointNormal(x,y);
    if(newPos[3] < 0.5)
        return;

    //pont felvétele

    glm::vec3 newNormal = getPointNormal(translatedX, translatedY);
    TopologyPoint tmpPoint(newPos[0],newPos[1], newPos[2]);
   // tmpPoint.setActive(true);
    tmpPoint.setNormal(newNormal.x, newNormal.y, newNormal.z);

    topology.addPoint(tmpPoint);

    //topology.setActivePoint(tmpPoint.getId());

    updatePointBuffers();
*/
    return true;
}

void SimpleScene::tmpExtrudeFirstTriangle(int x, int y)
{
    int glY = height - y;
    std::list<unsigned int> edgeIds;
//    edgeIds.push_back(1);
//    edgeIds.push_back(2);
//    edgeIds.push_back(3);

    for(std::set<unsigned int>::iterator it = topology.activeEdges.begin(); it != topology.activeEdges.end(); it++){
        edgeIds.push_back(*it);
    }

    initExtrude(edgeIds, x, glY);


    //moveExtrudablePoint(0,0);


    updatePointBuffers();
    updateEdgeBuffers();
    updateQuadBuffers();
}

void SimpleScene::initExtrude(std::list<unsigned int> edgeIds, int initX, int initY)
{
     extrudeInitX = initX;
     extrudeInitY = initY;
     std::list<ExtrudeEdge> edges;
     for(std::list<unsigned int>::iterator it = edgeIds.begin(); it != edgeIds.end(); it++){
        edges.push_back(topology.extrudeEdge((*it),modelview, projection, camera));
     }

     for(std::list<ExtrudeEdge>::iterator it = edges.begin(); it != edges.end(); it++){
        ExtrudeEdge edge = (*it);
        unsigned int newPointId1 = pointHax(edge.pointId1, edge.extrudeDir, edge.cameraPos1);
        unsigned int newPointId2 = pointHax(edge.pointId2, edge.extrudeDir, edge.cameraPos2);
        topology.addEdge(newPointId1, newPointId2);
        topology.addQuad(edge.edgeId, edge.pointId1, edge.pointId2, newPointId1, newPointId2);
     }
}

glm::vec2 SimpleScene::vectorHelper(glm::vec2 inpVec){
    glm::vec2 retVec;
    retVec.x = (inpVec.x + 1.0f)/2.0f * width;
    retVec.y = (inpVec.y + 1.0f)/2.0f * height;
    return retVec;
}

unsigned int SimpleScene::pointHax(unsigned int pointId, glm::vec2 dir, glm::vec2 cameraPos){
    glm::vec2 screenPos = vectorHelper(cameraPos);
    unsigned int retId;

    if(extrudePointMap.find(pointId) == extrudePointMap.end()){
        //not found
        //add point
        glm::vec3 newNormal = getPointNormal(screenPos.x, screenPos.y);
        float* newPos = getPointPosition(screenPos.x, screenPos.y);
        TopologyPoint tmpPoint(newPos[0],newPos[1], newPos[2]);
        //TopologyPoint tmpPoint(newPos[0]*1.5f,newPos[1]*1.5f, newPos[2]);
        tmpPoint.setNormal(newNormal.x, newNormal.y, newNormal.z);
        retId = topology.addPoint(tmpPoint);
        //adding done

        ExtrudePoint extrudeP;
        extrudeP.cameraPos = cameraPos;
        extrudeP.extrudeDir = dir;
        extrudeP.pointId = retId;

        topology.addEdge(pointId, retId);

        extrudePointMap[pointId] = extrudeP;

    }else{
        //found
        extrudePointMap[pointId].extrudeDir += dir;
        extrudePointMap[pointId].extrudeDir = glm::normalize(extrudePointMap[pointId].extrudeDir);
        retId = extrudePointMap[pointId].pointId;


    }

    return retId;
}

void SimpleScene::stopExtrude()
{
    extrudePointMap.clear();
}

void SimpleScene::moveExtrudablePoint(int x, int y)
{
    int glY = height - y;
    float length = glm::length(glm::vec2(x,glY) - glm::vec2(extrudeInitX, extrudeInitY));
    //float length = 20;
    float param = 0.005f;


    for(std::map<unsigned int, ExtrudePoint>::iterator it = extrudePointMap.begin(); it != extrudePointMap.end(); it++){
        ExtrudePoint p = it->second;

        glm::vec2 newCameraPos = p.cameraPos + p.extrudeDir * (length * param);
        glm::vec2 newScreenCoord = vectorHelper(newCameraPos);

        glm::vec3 newNormal = getPointNormal((int)newScreenCoord.x, (int)newScreenCoord.y);

        float* newPos = getPointPosition((int)newScreenCoord.x, (int)newScreenCoord.y);

        //TopologyPoint pp = topology.newPoints[topology.pointIdToIndex(p.pointId)];
        //glm::vec3 newPos = pp.getPosition();
        //glm::vec3 newNormal = pp.getNormal();

        //topology.movePoint(p.pointId, newPos.x *1.5f, newPos.y*1.5f, newPos.z, newNormal.x, newNormal.y, newNormal.z);


        if(newPos[3] > 0.5){
            topology.movePoint(p.pointId, newPos[0], newPos[1], newPos[2], newNormal.x, newNormal.y, newNormal.z);
        }
    }

    updatePointBuffers();
    updateEdgeBuffers();
    updateTriangleBuffers();


}

void SimpleScene::selectBorderEdges()
{

    topology.setBorderEdgesActive();
    updatePointBuffers();
    updateEdgeBuffers();
}

void SimpleScene::selectBorderEdgeLine(int x, int y)
{
    int glY = height - y;
    unsigned int edgeId = getSelectedEdgeId(x,glY);
    if(edgeId == 0)
        return;

    topology.clearActiveEdge();
    std::list<unsigned int> edges = topology.getBorderEdgeLine(edgeId);

    for(std::list<unsigned int>::iterator i = edges.begin(); i != edges.end(); i++){
        topology.addActiveEdge(*i);
    }

    updatePointBuffers();
    updateEdgeBuffers();

}

void SimpleScene::clearEdgeSelection()
{
    topology.clearActiveEdge();
    updateEdgeBuffers();
}




bool SimpleScene::getSnapping() const
{
    return snapping;
}

void SimpleScene::setSnapping(bool value)
{
    snapping = value;
    if(!snapping){
        snappingPointId = 0;
    }
}

void SimpleScene::indicateSnappingPoint(int x, int y)
{
    int glY = height - y;

    snappingPointId = getSelectedPointId(x, glY);

}


void SimpleScene::projectToSimilarModel()
{
    std::set<unsigned int> pointIds = getSelectedPointsIds();
    for(auto pointId : pointIds){
        TopologyPoint projectingPoint = topology.newPoints[topology.pointIdToIndex(pointId)];
        glm::mat4 viewMatrix = similarProj.getViewMatrix(projectingPoint);
        glm::mat4 projectionMatrix = similarProj.getProjectionMatrix();

        glm::vec3 newPosition = getPointPositionForSimilarProjection(width/2, height/2, viewMatrix, projectionMatrix);
        glm::vec3 newNormal = getPointNormalForSimilarProjection(width/2, height/2, viewMatrix, projectionMatrix);

        topology.movePoint(pointId, newPosition.x, newPosition.y, newPosition.z, newNormal.x, newNormal.y, newNormal.z);
    }

    updatePointBuffers();
    updateEdgeBuffers();
    updateTriangleBuffers();
}



void SimpleScene::projectTMPCylinder()
{
    std::list<glm::mat4> matrices = cylinderProj.getProjectionMatrices();

    std::vector<glm::vec3> pointPos;
    std::vector<glm::vec3> pointNorm;

    for(auto matrix : matrices){
       pointPos.push_back(getPointPositionWithMatrix(width/2, height/2, matrix));
       pointNorm.push_back(getPointNormalWithMatrix(width/2, height/2, matrix));
    }
    cylinderProj.projectCylinderToModel(pointPos, pointNorm, topology);

    updatePointBuffers();
    updateEdgeBuffers();
    updateQuadBuffers();


}

void SimpleScene::projectTMPCylinderViaShader()
{
    std::vector<glm::vec3> pointNorm = getPointDataViaCylinderShader(false);
    std::vector<glm::vec3> pointPos = getPointDataViaCylinderShader(true);

    cylinderProj.projectCylinderToModel(pointPos, pointNorm, topology);

    //vec3 pointProjectionToLine = cylinderBasePoint + dot(baseToPoint,baseToTop) / dot(baseToTop, baseToTop) * baseToTop;

    glm::vec3 pointA = glm::vec3(1,0,0);
    glm::vec3 pointB = glm::vec3(2,0,0);
    glm::vec3 pointC = glm::vec3(1.5, 1, 0);
    glm::vec3 pointD = glm::vec3(0,1,0);

    glm::vec3 pointAC = pointC - pointA;
    glm::vec3 pointAD = pointD - pointA;
    glm::vec3 pointAB = pointB - pointA;

    glm::vec3 projectC = pointA + glm::dot(pointAC, pointAB) / glm::dot(pointAB, pointAB) * pointAB;

    glm::vec3 projectD = pointA + glm::dot(pointAD, pointAB) / glm::dot(pointAB, pointAB) * pointAB;


    updatePointBuffers();
    updateEdgeBuffers();
    updateQuadBuffers();

}

void SimpleScene::startDrawingOnSurface(int x, int y)
{
    if(!drawing){
        drawedPoints.clear();
        drawedBackFacePoints.clear();
    }
    drawing = true;
}

void SimpleScene::stopDrawingOnSurface(int x, int y)
{
    drawing = false;
}

void SimpleScene::drawingOnSurface(int x, int y)
{
    int glY = height - y;
    float* pos = getPointPosition(x, glY);

    drawedPoints.push_back(glm::vec3(pos[0], pos[1], pos[2]));
    drawedBackFacePoints.push_back(getBackFacePointPosition(x, glY));
}

void SimpleScene::linearRegression()
{
    std::vector<glm::vec3> line = getLineStartAndEndPoint(drawedPoints);
    std::vector<glm::vec3> lineBackFace = getLineStartAndEndPoint(drawedBackFacePoints);


    glm::vec3 startPoint = (float)(1.0/2.0) * (line[0] + lineBackFace[0]);
    glm::vec3 endPoint =  (float)(1.0/2.0) * (line[1] + lineBackFace[1]);

}

void SimpleScene::startRectangleSelection(int x, int y)
{
    int glY = height - y;
    selectionRectangle.setStartPosition(x, glY);
}

void SimpleScene::stopRectangleSelection(int x, int y)
{
    selectionRectangle.setShouldDraw(false);
    std::set<unsigned int> selectedIds = getSelectedPointsIds();
    topology.clearActivePoint();
    for(auto pointId : selectedIds){
        topology.addActivePoint(pointId);
    }
    updatePointBuffers();
    updateEdgeBuffers();
}

void SimpleScene::rectangleSelection(int x, int y)
{
    int glY = height - y;
    selectionRectangle.setEndPosition(x, glY);
    selectionRectangle.setShouldDraw(true);
}

std::vector<glm::vec3> SimpleScene::getLineStartAndEndPoint(std::list<glm::vec3> points)
{
    float* line = LinearRegressor::pointsToLineRegression(points);

    glm::vec3 pointA = glm::vec3(line[0], line[1], line[2]);
    glm::vec3 pointB = glm::vec3(line[3], line[4], line[5]);
    glm::vec3 pointC = points.back();
    glm::vec3 pointD = points.front();

    glm::vec3 pointAC = pointC - pointA;
    glm::vec3 pointAD = pointD - pointA;
    glm::vec3 pointAB = pointB - pointA;

    glm::vec3 projectC = pointA + glm::dot(pointAC, pointAB) / glm::dot(pointAB, pointAB) * pointAB;

    glm::vec3 projectD = pointA + glm::dot(pointAD, pointAB) / glm::dot(pointAB, pointAB) * pointAB;

    std::vector<glm::vec3> retValue;
    retValue.push_back(projectC);
    retValue.push_back(projectD);

    return retValue;
}


unsigned int SimpleScene::getSelectedPointId(int x, int y){
    if(snapping){
        updatePointIdSnapFB(false);
        pointIdSnapFrameBuffer->setRenderTarget();
    }else{
        updatePointIdFB(false);
        pointIdFrameBuffer->setRenderTarget();
    }

    //ez gyorsabb ! sokkal
    static float* positionsk = new float[4];
    glReadPixels(x,y,1,1, GL_RGBA, GL_FLOAT, positionsk);

    unsigned int dd = (unsigned int)positionsk[2];
    if(positionsk[2] > dd+ 0.5)
        dd++;
    printf("\n selected vetrex ID f: %f  -  d: %d  ", positionsk[2], dd);//static_cast<unsigned int>(positionsk[2]));


    if(snapping){
        pointIdSnapFrameBuffer->disableRenderTarget();
    }else{
        pointIdFrameBuffer->disableRenderTarget();
    }

    return dd;
}

std::set<unsigned int> SimpleScene::getSelectedPointsIds()
{
    updatePointIdFB(false);
    pointIdFrameBuffer->setRenderTarget();

    int startX;
    int endX;
    int startY;
    int endY;

    if( selectionRectangle.startX < selectionRectangle.endX ){
        startX = selectionRectangle.startX;
        endX = selectionRectangle.endX;
    }else{
        startX = selectionRectangle.endX;
        endX = selectionRectangle.startX;
    }

    if( selectionRectangle.startY < selectionRectangle.endY ){
        startY = selectionRectangle.startY;
        endY = selectionRectangle.endY;
    }else{
        startY = selectionRectangle.endY;
        endY = selectionRectangle.startY;
    }

    int distX = endX-startX;
    int distY = endY-startY;


    //ez gyorsabb ! sokkal
    static float* positionsk = new float[distX*distY*4];


    glReadPixels(startX, startY, distX, distY, GL_RGBA, GL_FLOAT, positionsk);

    /*
    unsigned int dd = (unsigned int)positionsk[2];
    if(positionsk[2] > dd+ 0.5)
        dd++;
    */

    unsigned int dd;
    std::set<unsigned int> returnValue;
    for(int i = 2; i < distX*distY*4; i+=4){
        dd = (unsigned int)positionsk[i];
        if(positionsk[i] > dd+ 0.5)
                dd++;
        if(dd != 0)
            returnValue.insert(dd);

    }

    pointIdFrameBuffer->disableRenderTarget();


    return returnValue;
}

unsigned int SimpleScene::getSelectedEdgeId(int x, int y)
{
    updateEdgeIdFB(false);
    edgeIdFrameBuffer->setRenderTarget();

    static float* data = new float[4];
    glReadPixels(x,y,1,1, GL_RGBA, GL_FLOAT, data);

    unsigned int dd = (unsigned int)data[2];
    if(data[2] > dd+ 0.5)
        dd++;
    printf("\n selected line ID f: %f  -  d: %d  ", data[2], dd);//static_cast<unsigned int>(positionsk[2]));


    edgeIdFrameBuffer->disableRenderTarget();
    return dd;

}

unsigned int SimpleScene::getSelectedTriangleId(int x, int y)
{
    updateTriangleIdFB(false);
    triangleIdFrameBuffer->setRenderTarget();

    //ez gyorsabb ! sokkal
    static float* data = new float[4];
    glReadPixels(x,y,1,1, GL_RGBA, GL_FLOAT, data);

    unsigned int dd = (unsigned int)data[2];
    if(data[2] > dd+ 0.5)
        dd++;
    printf("\n selected triangle ID f: %f  -  d: %d  ", data[2], dd);//static_cast<unsigned int>(positionsk[2]));


    triangleIdFrameBuffer->disableRenderTarget();
    return dd;
}

glm::vec3 SimpleScene::getPointNormal(int x, int y)
{
    updateNormalFB(false);
    normalFrameBuffer->setRenderTarget();
    /*pointnormal_shader->enable();
    pointnormal_shader->bindUniformMatrixMat4("projection",projection);
    pointnormal_shader->bindUniformMatrixMat4("modelview", modelview);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vaoHandle[0]);

    glDrawElements(GL_TRIANGLES, nVertex, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);


    glFlush();*/

    //ez gyorsabb ! sokkal
    static float* positionsk = new float[4];
    glReadPixels(x,y,1,1, GL_RGBA, GL_FLOAT, positionsk);

    //printf("\n selected vetrex ID f: %f  -  d: %d  ", positionsk[2], dd);//static_cast<unsigned int>(positionsk[2]));


    //pointnormal_shader->disable();

    normalFrameBuffer->disableRenderTarget();
    return glm::vec3(positionsk[0], positionsk[1], positionsk[2]);
}

float *SimpleScene::getPointPosition(int x, int y)
{
    updatePositionFB(false);

    positionFrameBuffer->setRenderTarget();
    /*newpoint_shader->enable();

    newpoint_shader->bindUniformMatrixMat4("projection",projection);
    newpoint_shader->bindUniformMatrixMat4("modelview", modelview);

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vaoHandle[0]);

    glDrawElements(GL_TRIANGLES, nVertex, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);


    glFlush();*/

    //ez gyorsabb ! sokkal
    static float* positionsk = new float[4];
    glReadPixels(x,y,1,1, GL_RGBA, GL_FLOAT, positionsk);

    //newpoint_shader->disable();
    positionFrameBuffer->disableRenderTarget();

    return positionsk;
}

glm::vec3 SimpleScene::getBackFacePointPosition(int x, int y)
{
    updatePositionBackFaceFB(false);

    positionBackFaceFrameBuffer->setRenderTarget();

    static float* positionsk = new float[4];
    glReadPixels(x,y,1,1, GL_RGBA, GL_FLOAT, positionsk);

    positionBackFaceFrameBuffer->disableRenderTarget();

    return glm::vec3(positionsk[0], positionsk[1], positionsk[2]);
}

glm::vec3 SimpleScene::getPointPositionForSimilarProjection(int x, int y, glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix)
{
    similarPositionFrameBuffer->setRenderTarget();

    newpoint_shader->enable();
    newpoint_shader->bindUniformMatrixMat4("projection",projectionMatrix);//projection);
    newpoint_shader->bindUniformMatrixMat4("modelview", viewMatrix);

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vaoHandle[0]);

    glDrawElements(GL_TRIANGLES, nVertex, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);


    glFlush();


    static float* positionsk = new float[4];
    glReadPixels(x,y,1,1, GL_RGBA, GL_FLOAT, positionsk);


    newpoint_shader->disable();
    similarPositionFrameBuffer->disableRenderTarget();

    return glm::vec3(positionsk[0], positionsk[1], positionsk[2]);
}

glm::vec3 SimpleScene::getPointNormalForSimilarProjection(int x, int y, glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix)
{
    similarNormalFrameBuffer->setRenderTarget();
    pointnormal_shader->enable();
    pointnormal_shader->bindUniformMatrixMat4("projection", projectionMatrix);//projection);
    pointnormal_shader->bindUniformMatrixMat4("modelview", viewMatrix);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vaoHandle[0]);

    glDrawElements(GL_TRIANGLES, nVertex, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);

    glFlush();


    static float* positionsk = new float[4];
    glReadPixels(x,y,1,1, GL_RGBA, GL_FLOAT, positionsk);

    pointnormal_shader->disable();
    similarNormalFrameBuffer->disableRenderTarget();
    return glm::vec3(positionsk[0], positionsk[1], positionsk[2]);
}

glm::vec3 SimpleScene::getPointPositionWithMatrix(int x, int y, glm::mat4 &modelMatrix)
{
    cylinderPositionFrameBuffer->setRenderTarget();

    newpoint_shader->enable();
    newpoint_shader->bindUniformMatrixMat4("projection",glm::mat4());//projection);
    newpoint_shader->bindUniformMatrixMat4("modelview", modelMatrix);

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vaoHandle[0]);

    glDrawElements(GL_TRIANGLES, nVertex, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);


    glFlush();


    static float* positionsk = new float[4];
    glReadPixels(x,y,1,1, GL_RGBA, GL_FLOAT, positionsk);


    newpoint_shader->disable();
    cylinderPositionFrameBuffer->disableRenderTarget();

    return glm::vec3(positionsk[0], positionsk[1], positionsk[2]);
}

glm::vec3 SimpleScene::getPointNormalWithMatrix(int x, int y, glm::mat4 &modelMatrix)
{
    cylinderNormalFrameBuffer->setRenderTarget();
    pointnormal_shader->enable();
    pointnormal_shader->bindUniformMatrixMat4("projection",glm::mat4());//projection);
    pointnormal_shader->bindUniformMatrixMat4("modelview", modelMatrix);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vaoHandle[0]);

    glDrawElements(GL_TRIANGLES, nVertex, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);

    glFlush();


    static float* positionsk = new float[4];
    glReadPixels(x,y,1,1, GL_RGBA, GL_FLOAT, positionsk);

    pointnormal_shader->disable();
    cylinderNormalFrameBuffer->disableRenderTarget();
    return glm::vec3(positionsk[0], positionsk[1], positionsk[2]);
}

std::vector<glm::vec3> SimpleScene::getPointDataViaCylinderShader(bool outputType)
{
    if(outputType)
        cylinderPositionFrameBuffer->setRenderTarget();
    else
        cylinderNormalFrameBuffer->setRenderTarget();
    cylinderProjection_shader->enable();
    cylinderProjection_shader->bindUniformFloat3("cylinderBasePoint", cylinderProj.basePointPos.x, cylinderProj.basePointPos.y, cylinderProj.basePointPos.z);
    cylinderProjection_shader->bindUniformFloat3("cylinderTopPoint", cylinderProj.topPointPos.x, cylinderProj.topPointPos.y, cylinderProj.topPointPos.z);
    cylinderProjection_shader->bindUniformFloat3("cylinderUpDir", cylinderProj.baseUpDirection.x, cylinderProj.baseUpDirection.y, cylinderProj.baseUpDirection.z);
    cylinderProjection_shader->bindUniformFloat3("cylinderRightDir", cylinderProj.baseRayDirection.x, cylinderProj.baseRayDirection.y, cylinderProj.baseRayDirection.z);

    cylinderProjection_shader->bindUniformFloat("cylinderHeight", cylinderProj.height);
    cylinderProjection_shader->bindUniformFloat("cylinderRadius", cylinderProj.radius);

    cylinderProjection_shader->bindUniformBool("outputType", outputType);

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vaoHandle[0]);

    glDrawElements(GL_TRIANGLES, nVertex, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);

    glFlush();

    //tmp
    static float* positions = new float[width*height*4];
    if(outputType)
        glBindTexture(GL_TEXTURE_2D, cylinderPositionFrameBuffer->getColorBuffer(0));
    else
        glBindTexture(GL_TEXTURE_2D, cylinderNormalFrameBuffer->getColorBuffer(0));

    glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_FLOAT,positions);
    //tmp

    std::vector<glm::vec3> returnValue;
    std::vector<glm::vec3> returnValue2;

    static float* positionsk = new float[4];
    int x = width / cylinderProj.verticalGridSize;
    int y = height / cylinderProj.horizontalGridSize;
    int index;

    //TODO megvizsgalni
    for(int j = 0; j < cylinderProj.horizontalGridSize; j++){
        for(int i = 0; i < cylinderProj.verticalGridSize; i++){
            glReadPixels(i*x,j*y,1,1, GL_RGBA, GL_FLOAT, positionsk);
            returnValue.push_back(glm::vec3(positionsk[0], positionsk[1], positionsk[2]));
            index = i*x*4 + j*y*width;
            returnValue2.push_back(glm::vec3(positions[index], positions[index+1], positions[index+2]));

        }
    }

    if(outputType)
        cylinderPositionFrameBuffer->disableRenderTarget();
    else
        cylinderNormalFrameBuffer->disableRenderTarget();

    cylinderProjection_shader->disable();

    return returnValue;
}



void SimpleScene::render()
{
    altshader->enable();
    altshader->bindUniformMatrixMat3("normalmatrix",normalmatrix);
    altshader->bindUniformMatrixMat4("projection",projection);
    altshader->bindUniformFloat3("lightPosition",lightPosition.x, lightPosition.y, lightPosition.z);
    altshader->bindUniformMatrixMat4("modelview", modelview);



    glClearColor(0.2f,0.2f,0.2f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vaoHandle[0]);
    glDrawElements(GL_TRIANGLES, nVertex, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
    altshader->disable();

    if(!topology.quadIndices.empty()){
        //pointsprite vagy geometry shader
        //glDisable(GL_CULL_FACE);
        glDepthMask(false);
        //glClear(GL_DEPTH_BUFFER_BIT);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_POLYGON_OFFSET_POINT);
        glPolygonOffset( -1.0, -10.0 );
        quad_shader->enable();
        quad_shader->bindUniformMatrixMat4("projection",projection);
        quad_shader->bindUniformMatrixMat4("modelview", modelview);
        glBindVertexArray(vaoHandle[3]);
        //glPointSize(6);
        glDrawElements(GL_QUADS, topology.quadIndices.size(), GL_UNSIGNED_INT, NULL);
        //glDrawArrays(GL_POINTS, 0, topology.newPoints.size());
        glBindVertexArray(0);

        quad_shader->disable();
        glDisable(GL_POLYGON_OFFSET_POINT);
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glDepthMask(true);
    }

    if(!topology.triangles.empty()){
        //pointsprite vagy geometry shader
        //glDisable(GL_CULL_FACE);
        glDepthMask(false);
        //glClear(GL_DEPTH_BUFFER_BIT);
        glDisable(GL_CULL_FACE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_POLYGON_OFFSET_POINT);
        glPolygonOffset( -1.0, -10.0 );
        triangle_shader->enable();
        triangle_shader->bindUniformMatrixMat4("projection",projection);
        triangle_shader->bindUniformMatrixMat4("modelview", modelview);
        glBindVertexArray(vaoHandle[4]);
        //glPointSize(6);
        //glDrawElements(GL_TRIANGLES, topology.triangleIndices.size(), GL_UNSIGNED_INT, NULL);
        glDrawArrays(GL_TRIANGLES, 0, topology.triangles.size()*3);
        //glDrawArrays(GL_POINTS, 0, topology.newPoints.size());
        glBindVertexArray(0);

        triangle_shader->disable();
        glDisable(GL_POLYGON_OFFSET_POINT);
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glDepthMask(true);
    }

    if(!topology.edges.empty()){

        //glClear(GL_DEPTH_BUFFER_BIT);
        glDepthMask(false);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glEnable(GL_POLYGON_OFFSET_POINT);

        edge_shader->enable();
        edge_shader->bindUniformMatrixMat4("projection",projection);
        edge_shader->bindUniformMatrixMat4("modelview", modelview);
        glBindVertexArray(vaoHandle[2]);
        glLineWidth(3);
        //glDrawElements(GL_LINES, topology.lineIndieces.size(), GL_UNSIGNED_INT, NULL);//&topology.lineIndieces[0]);
        glDrawArrays(GL_LINES, 0, topology.edges.size()*2);
        glBindVertexArray(0);

        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glDepthMask(true);
        edge_shader->disable();
    }

    if(!topology.newPoints.empty()){
        //pointsprite vagy geometry shader
        //glDisable(GL_CULL_FACE);
        glDepthMask(false);
        //glClear(GL_DEPTH_BUFFER_BIT);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_POLYGON_OFFSET_POINT);
        glPolygonOffset( -1.0, -10.0 );
        topology_shader->enable();
        topology_shader->bindUniformMatrixMat4("projection",projection);
        topology_shader->bindUniformMatrixMat4("modelview", modelview);
        topology_shader->bindUniformFloat("snappingId", (float)snappingPointId);
        glBindVertexArray(vaoHandle[1]);
        if(snapping){
            glPointSize(30);
        }else{
            glPointSize(6);
        }
        //glDrawElements(GL_POINTS, topology.newPoints.size(), GL_UNSIGNED_INT, NULL);
        glDrawArrays(GL_POINTS, 0, topology.newPoints.size());
        glBindVertexArray(0);

        topology_shader->disable();
        glDisable(GL_POLYGON_OFFSET_POINT);
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glDepthMask(true);
    }


    selectionRectangle.allowedDraw();

    //glDisable(GL_CULL_FACE);


}

void SimpleScene::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
    projection = glm::perspective(45.0f, static_cast<GLfloat>(w) / static_cast<GLfloat>(h), 0.1f, 100.0f);

    if(positionFrameBuffer != 0){
        deleteFrameBuffers();
    }
    createFrameBuffers(width, height);
    selectionRectangle.setSize(height, width);
}


void SimpleScene::deleteFrameBuffers()
{
    delete cylinderNormalFrameBuffer;
    delete cylinderPositionFrameBuffer;
    delete similarNormalFrameBuffer;
    delete similarPositionFrameBuffer;
    delete positionFrameBuffer;
    delete positionBackFaceFrameBuffer;
    delete normalFrameBuffer;
    delete pointIdFrameBuffer;
    delete pointIdSnapFrameBuffer;
    delete edgeIdFrameBuffer;
    delete triangleIdFrameBuffer;
}

void SimpleScene::createFrameBuffers(int width, int height)
{
    cylinderNormalFrameBuffer = new Framebuffer(width, height, 1, false, true);
    cylinderPositionFrameBuffer = new Framebuffer(width, height, 1, false, true);
    similarNormalFrameBuffer = new Framebuffer(width, height, 1, false, true);
    similarPositionFrameBuffer = new Framebuffer(width, height, 1, false, true);
    positionFrameBuffer = new Framebuffer(width, height, 1, false, true);
    positionBackFaceFrameBuffer = new Framebuffer(width, height, 1, false, true);
    normalFrameBuffer = new Framebuffer(width, height, 1, false, true);
    pointIdFrameBuffer = new Framebuffer(width, height, 1, false, true);
    pointIdSnapFrameBuffer = new Framebuffer(width, height, 1, false, true);
    edgeIdFrameBuffer = new Framebuffer(width, height, 1, false, true);
    triangleIdFrameBuffer = new Framebuffer(width, height, 1, false, true);
}

void SimpleScene::updatePositionFB(bool force)
{
    if(!sUpdatePositionFB && !force)
        return;


    positionFrameBuffer->setRenderTarget();
    newpoint_shader->enable();

    newpoint_shader->bindUniformMatrixMat4("projection",projection);
    newpoint_shader->bindUniformMatrixMat4("modelview", modelview);

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vaoHandle[0]);

    glDrawElements(GL_TRIANGLES, nVertex, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);


    glFlush();

    newpoint_shader->disable();
    positionFrameBuffer->disableRenderTarget();
    sUpdatePositionFB = false;

}

void SimpleScene::updatePositionBackFaceFB(bool force)
{
    if(!sUpdatePositionBackFaceFB && !force)
        return;


    positionBackFaceFrameBuffer->setRenderTarget();
    newpoint_shader->enable();

    newpoint_shader->bindUniformMatrixMat4("projection",projection);
    newpoint_shader->bindUniformMatrixMat4("modelview", modelview);

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vaoHandle[0]);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glDrawElements(GL_TRIANGLES, nVertex, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);

    glCullFace(GL_BACK);

    glFlush();

    newpoint_shader->disable();
    positionBackFaceFrameBuffer->disableRenderTarget();
    sUpdatePositionBackFaceFB = false;
}

void SimpleScene::updateNormalFB(bool force)
{
    if(!sUpdateNormalFB && !force)
        return;

    normalFrameBuffer->setRenderTarget();
    pointnormal_shader->enable();
    pointnormal_shader->bindUniformMatrixMat4("projection",projection);
    pointnormal_shader->bindUniformMatrixMat4("modelview", modelview);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vaoHandle[0]);

    glDrawElements(GL_TRIANGLES, nVertex, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);


    glFlush();

    pointnormal_shader->disable();

    normalFrameBuffer->disableRenderTarget();
    sUpdateNormalFB = false;
}

void SimpleScene::updatePointIdFB(bool force)
{
    if(!sUpdatePointIdFB && !force)
        return;

    pointIdFrameBuffer->setRenderTarget();
    pointid_shader->enable();
    pointid_shader->bindUniformMatrixMat4("projection",projection);
    pointid_shader->bindUniformMatrixMat4("modelview", modelview);

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vaoHandle[1]);
    //if(snapping){
    //    glPointSize(30);
    //}else{
        glPointSize(6);
    //}
    glDrawArrays(GL_POINTS, 0, topology.newPoints.size());

    glBindVertexArray(0);


    glFlush();

    pointid_shader->disable();

    pointIdFrameBuffer->disableRenderTarget();
    sUpdatePointIdFB = false;
}

void SimpleScene::updatePointIdSnapFB(bool force)
{
    if(!sUpdatePointIdSnapFB && !force)
        return;

    pointIdSnapFrameBuffer->setRenderTarget();
    pointid_shader->enable();
    pointid_shader->bindUniformMatrixMat4("projection",projection);
    pointid_shader->bindUniformMatrixMat4("modelview", modelview);

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vaoHandle[1]);

    //size 30
    glPointSize(30);

    glDrawArrays(GL_POINTS, 0, topology.newPoints.size());

    glBindVertexArray(0);


    glFlush();

    pointid_shader->disable();

    pointIdSnapFrameBuffer->disableRenderTarget();
    sUpdatePointIdSnapFB = false;

}

void SimpleScene::updateEdgeIdFB(bool force)
{
    if(!sUpdateEdgeIdFB && !force)
        return;

    edgeIdFrameBuffer->setRenderTarget();
    edgeid_shader->enable();
    edgeid_shader->bindUniformMatrixMat4("projection",projection);
    edgeid_shader->bindUniformMatrixMat4("modelview", modelview);

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vaoHandle[2]);
    glLineWidth(3);
    glDrawArrays(GL_LINES, 0, topology.edges.size()*2);

    glBindVertexArray(0);

    glFlush();


    edgeid_shader->disable();

    edgeIdFrameBuffer->disableRenderTarget();

    sUpdateEdgeIdFB = false;

}

void SimpleScene::updateTriangleIdFB(bool force)
{
    if(!sUpdateTriangleIdFB && !force)
        return;


    triangleIdFrameBuffer->setRenderTarget();
    triangleid_shader->enable();
    triangleid_shader->bindUniformMatrixMat4("projection",projection);
    triangleid_shader->bindUniformMatrixMat4("modelview", modelview);

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vaoHandle[4]);
    //glLineWidth(3);
    glDrawArrays(GL_TRIANGLES, 0, topology.triangles.size()*3);

    glBindVertexArray(0);


    glFlush();

    triangleid_shader->disable();

    triangleIdFrameBuffer->disableRenderTarget();
    sUpdateTriangleIdFB = false;

}

void SimpleScene::setAllsUpdateFB(bool update)
{
    sUpdatePositionFB = update;
    sUpdatePositionBackFaceFB = update;
    sUpdateNormalFB = update;
    sUpdatePointIdFB = update;
    sUpdatePointIdSnapFB = update;
    sUpdateEdgeIdFB = update;
    sUpdateTriangleIdFB = update;
}

void SimpleScene::updatePointBuffers()
{
    topology.bindPointBuffers(vaoHandle[1]);
    sUpdatePointIdFB = true;
    sUpdatePointIdSnapFB = true;
}

void SimpleScene::updateEdgeBuffers()
{
    topology.bindEdgeBuffers(vaoHandle[2]);
    sUpdateEdgeIdFB = true;
}

void SimpleScene::updateTriangleBuffers()
{
    topology.bindTriangleBuffers(vaoHandle[4]);
    sUpdateTriangleIdFB = true;
}

void SimpleScene::updateQuadBuffers()
{
    topology.bindQuadBuffers(vaoHandle[3]);
}


//haxx
void SimpleScene::addTQuad()
{
    topology.addTQuad();
    updateQuadBuffers();
}
void SimpleScene::removeTQuad()
{
    topology.removeTQuad();
    updateQuadBuffers();
}
void SimpleScene::addDQuad()
{
    topology.addDQuad();
    updateQuadBuffers();
}
void SimpleScene::removeDQuad()
{
    topology.removeDQuad();
    updateQuadBuffers();
}
