//
//  Camera.h
//  ProjetoPG
//
//  Created by Igor Lira on 6/17/15.
//  Copyright (c) 2015 Igor Lira. All rights reserved.
//

#ifndef __ProjetoPG__Camera__
#define __ProjetoPG__Camera__

#include "Header.h"

class Camera
{
    float fov;
    float aspect;
    float near;
    float far;
    
    vector3 position;
    vector3 rotation;
    class World* world;
    
public:
    bool shouldDraw;
    
    Camera(World* world);
    void SetPosition(vector3 position);
    void SetPosition(float x, float y, float z);
    void SetRotation(float x, float y);
    
    void SetFOV(float fov);
    float GetFOV();
    
    void SetAspect(float aspect);
    
    void Translate(float x, float y, float z);
    void Rotate(float x, float y);
    
    void LoadMatrix();
    vector3 GetRotation();
    vector3 GetPosition();
    
    float GetNear();
    void SetNear(float near);
    
    float GetFar();
    void SetFar(float far);
    
    void DollyZoom(float deltaFOV, float focusDistance);
    void Draw(Camera* camera);
    
    void MoveForward(float distance);
    void MoveBackward(float distance);
    void MoveLeft(float distance);
    void MoveRight(float distance);
    
public:
    void UpdateProjectionMatrix();
};

#endif /* defined(__ProjetoPG__Camera__) */
