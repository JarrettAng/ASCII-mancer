/*!
@file	  Particles.c
@author	  Amadeus Chia (amadeusjinhan.chia@digipen.edu)
@date     18/11/2022
@brief    This source file for the handling of particles in game
________________________________________________________________________________________________________*/

#include "Particles.h"
#include "Screenshake.h"        
#include "SoundManager.h"       
#include "WaveSystem.h"         //for access to enemies
#include "Grid.h"               //for grid coordinate conversions

//Particle Array. Will use some form of Object Pooling.
//ParticleCount defined in particles.h, 1000. 
Particle particleArray[PARTICLECOUNT];
particleIndex =0;

//=========== PARTICLE ANIMATIONS ===========
char* SparkleAnimString = "x+*\".";
char* ZombieDeathAnimString = "ZZZZ    ZZZZ    ZZZZ    ZZZNNzznncu*\'`";
char* LeaperDeathAnimString = "LLLL    LLLL    LLLL    LLL[[ii;;::*\'`";
char* GraveDeathAnimString = "GGGG    GGGG    GGGG    zznncu*\'`";
char* BreakerDeathAnimString = "BBBB    BBBB    BBBB    BBB33bbcccu*\'`";
char* TankDeathAnimString = "TTTT    TTTT    TTTT    TTTYYwwnncu*\'`";
char* WallDeathAnimString = "XXXX    XXXX    XXXX    XXXvvwwnncu*\'`";
char* ZombieAnim = "ZzZzZzZzZzZzZzZz";
char* ZombieSpawnAnimString = "@Oo*\'";



//Function that handles creating of particle. Calling this will add the particle to the array
void CreateParticle(float xPos, float yPos, float lifeTime, float size,float gravityScale, CP_Color color,CP_Vector force,const char* animString,BOOL isRGB,float timeBeforeActive){

    Particle newParticle = {
        .x = xPos,
        .y = yPos,
        .lifeTime = lifeTime,
        .cachedLifeTime = lifeTime,
        .size = size,
        .gravityScale = gravityScale,
        .color = color,
        .xVelocity = 0,
        .yVelocity = 0,
        .force = force,
        .animString = animString,
        .isRGB = isRGB,
        .timeBeforeActive = timeBeforeActive,
        .isLoop = FALSE
    };

    //particle%PARTICLECOUNT will loop over to the start of the array
    particleArray[particleIndex%PARTICLECOUNT] = newParticle;
    particleIndex++;
}

//Function that updates a given particle. Used in Update effects forloop. Handles particle movement and lifetime.
void UpdateParticle(Particle* particlePointer){

    //Checks if there's a delay for the particle before it can be shown.     
    if(particlePointer->timeBeforeActive > 0){
        particlePointer->timeBeforeActive-=CP_System_GetDt();
        return;
    } 
    if(particlePointer->lifeTime > 0){
        //Add velocity first THEN add position for SI Euler. 
        //Doing the other way around is Explicit Euler and inaccurate!
        particlePointer->xVelocity= particlePointer->force.x;
        particlePointer->yVelocity= particlePointer->force.y;

        particlePointer->x += particlePointer->xVelocity;
        particlePointer->y += particlePointer->yVelocity;

        particlePointer->lifeTime -= CP_System_GetDt();
        particlePointer->force.y += particlePointer->gravityScale * 9.81f*CP_System_GetDt();
        //If the particle loops, it has infinite lifetime. 
        if(particlePointer->isLoop && particlePointer->lifeTime <= 0){
            particlePointer->lifeTime = particlePointer->cachedLifeTime;
        }
    }
    else{
        // catch any errors here
    }

}

//Spawns a radial particle with variance in angle, force and particle count
void RadialParticleVaried(float x, float y){
    //Values are hardcoded and tuned for the game.
    int randomParticleCount = CP_Random_RangeInt(20,30);
    //CP_Vector gravity = CP_Vector_Scale(VECTOR_DOWN,9.81f);
    for(short i = 0;i<(short)randomParticleCount;++i)
    {
        float randomForceVariance = CP_Random_RangeFloat(1.f,4.f);
        float angle = (float)(360/randomParticleCount)*i+(CP_Random_Gaussian()*2.f);
        CP_Vector forceDirection = CP_Vector_Scale(AngleToVector(angle),randomForceVariance);
        CreateParticle(x,y,.5f, GetCellSize()/3,0.f, MENU_GRAY, forceDirection,SparkleAnimString,FALSE,0);
    }
}

//Spawns a radial particle with user defined color
void RadialParticleColor(float x, float y,int particleCount,float force,CP_Color color){
    for(short i =0; i<particleCount;++i){
        float angle = (float)360/particleCount*i;
        CP_Vector forceDirection = CP_Vector_Scale(AngleToVector(angle),force);
        CreateParticle(x,y,1.0f,GetCellSize()/3,0.f,color,forceDirection,SparkleAnimString,FALSE,0);
    }
}

//Spawns a radial particle with RGB colors
void RadialParticleRGB(float x, float y,int particleCount,float force){
    for(short i =0; i<particleCount;++i){
        float angle = (float)360/particleCount*i;
        CP_Vector forceDirection = CP_Vector_Scale(AngleToVector(angle),force);
        CreateParticle(x,y,1.0f,GetCellSize()/3,0.f,MENU_RED,forceDirection,SparkleAnimString,TRUE,0);
    }
}
//Spawns the death particle of a zombie.
void ZombieDeathParticle(float x, float y,ZombieType type){
    switch (type){
    case ZOMBIE:
        CreateParticle(x,y,1.8f,GetCellSize()/3,0,MENU_RED,CP_Vector_Zero(),ZombieDeathAnimString,FALSE,0);
        trauma+=0.3f;
        break;
    case LEAPER:
        CreateParticle(x,y,1.8f,GetCellSize()/3,0,MENU_RED,CP_Vector_Zero(),LeaperDeathAnimString,FALSE,0);
        trauma+=0.3f;
        break;
    case TANK:
        CreateParticle(x,y,1.8f,GetCellSize()/3,0,MENU_RED,CP_Vector_Zero(),TankDeathAnimString,FALSE,0);
        trauma+=0.5f;
        break;
    case BREAKER:
        CreateParticle(x,y,1.8f,GetCellSize()/3,0,MENU_RED,CP_Vector_Zero(),BreakerDeathAnimString,FALSE,0);
        trauma+=0.5f;
        break;
    case GRAVE:
        CreateParticle(x,y,1.8f,GetCellSize()/3,0,GREEN,CP_Vector_Zero(),GraveDeathAnimString,FALSE,0);
        trauma+=0.5f;
        break;
    default:
        CreateParticle(x,y,1.8f,GetCellSize()/3,0,MENU_RED,CP_Vector_Zero(),ZombieDeathAnimString,FALSE,0);
        break;
    }
}

//Spawns the spawn particle of zombies.
void ZombieSpawnParticle(float x, float y){
    CreateParticle(x,y,0.5f,GetCellSize(),0.1f,MENU_RED,CP_Vector_Zero(),ZombieSpawnAnimString,TRUE,0);
}

//Spawns the particle of the zombie moving to the player 
void ZombieToPlayerParticle(float x,float y){
    CP_Vector dirToPlayer = CP_Vector_Set((CP_System_GetWindowWidth()*0.05f)-x,(GetGridPlayingArea()/2)+GetGridTopBuffer() - y);
    dirToPlayer = CP_Vector_Normalize(dirToPlayer);
    CreateParticle(x,y,2.f,GetCellSize()/3,0,MENU_RED,dirToPlayer,ZombieAnim,TRUE,0);
    trauma+=0.5f;
}

//Handles the creation of particles with delay
void CreateParticleEmission(float x, float y,int burstCount, float duration){
    for(short i=0; i<burstCount; ++i){
            for(short j =0; j<20;++j){
                float angle = (float)360/20*j;
                CP_Vector forceDirection = CP_Vector_Scale(AngleToVector(angle),8.f);
                CreateParticle(x,y,1.0f,GetCellSize()/3,0.f,MENU_RED,forceDirection,SparkleAnimString,TRUE,(duration/(float)burstCount)*i);
            }
    }
}

//Function that handles the animation and drawing of the particle to screen. Sets the color but not the alignment.
void DrawParticle(Particle* particlePointer){
    if((particlePointer->lifeTime >= 0)&& particlePointer->timeBeforeActive <=0){

        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER,CP_TEXT_ALIGN_V_MIDDLE);
        CP_Settings_TextSize(particlePointer->size);
        
        //Gets frame count and time step by mapping number of frames to particle lifetime using lerp.
        float elapsedLifeTime = particlePointer->cachedLifeTime-particlePointer->lifeTime;
        float timeStep = elapsedLifeTime/particlePointer->cachedLifeTime;
        int totalFrames = (int)strlen(particlePointer->animString);
        int frameCount = (int)CP_Math_LerpFloat(0,(float)(totalFrames),timeStep);

        //Note : Lerp 2 values at one time for Razer RGB. (for future reference)
        
        if(particlePointer->isRGB){
            CP_Settings_Fill(LerpedHSLColor(particlePointer->color,timeStep));
        } else{
            CP_Settings_Fill(particlePointer->color);
        }
        //Char array size of 2, where 0 is the frame and 1 is '\0'
        char singleCharFrame[2];
        //memcpy is better than array=array.
        memcpy(&singleCharFrame,&particlePointer->animString[frameCount],1);
        singleCharFrame[1]='\0';    //set the null char if not drawtext will have undefined behaviour reading indefinitely
        CP_Font_DrawText(*&singleCharFrame, particlePointer->x, particlePointer->y);
    }
}

//Function that is required to be called to update all particles. If particles don't work, check that this is in update loop.
void UpdateEffects(void){
    // if(CP_Input_MouseTriggered(1)){
    //     RadialParticleVaried(CP_Input_GetMouseX(),CP_Input_GetMouseY());
    // }
    //Main loop for handling particle movement and rendering.
    //Important to loop through the whole array because the index wraps around.
    for(short i =0; i< PARTICLECOUNT; ++i){
        if(particleArray[i].lifeTime <=0) continue; //an attempt at optimisation to skip dead particles.
        UpdateParticle(&particleArray[i]);
        DrawParticle(&particleArray[i]);
    }
}

//Retuns color from lerping of hues to make rainbow color effect.
//Check if this works if color is white. I think it doesn't
CP_Color LerpedHSLColor(CP_Color color,float timeStep){
    CP_ColorHSL startColor = CP_ColorHSL_FromColor(color);
    CP_ColorHSL endColor = CP_ColorHSL_Create(startColor.h-1,startColor.s,startColor.l,startColor.a);
    CP_ColorHSL HSLColor = CP_ColorHSL_Lerp(startColor,endColor,timeStep);
    return CP_Color_FromColorHSL(HSLColor);
}

//Helper math function to convert angles to vector.
CP_Vector AngleToVector(float degreeAngle){
    float angleRadians = CP_Math_Radians(degreeAngle);
    //Note, if the starting vector points up, the calculations will be different.
    float x = (VECTOR_UP.x*cosf(angleRadians)-VECTOR_UP.y*sinf(angleRadians));
    float y = (VECTOR_UP.x*sinf(angleRadians)+VECTOR_UP.y*cosf(angleRadians));
    return CP_Vector_Set(x,y);
}