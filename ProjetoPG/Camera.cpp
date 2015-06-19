//
//  Camera.cpp
//  ProjetoPG
//
//  Created by Igor Lira on 6/17/15.
//  Copyright (c) 2015 Igor Lira. All rights reserved.
//

#include "Camera.h"
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <math.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(World* world)
{
    this->world = world;
    this->SetPosition(0, 0, 0);
    this->SetRotation(0, 0);
    this->fov = 45;
    this->near = 1.f;
    this->far = 100.f;
    this->shouldDraw = false;
}

void Camera::SetPosition(float x, float y, float z)
{
    this->position = { x, y, z };
}

void Camera::SetRotation(float x, float y)
{
    this->rotation = { x, y, 0 };
}

void Camera::Translate(float x, float y, float z)
{
    this->position.x += x;
    this->position.y += y;
    this->position.z += z;
}

float Camera::GetFar()
{
    return this->far;
}

void Camera::SetFar(float far)
{
    this->far = far;
}

void Camera::Draw(Camera *camera)
{
    glMatrixMode(GL_MODELVIEW);
    camera->LoadMatrix();
    glTranslatef(this->position.x, this->position.y, this->position.z);
    
    glRotatef(-this->rotation.y, 0, 1.0f, 0);
    glRotatef(-this->rotation.x, 1.f, 0, 0);
    
    //glutSolidTeapot(1);
    glutSolidCone(.3, 1, 10, 10);
    glTranslatef(0, 0, 1);
    glutSolidCube(1);
    glTranslatef(0, 0, -1);
    
    glPointSize(1);
    glBegin(GL_LINES);
    
    float far = -this->far;
    float near = -this->near;
    
    // Desenhar plano near
    float Hnear = 2 * tanf(toRadians(this->fov / 2)) * near;
    float Wnear = Hnear * this->aspect;
    
    float nleft = (Wnear / 2);
    float nright = -(Wnear / 2);
    float ntop = (Hnear / 2);
    float nbottom = -(Hnear / 2);
    
    glColor3f(1, 1, 1);
    glVertex3f(nleft, ntop, near);
    glVertex3f(nright, ntop, near);
    
    glVertex3f(nleft, nbottom, near);
    glVertex3f(nright, nbottom, near);
    
    glVertex3f(nleft, ntop, near);
    glVertex3f(nleft, nbottom, near);
    
    glVertex3f(nright, ntop, near);
    glVertex3f(nright, nbottom, near);
    
    // ----------
    // Desenhar plano far
    
    float Hfar = 2 * tanf(toRadians(this->fov / 2)) * far;
    float Wfar = Hfar * this->aspect;
    
    float fleft = (Wfar / 2);
    float fright = -(Wfar / 2);
    float ftop = (Hfar / 2);
    float fbottom = -(Hfar / 2);
    
    glVertex3f(fleft, ftop, far);
    glVertex3f(fright, ftop, far);

    glVertex3f(fleft, fbottom, far);
    glVertex3f(fright, fbottom, far);
    
    glVertex3f(fleft, ftop, far);
    glVertex3f(fleft, fbottom, far);
    
    glVertex3f(fright, ftop, far);
    glVertex3f(fright, fbottom, far);
    
    // ------------
    // Desenhar linhas entre os planos far e near
    
    glVertex3f(nleft, ntop, near);
    glVertex3f(fleft, ftop, far);

    glVertex3f(nright, ntop, near);
    glVertex3f(fright, ftop, far);
    
    glVertex3f(nright, nbottom, near);
    glVertex3f(fright, fbottom, far);

    glVertex3f(nleft, nbottom, near);
    glVertex3f(fleft, fbottom, far);
    
    glEnd();
}

void Camera::MoveForward(float distance)
{
    vector3 observerPosition = this->GetPosition();
    vector3 observerRotation = this->GetRotation();
    
    float pitchFactor = cosf(toRadians(observerRotation.x));
    float camMovementXComponent = ( distance * sinf(toRadians(observerRotation.y)) ) * pitchFactor;
    
    float camMovementYComponent = distance * sinf(toRadians(observerRotation.x)) * -1.0f;
    
    float yawFactor = cosf(toRadians(observerRotation.x));
    float camMovementZComponent = ( distance * cosf(toRadians(observerRotation.y)) * -1.0f ) * yawFactor;
    
    observerPosition.x += camMovementXComponent;
    observerPosition.y += camMovementYComponent;
    observerPosition.z += camMovementZComponent;
    
    this->SetPosition(observerPosition);
}

void Camera::MoveBackward(float distance)
{
    this->MoveForward(-distance);
}

void Camera::MoveLeft(float distance)
{
    float yRotRad = toRadians(this->rotation.y);
    
    float camMovementXComponent = -distance * cosf(yRotRad);
    float camMovementZComponent = -distance * sinf(yRotRad);
    
    this->Translate(camMovementXComponent, 0, camMovementZComponent);
}

void Camera::MoveRight(float distance)
{
    this->MoveLeft(-distance);
}

void Camera::DollyZoom(float deltaFOV, float focusDistance)
{
    float newFOV = this->fov + deltaFOV;
    if(newFOV < 10)
        newFOV = 10;
    else if(newFOV > 90)
        newFOV = 90;
    
    float width = focusDistance * 2.f * tanf(toRadians(fov / 2.f));
    float newDistance = (width) / (2.f * tanf(toRadians(newFOV / 2.f)));
    
    float deltaDistance = newDistance - focusDistance;
    
    this->SetFOV(newFOV);
    this->MoveBackward(deltaDistance);
    /*this->Translate(
                      deltaDistance * sinf(toRadians(observerRotation.y)),
                      deltaDistance * sinf(toRadians(observerRotation.x)),
                      deltaDistance * cosf(toRadians(observerRotation.y))
                      );*/
}

void Camera::Rotate(float x, float y)
{
    this->rotation.x += x;
    this->rotation.y += y;
}

void Camera::SetAspect(float aspect)
{
    this->aspect = aspect;
}

void Camera::SetFOV(float fov)
{
    this->fov = fov;
}

void Camera::UpdateProjectionMatrix()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(this->fov, this->aspect, this->near, this->far);
    
    //glMatrixMode(GL_MODELVIEW);
}

float Camera::GetNear()
{
    return this->near;
}

void Camera::SetNear(float near)
{
    this->near = near;
}

float Camera::GetFOV()
{
    return this->fov;
}

vector3 Camera::GetRotation()
{
    return this->rotation;
}

vector3 Camera::GetPosition()
{
    return this->position;
}

void Camera::SetPosition(vector3 position)
{
    this->position = position;
}

void Camera::LoadMatrix()
{
    // rotação X
    glLoadMatrixf(new GLfloat[16] {
        1, 0, 0, 0,
        0, cosf(this->rotation.x * M_PI / 180.f), sinf(this->rotation.x * M_PI / 180.f), 0,
        0, -sinf(this->rotation.x * M_PI / 180.f), cosf(this->rotation.x * M_PI / 180.f), 0,
        0, 0, 0, 1
    });
    // rotação y
    glMultMatrixf(new GLfloat[16] {
        cosf(this->rotation.y * M_PI / 180.f), 0, -sinf(this->rotation.y * M_PI / 180.f), 0,
        0, 1, 0, 0,
        sinf(this->rotation.y * M_PI / 180.f), 0, cosf(this->rotation.y * M_PI / 180.f), 0,
        0, 0, 0, 1
    });
    
    // translação
    glMultMatrixf(new GLfloat[16] {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -this->position.x, -this->position.y, -this->position.z, 1
    });
}