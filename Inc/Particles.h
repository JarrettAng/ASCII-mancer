/*!
@file	  Particles.h
@author	  Amadeus Chia (amadeusjinhan.chia@digipen.edu)
@date     18/11/2022
@brief    This header file contains the information about all particles in the game. The values of the particles are tweaked in the source file.
________________________________________________________________________________________________________*/
#pragma once
#include "cprocessing.h"
#include "ColorTable.h"     
#include <math.h>           //some angle to vector calculations are needed
#include <string.h>         //used for animating the particles
#include "EnemyStats.h"     //for grabbing enemy data

//====================== DEFINES ======================
#define VECTOR_UP CP_Vector_Set(0,-1)
#define VECTOR_DOWN CP_Vector_Set(0,1)
#define PARTICLECOUNT 1001


typedef struct Particle{
    float x;
    float y;
    float xVelocity;
    float yVelocity;        
    float lifeTime;         //Current lifetime remaining for particle. Disappears when <0
    float timeBeforeActive; //Used to control the delay before a particle spawns.
    float cachedLifeTime;   //Used for calculating time particle has been alive for.
    float size;             //NOTE!! Size is normally set to cell size!
    BOOL isRGB;
    BOOL isLoop;            //Manually set for certain particles in source
    CP_Vector force;        //force that is acting on particle. Cummulative and will be calculated in UpdateParticle.
    char* animString;
    CP_Color color;
    float gravityScale;    
} Particle;

/*
@brief Create Particle will add the particle to the particle array for use.

@param[in] Params : Refer to particle struct for full list of params.
*/
void CreateParticle(float xPos, float yPos, float lifeTime, float size,float gravityScale, CP_Color color,CP_Vector force,const char* animString,BOOL isRGB,float timeBeforeActive);

/*
@brief Updates a single particle's position and physics and deducts lifetime.
@param[in] particlePointer pointer to a particle in the particleArray.
*/
void UpdateParticle(Particle* particlePointer);
/*
@brief Draws the particle if it still has lifetime.
@param[in] particlePointer pointer to a particle in the particleArray.
*/
void DrawParticle(Particle* particlePointer);
/*
@brief Needs to be called every frame in update in order for all particles to be updated!
*/
void UpdateEffects(void);
/*
@brief Helper function for RGB values
@param[in] Color,timeStep Returns a HSL color based on the time step given. 
*/
CP_Color LerpedHSLColor(CP_Color color,float timeStep);
/*
@brief Spawns a radial particle with no variance. 
@param[in] Position is the particles x and y positions respectively.
@param[in] Count is the number of particles that will be spawned. The angle of the particles is the 360/count. @param[in] Force controls how far and fast the particle moves.
*/
void RadialParticle(float x, float y,int particleCount,float force);
/*
@brief Spawns a radial particle with varied force and count. (set in source)
@param[in] Position is the particles x and y positions respectively.
*/
void RadialParticleVaried(float x, float y);
/*
@brief Spawns the enemy's respective death particle. (Animation set in source)
@param[in] Position is the particles x and y positions respectively.
@param[in] Type refers to the zombies type in enemystats.c
*/
void ZombieDeathParticle(float x, float y,ZombieType type);
/*
@brief Spawns the enemy's spawn particle. (Animation set in source)
@param[in] Position is the particles x and y positions respectively.
*/
void ZombieSpawnParticle(float x, float y);
/*
@brief Spawns the particle of the enemy moving towards the player.
@param[in] Position is the particles x and y positions respectively.
*/
void ZombieToPlayerParticle(float x,float y);
/*
@brief Helper function for spawning particles in a circle. Returns the vector from the angle.
@param[in] Angle is in degrees.
*/
CP_Vector AngleToVector(float degreeAngle);