//
//  World.cpp
//  ProjetoPG
//
//  Created by Igor Lira on 6/17/15.
//  Copyright (c) 2015 Igor Lira. All rights reserved.
//

#include "World.h"
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include "Camera.h"

void World::AddObject(WorldObject *obj)
{
    this->objects.push_back(obj);
}

void World::AddLightSource(LightSource *lightSource)
{
    this->lightSources.push_back(lightSource);
}

void World::DrawFloor()
{
    glLineWidth(3);
    glBegin(GL_LINES);
    
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(100, 0, 0);
    
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1000, 0);
    
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 100);

    float step = 1.f;
    int count = 1000;
    for(int x = -count; x < count; x++)
    {
        float pos = step * (float)x;
        
        glColor3f(.2, .2, .2);
        glVertex3f(-100000, 0, pos);
        glVertex3f(100000, 0, pos);
        
        glVertex3f(pos, 0, -100000);
        glVertex3f(pos, 0, 100000);
    }
    
    glEnd();
}

void World::AddCamera(Camera* camera)
{
    this->cameras.push_back(camera);
}

void World::Draw(Camera* camera)
{
    this->DrawFloor();
    
    glMatrixMode(GL_MODELVIEW);
    camera->LoadMatrix();
    
    for (int i = 0; i < this->objects.size(); i++)
    {
        WorldObject* obj = this->objects[i];
        obj->Draw(camera);
    }
    
    camera->LoadMatrix();
    for(int i = 0; i < this->lightSources.size(); i++)
    {
        LightSource* light = this->lightSources[i];
        vector3 pos = light->GetPosition();
        vector3 color = light->getColor();
        
        glPointSize(40);
        glBegin(GL_POINTS);
        glColor3f(color.x, color.y, color.z);
        glVertex3f(pos.x, pos.y, pos.z);
        glEnd();
    }
    
    camera->LoadMatrix();
    for(int i = 0; i < this->cameras.size(); i++)
    {
        Camera* cam = this->cameras[i];
        if(cam == camera || !cam->shouldDraw)
            continue;
        
        cam->Draw(camera);
    }
    
    //glPopMatrix();
}

std::vector<LightSource*> World::GetLightSources()
{
    return this->lightSources;
}