//
//  World.h
//  ProjetoPG
//
//  Created by Igor Lira on 6/17/15.
//  Copyright (c) 2015 Igor Lira. All rights reserved.
//

#ifndef __ProjetoPG__World__
#define __ProjetoPG__World__

#include <vector>
#include "WorldObject.h"
#include "LightSource.h"
#include "Camera.h"

#define MAX_OBJECTS 100
#define MAX_LIGHTS 100

class World {
    std::vector<WorldObject*> objects;
    std::vector<LightSource*> lightSources;
    std::vector<Camera*> cameras;
    
public:
    void Draw(class Camera*);
    void AddObject(WorldObject* obj);
    void AddLightSource(LightSource* lightSource);
    void AddCamera(class Camera*);
    void DrawFloor();
    
    std::vector<LightSource*> GetLightSources();
};

#endif /* defined(__ProjetoPG__World__) */
