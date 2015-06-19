//
//  WorldObject.h
//  ProjetoPG
//
//  Created by Igor Lira on 5/16/15.
//  Copyright (c) 2015 Igor Lira. All rights reserved.
//

#ifndef __ProjetoPG__WorldObject__
#define __ProjetoPG__WorldObject__

#include "Header.h"

#define MAX_VERTICES 102400
#define MAX_FACES 102400

class WorldObject {
    vector3 vertices[MAX_VERTICES];
    int vertexCount;
    
    vector3 vertexNormals[MAX_VERTICES];
    vector3 transformedVertexNormals[MAX_VERTICES];
    vector3 transformedVertices[MAX_VERTICES];
    bool vertexHasNormal[MAX_VERTICES];
    
    bool transformedNormalsAreValid;
    
    modelFace faces[MAX_FACES];
    int faceCount;
    
    vector3 color;
    
    vector3 position;
    vector3 rotation;
    float scale;
    
    class World* world;
    
public:
    WorldObject();
    void SetModel(const char* modelFileName);
    void Draw(class Camera* camera);
    void SetPosition(float x, float y, float z);
    void Translate(float x, float y, float z);
    void Rotate(float x, float y, float z);
    void AddScale(float x);
    
    void SetColor(vector3 color);
    vector3 GetColor();
    
    vector3 GetPosition();
    
    void SetWorld(class World* world);
    
private:
    void AddVertex(float x, float y, float z);
    void AddFace(int v1, int v2, int v3);
    vector3 GetTransformedVertex(int v);
    vector3 GetVertexNormal(int v);
    vector3 ApplyTransformationsToVector(vector3 v);
    
    vector3 CalculateAmbientComponent();
    vector3 CalculateDiffuseComponent(vector3 lightIntensity, vector3 normal, vector3 lightPosition);
    vector3 CalculateSpecularComponent(vector3 lightIntensity, vector3 normal, vector3 lightPosition, vector3 observerPosition);
};

#endif /* defined(__ProjetoPG__WorldObject__) */
