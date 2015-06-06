//
//  WorldObject.h
//  ProjetoPG
//
//  Created by Igor Lira on 5/16/15.
//  Copyright (c) 2015 Igor Lira. All rights reserved.
//

#ifndef __ProjetoPG__WorldObject__
#define __ProjetoPG__WorldObject__

#define MAX_VERTICES 102400
#define MAX_FACES 102400

typedef struct
{
    float x;
    float y;
    float z;
} vector3;

typedef struct
{
    int v1;
    int v2;
    int v3;
} modelFace;

class WorldObject {
    vector3 vertices[MAX_VERTICES];
    int vertexCount;
    
    vector3 vertexNormals[MAX_VERTICES];
    bool vertexHasNormal[MAX_VERTICES];
    
    modelFace faces[MAX_FACES];
    int faceCount;
    
    vector3 position;
    vector3 rotation;
    float scale;
    
public:
    WorldObject();
    void SetModel(const char* modelFileName);
    void Draw();
    void SetPosition(float x, float y, float z);
    void Translate(float x, float y, float z);
    void Rotate(float x, float y, float z);
    void AddScale(float x);
    
private:
    void AddVertex(float x, float y, float z);
    void AddFace(int v1, int v2, int v3);
    vector3 GetVertexNormal(int v);
    
    vector3 CalculateAmbientComponent();
    vector3 CalculateDiffuseComponent(vector3 lightIntensity, vector3 normal, vector3 lightPosition);
    vector3 CalculateSpecularComponent(vector3 lightIntensity, vector3 normal, vector3 lightPosition, vector3 observerPosition);
};

#endif /* defined(__ProjetoPG__WorldObject__) */
