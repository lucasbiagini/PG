//
//  LightSource.h
//  ProjetoPG
//
//  Created by Igor Lira on 6/17/15.
//  Copyright (c) 2015 Igor Lira. All rights reserved.
//

#ifndef __ProjetoPG__LightSource__
#define __ProjetoPG__LightSource__

#include "Header.h"

class LightSource
{
    vector3 position;
    vector3 color;
    float intensity;
    
public:
    LightSource(vector3 color, float intensity);
    
    void setPosition(float x, float y, float z);
    vector3 getColor();
    float getIntensity();
    
    vector3 GetPosition();
    vector3 CalculateLight(class WorldObject* object, vector3 vertex, vector3 vertexNormal, class Camera* camera);
    
    void Translate(float x, float y, float z);
};

#endif /* defined(__ProjetoPG__LightSource__) */
