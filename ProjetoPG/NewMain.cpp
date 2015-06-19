//
//  NewMain.cpp
//  ProjetoPG
//
//  Created by Igor Lira on 6/17/15.
//  Copyright (c) 2015 Igor Lira. All rights reserved.
//

#include <stdio.h>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include "World.h"
#include "Camera.h"
#include "Header.h"
#include "openGL_tutorial.h"
#include <math.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int $window_width = 800;
int $window_height = 600;

bool $mouseDown = false;
vector3 $mouseRotation = { 0, 0, 0 };
vector3 $mouseClickPosition = { 0, 0, 1 };

const char* models[2] = { "/Volumes/Igor/Users/igorlira/Desktop/yoda.obj", "/Volumes/Igor/Users/igorlira/Desktop/cubo.obj" };
int modelCount = 2;
int modelIndex = 0;

World* world;
WorldObject* $object;
Camera* camera;
Camera* directorCamera;
int selectedLightSourceIndex = -1;

void defaultViewport();
void directorViewport();

void display()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Desenhar câmera de visualização
    defaultViewport();
    glMatrixMode(GL_MODELVIEW);
    camera->LoadMatrix();
    world->Draw(camera);
    
    // Desenhar câmera do diretor
    directorViewport();
    glMatrixMode(GL_MODELVIEW);
    directorCamera->LoadMatrix();
    world->Draw(directorCamera);
    
    glFlush();
}

void defaultViewport()
{
    int width = $window_width / 2;
    int height = $window_height;
    
    glViewport(0, 0, width, height);
    camera->SetAspect((float)width / (float)height);
    camera->UpdateProjectionMatrix();
}

void directorViewport()
{
    int width = $window_width / 2;
    int height = $window_height;
    
    glViewport($window_width / 2.f, 0, width, height);
    directorCamera->SetAspect((float)width / (float)height);
    directorCamera->UpdateProjectionMatrix();
}

void reshape(int width, int height)
{
    //width /= 2;
    //glViewport(0, 0, width, height);
    
    $window_width = width;
    $window_height = height;
    
    //camera->SetAspect((float)width / (float)height);
    //camera->UpdateProjectionMatrix();
    //glMatrixMode(GL_PROJECTION);
    //gluPerspective(45, (float)$window_width / (float)$window_height, 0.1, 100);
    
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        $mouseClickPosition = { (float)x, (float)y, 1 };
        $mouseRotation = camera->GetRotation();
        $mouseDown = true;
    }
}

void passiveMotion(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
    
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
    
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
    
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
    
    ray castRay({ camera->GetPosition(), norm({ (float)posX, (float)posY, (float)posZ }) });
    
}

void motion(int x, int y)
{
    vector3 newRotation;
    newRotation.x = $mouseRotation.x + (y - $mouseClickPosition.y) * 0.1f;
    newRotation.y = $mouseRotation.y + (x - $mouseClickPosition.x) * 0.1f;
    
    camera->SetRotation(newRotation.x, newRotation.y);
    glutPostRedisplay();
}

float toRadians(float deg)
{
    return deg * M_PI / 180.f;
}

void keyboard(unsigned char key, int x, int y)
{
    float moveFactor = 0.1;
    
    if(key == ESC){
        exit(0);
    }
    else if(key == '1')
    {
        if(selectedLightSourceIndex >= 0)
        {
            LightSource* lightSource = world->GetLightSources()[selectedLightSourceIndex];
            lightSource->Translate(-moveFactor, 0, 0);
        }
        else
            $object->Translate(-moveFactor, 0, 0);
    }
    else if(key == '2')
    {
        if(selectedLightSourceIndex >= 0)
        {
            LightSource* lightSource = world->GetLightSources()[selectedLightSourceIndex];
            lightSource->Translate(moveFactor, 0, 0);
        }
        else
            $object->Translate(moveFactor, 0, 0);
    }
    else if(key == '3')
    {
        if(selectedLightSourceIndex >= 0)
        {
            LightSource* lightSource = world->GetLightSources()[selectedLightSourceIndex];
            lightSource->Translate(0, -moveFactor, 0);
        }
        else
            $object->Translate(0, -moveFactor, 0);
    }
    else if(key == '4')
    {
        if(selectedLightSourceIndex >= 0)
        {
            LightSource* lightSource = world->GetLightSources()[selectedLightSourceIndex];
            lightSource->Translate(0, moveFactor, 0);
        }
        else
            $object->Translate(0, moveFactor, 0);
    }
    else if(key == '5')
    {
        if(selectedLightSourceIndex >= 0)
        {
            LightSource* lightSource = world->GetLightSources()[selectedLightSourceIndex];
            lightSource->Translate(0, 0, -moveFactor);
        }
        else
            $object->Translate(0, 0, -moveFactor);
    }
    else if(key == '6')
    {
        if(selectedLightSourceIndex >= 0)
        {
            LightSource* lightSource = world->GetLightSources()[selectedLightSourceIndex];
            lightSource->Translate(0, 0, moveFactor);
        }
        else
            $object->Translate(0, 0, moveFactor);
    }
    else if(key == '7')
    {
        if(selectedLightSourceIndex < 0)
            $object->Rotate(10, 0, 0);
    }
    else if(key == '8')
    {
        if(selectedLightSourceIndex < 0)
            $object->Rotate(0, 10, 0);
    }
    else if(key == '9')
    {
        if(selectedLightSourceIndex < 0)
            $object->Rotate(0, 0, 10);
    }
    else if(key == '-' || key == '_')
    {
        if(selectedLightSourceIndex < 0)
            $object->AddScale(-.01f);
    }
    else if(key == '+' || key == '=')
    {
        if(selectedLightSourceIndex < 0)
            $object->AddScale(.01f);
    }
    else if(key == 'w')
    {
        camera->MoveForward(.5f);
    }
    else if(key == 'a')
    {
        camera->MoveLeft(.5f);
    }
    else if(key == 'd')
    {
        camera->MoveRight(.5f);
    }
    else if(key == 's')
    {
        camera->MoveBackward(.5f);
    }
    else if(key == 'o')
    {
        vector3 observerPosition = camera->GetPosition();
        vector3 objectPosition = $object->GetPosition();
        
        float distance = sqrtf(powf(observerPosition.x - objectPosition.x, 2) + powf(observerPosition.y - objectPosition.y, 2) + powf(observerPosition.z - objectPosition.z, 2));
        camera->DollyZoom(5, distance);
        camera->UpdateProjectionMatrix();
    }
    else if(key == 'p')
    {
        vector3 observerPosition = camera->GetPosition();
        vector3 observerRotation = camera->GetRotation();
        vector3 objectPosition = $object->GetPosition();
        
        float distance = sqrtf(powf(observerPosition.x - objectPosition.x, 2) + powf(observerPosition.y - objectPosition.y, 2) + powf(observerPosition.z - objectPosition.z, 2));
        camera->DollyZoom(-5, distance);
        camera->UpdateProjectionMatrix();
    }
    else if(key == ',')
    {
        if(selectedLightSourceIndex == -1)
        {
            modelIndex--;
            if(modelIndex < 0)
            {
                selectedLightSourceIndex = (int)world->GetLightSources().size() - 1;
            }
            else
                $object->SetModel(models[modelIndex]);
        }
        else
        {
            selectedLightSourceIndex--;
            if(selectedLightSourceIndex < 0)
            {
                modelIndex = modelCount - 1;
                $object->SetModel(models[modelIndex]);
            }
        }
    }
    else if(key == '.')
    {
        if(selectedLightSourceIndex == -1)
        {
            modelIndex++;
            if(modelIndex >= modelCount)
            {
                selectedLightSourceIndex = 0;
            }
            else
                $object->SetModel(models[modelIndex]);
        }
        else
        {
            selectedLightSourceIndex++;
            if(selectedLightSourceIndex >= world->GetLightSources().size())
            {
                selectedLightSourceIndex = -1;
                modelIndex = 0;
                $object->SetModel(models[modelIndex]);
            }
        }
    }
    
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    world = new World();
    camera = new Camera(world);
    camera->SetPosition(20, 20, 20);
    camera->SetRotation(20, -45);
    camera->shouldDraw = true;
    
    directorCamera = new Camera(world);
    directorCamera->SetPosition(30, 30, 30);
    directorCamera->SetRotation(20, -35);
    directorCamera->SetFar(1000);
    
    
    world->AddCamera(camera);
    world->AddCamera(directorCamera);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize($window_width, $window_height);
    glutCreateWindow("OpenGL");
    
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(passiveMotion);
    glutMotionFunc(motion);
    glutKeyboardUpFunc(keyboard);
    //glutSpecialUpFunc(hadleSpecialKeyboard);
    
    glEnable(GL_CULL_FACE);
    glDepthMask(true);
    glDepthFunc(GL_LEQUAL);
    
    WorldObject* obj = new WorldObject();
    obj->SetWorld(world);
    obj->SetModel("/Volumes/Igor/Users/igorlira/Desktop/yoda.obj");
    obj->AddScale(-.9);
    obj->Translate(0, 0, 0);
    
    $object = new WorldObject();
    //$object->SetWorld(world);
    $object->SetModel("/Volumes/Igor/Users/igorlira/Desktop/yoda.obj");
    $object->AddScale(-.9);
    //obj.Rotate(80, 90, 0);
    //$object->Translate(0, 0, 100);
    
    $object = obj;
    
    LightSource* ls = new LightSource({ 1, 0, 0 }, 1.f);
    ls->setPosition(10, 10, 10);
    world->AddLightSource(ls);
    
    ls = new LightSource({ 1, 1, .0 }, 1.f);
    ls->setPosition(0, 10, 10);
    world->AddLightSource(ls);
    
    glutMainLoop();
    
    return 0;
}