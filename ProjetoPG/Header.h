//
//  Header.h
//  ProjetoPG
//
//  Created by Igor Lira on 6/17/15.
//  Copyright (c) 2015 Igor Lira. All rights reserved.
//

#ifndef ProjetoPG_Header_h
#define ProjetoPG_Header_h

#ifndef M_PI
#define M_PI 3.14156
#endif


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

typedef struct
{
    vector3 position;
    vector3 direction;
} ray;

float toRadians(float degrees);
float dot(vector3 a, vector3 b);
float len(vector3 a);
vector3 norm(vector3 x);

#endif
