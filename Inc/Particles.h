#pragma once
#include "cprocessing.h"
#include "ColorTable.h"
#include <math.h>           //Remember to move to a utils.c!!
#include <string.h>

#define VECTOR_UP CP_Vector_Set(0,-1)

typedef struct Particle{
    int UID;                //Particle's uniqueIdentifier to search out the specific particle
    float x;
    float y;
    float xVelocity;
    float yVelocity;        
    float lifeTime;         //Current lifetime remaining for particle. Disappears when <0
    float cachedLifeTime;   //Used for calculating time particle has been alive for.
    float size;             //NOTE!! FONT SIZE GOTTA BE BIG! OR CAN'T SEE SHITTT!! At least like, Min 15.f
    CP_Vector force;        //force that is acting on particle. Cummulative and will be calculated in UpdateParticles.
    CP_Color color;
     
    //BOOL isActive;        //Returns false if the lifeTime is zero. If that's the case then just check against lifeTime?
    //float gravityScale    //Maybe have gravity? Might be nice because it makes it curve
} Particle;

void CreateParticle(float xPos, float yPos, float lifeTime, float size, CP_Color color, CP_Vector force);
void UpdateParticles(Particle* particlePointer);
void DrawParticle(Particle* particlePointer);
void ResetParticle(Particle* particlePointer);
void UpdateEffects(void);
//Temp function for testing
void RadialParticle(float x, float y);

//Move this to utils!!
CP_Vector AngleToVector(float degreeAngle);