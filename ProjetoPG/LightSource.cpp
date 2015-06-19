//
//  LightSource.cpp
//  ProjetoPG
//
//  Created by Igor Lira on 6/17/15.
//  Copyright (c) 2015 Igor Lira. All rights reserved.
//

#include "LightSource.h"
#include "WorldObject.h"
#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <math.h>
#include "Header.h"

LightSource::LightSource(vector3 color, float intensity)
{
    this->color = color;
    this->intensity = intensity;
}

void LightSource::setPosition(float x, float y, float z)
{
    this->position = { x, y, z };
}

float LightSource::getIntensity()
{
    return this->intensity;
}

vector3 LightSource::getColor()
{
    return this->color;
}

vector3 LightSource::GetPosition()
{
    return this->position;
}

void LightSource::Translate(float x, float y, float z)
{
    this->position.x += x;
    this->position.y += y;
    this->position.z += z;
}

vector3 LightSource::CalculateLight(class WorldObject* object, vector3 vertex, vector3 vertexNormal, class Camera* camera)
{
    float ambientCoefficient = .05;
    float diffuseCoefficient = .45;
    float specularCoefficient = .5;
    float specularQ = 200;
    
    glm::vec3 incidenceGLM = glm::vec3(vertex.x - this->position.x, vertex.y - this->position.y, vertex.z - this->position.z);
    glm::vec3 reflectionGLM = glm::reflect(incidenceGLM, glm::vec3(vertexNormal.x, vertexNormal.y, vertexNormal.z));
    vector3 reflection = { reflectionGLM[0], reflectionGLM[1], reflectionGLM[2] };
    vector3 incidence = { incidenceGLM[0], incidenceGLM[1], incidenceGLM[2] };
    vector3 observer = { vertex.x - camera->GetPosition().x, vertex.y - camera->GetPosition().y, vertex.z - camera->GetPosition().z };
    observer = norm(observer);
    reflection = norm(reflection);
    
    // Componente ambiental
    vector3 color = object->GetColor();
    vector3 ambient = { color.x * ambientCoefficient, color.y * ambientCoefficient, color.z * ambientCoefficient };
    
    // Componente difusa
    float cos = dot(vertexNormal, this->position) / (len(vertexNormal) * len(this->position));
    
    vector3 diffuse = this->color;
    diffuse.x *= cos * this->intensity * diffuseCoefficient;
    diffuse.y *= cos * this->intensity * diffuseCoefficient;
    diffuse.z *= cos * this->intensity * diffuseCoefficient;
    
    // Componente especular
    vector3 specular = { 0, 0, 0 };
    specular.x = this->color.x * specularCoefficient * powf(dot(reflection, observer), specularQ);
    specular.y = this->color.y * specularCoefficient * powf(dot(reflection, observer), specularQ);
    specular.z = this->color.z * specularCoefficient * powf(dot(reflection, observer), specularQ);
    
    
    vector3 result;
    result.x = ambient.x + diffuse.x + specular.x;
    result.y = ambient.y + diffuse.y + specular.y;
    result.z = ambient.z + diffuse.z + specular.z;
    
    if(result.x > 1)
        result.x = 1;
    if(result.y > 1)
        result.y = 1;
    if(result.z > 1)
        result.z = 1;
    
    return result;
}