//
//  WorldObject.cpp
//  ProjetoPG
//
//  Created by Igor Lira on 5/16/15.
//  Copyright (c) 2015 Igor Lira. All rights reserved.
//

#include "WorldObject.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GLUT/GLUT.h>
#include <math.h>
#include <OpenGL/OpenGL.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "World.h"
#include "Camera.h"
#include <vector>

vector3 norm(vector3 v);

WorldObject::WorldObject()
{
    this->world = NULL;
    this->vertexCount = 0;
    this->faceCount = 0;
    this->scale = 1;
    this->transformedNormalsAreValid = false;
    this->color = { 1, 1, 1 };
    
    for(int i = 0; i < MAX_VERTICES; i++)
    {
        vertexHasNormal[i] = false;
    }
}

void WorldObject::SetColor(vector3 color)
{
    this->color = color;
}

vector3 WorldObject::GetColor()
{
    return this->color;
}

std::istream& safeGetline(std::istream& is, std::string& t)
{
    t.clear();
    
    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.
    
    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();
    
    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
            case '\n':
                return is;
            case '\r':
                if(sb->sgetc() == '\n')
                    sb->sbumpc();
                return is;
            case EOF:
                // Also handle the case when the last line has no line ending
                if(t.empty())
                    is.setstate(std::ios::eofbit);
                return is;
            default:
                t += (char)c;
        }
    }
}

void quickSort(int arr[], float weights[], int left, int right) {
    int i = left, j = right;
    int tmp;
    float ftmp;
    float pivot = weights[(left + right) / 2];
    
    /* partition */
    while (i <= j) {
        while (weights[i] < pivot)
            i++;
        while (weights[j] > pivot)
            j--;
        if (i <= j) {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            
            ftmp = weights[i];
            weights[i] = weights[j];
            weights[j] = tmp;
            
            i++;
            j--;
        }
    };
    
    /* recursion */
    if (left < j)
        quickSort(arr, weights, left, j);
    if (i < right)
        quickSort(arr, weights, i, right);
}

void compVetorNormalTriangulo ( vector3 p1, vector3 p2, vector3 p3, vector3 *n )
{
    vector3 v1, v2;
    double len;
    
    /* Encontra vetor v1 */
    v1.x = p2.x - p1.x;
    v1.y = p2.y - p1.y;
    v1.z = p2.z - p1.z;
    
    /* Encontra vetor v2 */
    v2.x = p3.x - p1.x;
    v2.y = p3.y - p1.y;
    v2.z = p3.z - p1.z;
    
    /* Calculo do produto vetorial de v1 e v2 */
    n->x = (v1.y * v2.z) - (v1.z * v2.y);
    n->y = (v1.z * v2.x) - (v1.x * v2.z);
    n->z = (v1.x * v2.y) - (v1.y * v2.x);
    
    /* normalizacao de n */
    len = sqrt(n->x*n->x + n->y*n->y + n->z*n->z);
    n->x /= len;
    n->y /= len;
    n->z /= len;
}

float len(vector3 v)
{
    return sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2));
}

void WorldObject::Draw(Camera* camera)
{
    glMatrixMode(GL_MODELVIEW);
    camera->LoadMatrix();
    glTranslatef(this->position.x, this->position.y, this->position.z);
    
    glRotatef(this->rotation.z, 0, 0, 1.0f);
    glRotatef(this->rotation.y, 0, 1.0f, 0);
    glRotatef(this->rotation.x, 1.f, 0, 0);
    glScalef(this->scale, this->scale, this->scale);
    
    
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < this->faceCount; i++)
    {
        modelFace face = this->faces[i];
        vector3 v1 = this->vertices[face.v1];
        vector3 v2 = this->vertices[face.v2];
        vector3 v3 = this->vertices[face.v3];
        
        vector3 transformedv1 = this->GetTransformedVertex(face.v1);
        vector3 transformedv2 = this->GetTransformedVertex(face.v2);
        vector3 transformedv3 = this->GetTransformedVertex(face.v3);
        
        vector3 v1norm = this->GetVertexNormal(face.v1);
        vector3 v2norm = this->GetVertexNormal(face.v2);
        vector3 v3norm = this->GetVertexNormal(face.v3);
        
        vector3 v1Color = { 0, 0, 0 };
        vector3 v2Color = { 0, 0, 0 };
        vector3 v3Color = { 0, 0, 0 };
        
        std::vector<LightSource*> lightSources = this->world->GetLightSources();
        for(int i = 0; i < lightSources.size(); i++)
        {
            LightSource* ls = lightSources[i];
            
            vector3 v1LightColor = ls->CalculateLight(this, transformedv1, v1norm, camera);
            vector3 v2LightColor = ls->CalculateLight(this, transformedv2, v2norm, camera);
            vector3 v3LightColor = ls->CalculateLight(this, transformedv3, v3norm, camera);
            
            v1Color.x += v1LightColor.x;
            v1Color.y += v1LightColor.y;
            v1Color.z += v1LightColor.z;
            
            v2Color.x += v2LightColor.x;
            v2Color.y += v2LightColor.y;
            v2Color.z += v2LightColor.z;
            
            v3Color.x += v3LightColor.x;
            v3Color.y += v3LightColor.y;
            v3Color.z += v3LightColor.z;
        }
        
        glColor3f(v1Color.x, v1Color.y, v1Color.z);
        glVertex3f(v1.x, v1.y, v1.z);
        
        glColor3f(v2Color.x, v2Color.y, v2Color.z);
        glVertex3f(v2.x, v2.y, v2.z);
        
        glColor3f(v3Color.x, v3Color.y, v3Color.z);
        glVertex3f(v3.x, v3.y, v3.z);
    }
    glEnd();
    
    this->transformedNormalsAreValid = true;
}

float dot(vector3 a, vector3 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

vector3 WorldObject::CalculateDiffuseComponent(vector3 lightIntensity, vector3 normal, vector3 lightPosition)
{
    float cos = dot(normal, lightPosition) / (len(normal) * len(lightPosition));
    
    vector3 result = lightIntensity;
    result.x *= cos * 0.5;
    result.y *= cos * 0.5;
    result.z *= cos * 0.5;
    
    return result;
}

vector3 WorldObject::GetTransformedVertex(int v)
{
    if(this->transformedNormalsAreValid)
        return this->transformedVertices[v];
    else
    {
        this->transformedVertices[v] = this->ApplyTransformationsToVector(this->vertices[v]);
        return this->transformedVertices[v];
    }
}

vector3 norm(vector3 v)
{
    float l = len(v);
    
    vector3 result;
    result.x = v.x / l;
    result.y = v.y / l;
    result.z = v.z / l;
    
    return result;
}

vector3 WorldObject::CalculateSpecularComponent(vector3 lightIntensity, vector3 normal, vector3 lightPosition, vector3 observerPosition)
{
    vector3 r;
    vector3 toEye = norm({ observerPosition.x - position.x, observerPosition.y - position.y, observerPosition.z - position.z });
    
    float factor = powf(dot(toEye, r), 10);
    
    // Ks (RV)^q
    vector3 result = lightIntensity;
    result.x *= factor * 0.3;
    result.y *= factor * 0.3;
    result.z *= factor * 0.3;
    
    return result;
}

vector3 WorldObject::CalculateAmbientComponent()
{
    vector3 result;
    result.x = 1 * .7f;
    result.y = 1 * 0.0;
    result.z = 1 * 0.0;
    
    return result;
}

vector3 WorldObject::ApplyTransformationsToVector(vector3 v)
{
    glm::vec4 vec;
    vec[0] = v.x;
    vec[1] = v.y;
    vec[2] = v.z;
    vec[3] = 1;
    
    glm::mat4x4 transform = glm::mat4x4(1.0f);
    transform = glm::scale(transform, glm::vec3(this->scale, this->scale, this->scale));
    transform = glm::rotate(transform, toRadians(this->rotation.z), glm::vec3(0, 0, 1.f));
    transform = glm::rotate(transform, toRadians(this->rotation.y), glm::vec3(0, 1.f, 0));
    transform = glm::rotate(transform, toRadians(this->rotation.x), glm::vec3(1.f, 0, 0));
    transform = glm::translate(transform, glm::vec3(this->position.x, this->position.y, this->position.z));
    
    vec = transform * vec;
    
    return { vec[0], vec[1], vec[2] };
}

vector3 WorldObject::GetPosition()
{
    return this->position;
}

vector3 WorldObject::GetVertexNormal(int v)
{
    if(vertexHasNormal[v])
    {
        if(this->transformedNormalsAreValid)
            return transformedVertexNormals[v];
        else
        {
            vector3 normal = vertexNormals[v];
            
            this->transformedVertexNormals[v] = this->ApplyTransformationsToVector(normal);
            return this->transformedVertexNormals[v];
        }
    }
    
    vector3 sum = { 0, 0, 0 };
    int count = 1;
    
    for(int i = 0; i < faceCount; i++)
    {
        modelFace face = this->faces[i];
        if(face.v1 == v || face.v2 == v || face.v3 == v)
        {
            vector3 faceNormal;
            compVetorNormalTriangulo(vertices[face.v1], vertices[face.v2], vertices[face.v3], &faceNormal);
            
            sum.x += faceNormal.x;
            sum.y += faceNormal.y;
            sum.z += faceNormal.z;
            
            //count++;
        }
    }
    
    vector3 result;
    result.x = sum.x / (float)count;
    result.y = sum.y / (float)count;
    result.z = sum.z / (float)count;
    
    vertexHasNormal[v] = true;
    vertexNormals[v] = norm(result);
    
    return norm(result);
}

void WorldObject::SetModel(const char* modelFileName)
{
    this->transformedNormalsAreValid = false;
    this->vertexCount = 0;
    this->faceCount = 0;
    
    std::ifstream f(modelFileName);

    int vn = 0;
    while(!f.eof())
    {
        std::string line;
        safeGetline(f, line);
        
        if(line.length() < 2)
            continue;
        
        char token = line[0];
        std::string restOfLine = line.substr(2);
        
        if(token == '#') // Comentario
        {
        }
        else if(token == 'v' && line[1] == 'n') // Vertice normal
        {
            restOfLine = line.substr(3);
            
            float x, y, z;
            sscanf(restOfLine.c_str(), "%f %f %f", &x, &y, &z);
            
            vector3 normal = { x, y, z };
            this->vertexNormals[vn] = normal;
            this->vertexHasNormal[vn++] = true;
        }
        else if(token == 'v') // Vertice
        {
            float x, y, z;
            sscanf(restOfLine.c_str(), "%f %f %f", &x, &y, &z);
            
            this->AddVertex(x, y, z);
        }
        else if(token == 'f') // Face
        {
            int v1, v2, v3;
            sscanf(restOfLine.c_str(), "%i %i %i", &v1, &v2, &v3);
            
            this->AddFace(v1 - 1, v2 - 1, v3 - 1);
        }
    }
}

void WorldObject::AddVertex(float x, float y, float z)
{
    this->transformedNormalsAreValid = false;
    
    this->vertices[vertexCount].x = x;
    this->vertices[vertexCount].y = y;
    this->vertices[vertexCount].z = z;
    
    this->vertexCount++;
}

void WorldObject::AddFace(int v1, int v2, int v3)
{
    this->faces[faceCount].v1 = v1;
    this->faces[faceCount].v2 = v2;
    this->faces[faceCount].v3 = v3;
    
    this->faceCount++;
}

void WorldObject::SetPosition(float x, float y, float z)
{
    this->transformedNormalsAreValid = false;
    
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
}

void WorldObject::Translate(float x, float y, float z)
{
    this->transformedNormalsAreValid = false;
    
    this->position.x += x;
    this->position.y += y;
    this->position.z += z;
}

void WorldObject::Rotate(float x, float y, float z)
{
    this->transformedNormalsAreValid = false;
    
    this->rotation.x += x;
    this->rotation.y += y;
    this->rotation.z += z;
}

void WorldObject::AddScale(float x)
{
    this->transformedNormalsAreValid = false;
    this->scale += x;
}

void WorldObject::SetWorld(World *world)
{
    this->world = world;
    world->AddObject(this);
}