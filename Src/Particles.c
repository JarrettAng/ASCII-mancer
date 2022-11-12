#include "Particles.h"
#include "Screenshake.h"
#include "SoundManager.h"
#include "WaveSystem.h"
#include "Grid.h"
//Particle Array. Will use some form of Object Pooling.
//ParticleCount defined in particles.h, 1000. 
Particle particleArray[PARTICLECOUNT];
particleIndex =0;

//This is where I'll declare "anim strings for animation"
char* SparkleAnimString = "x+*\".";
char* ZombieDeathAnimString = "ZZZZ    ZZZZ    ZZZZ    ZZZNNzznncu*\'`";
char* ZombieAnim = "ZzZzZzZzZzZzZzZz";
char* ZombieSpawnAnimString = "@Oo*\'";


//Function that handles creating of particle.
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
        .timeBeforeActive = timeBeforeActive
    };

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


    }
    else{
        // ResetParticle(particlePointer);
    }

}

//Function that resets the particle. Called when particle's lifetime is 0 or less.
//Called when lifetime < 0 
void ResetParticle(Particle* particlePointer){

    particlePointer->x = 0;
    particlePointer->y = 0;
    particlePointer->xVelocity=0;
    particlePointer->yVelocity=0;
    particlePointer->lifeTime =0;
    particlePointer->timeBeforeActive=0;
    particlePointer->force = CP_Vector_Zero();
    //The other variables probably do not need to be reset.
}

//Temp function for testing the radial particles and it's anim
//Maybe I'll make custom "particle functions" for things like spawning/despawning of enemies etc.
void RadialParticleVaried(float x, float y){

    int randomParticleCount = CP_Random_RangeInt(8,30);
    //CP_Vector gravity = CP_Vector_Scale(VECTOR_DOWN,9.81f);
    for(short i = 0;i<(short)randomParticleCount;++i)
    {
        // float randomForceVariance = CP_Random_RangeFloat(8.f,15.f);
        //Tweak the force here. For a proper circle, do not include variance. Will change this later
        float randomForceVariance = CP_Random_RangeFloat(2.f,8.f);
        float angle = (float)(360/randomParticleCount)*i+(CP_Random_Gaussian()*2.f);
        CP_Vector forceDirection = CP_Vector_Scale(AngleToVector(angle),randomForceVariance);
        //forceDirection = CP_Vector_Add(forceDirection,gravity);
        CreateParticle(x,y,1.0f, 55.f,0.f, MENU_RED, forceDirection,SparkleAnimString,TRUE,0);
    }
}


void RadialParticle(float x, float y,int particleCount,float force){
    for(short i =0; i<particleCount;++i){
        float angle = (float)360/particleCount*i;
        CP_Vector forceDirection = CP_Vector_Scale(AngleToVector(angle),force);
        CreateParticle(x,y,1.0f,55.f,0.f,MENU_RED,forceDirection,SparkleAnimString,TRUE,0);
    }
}

void ZombieDeathParticle(float x, float y){
    CreateParticle(x,y,1.8f,GetCellSize()/3,0,MENU_RED,CP_Vector_Zero(),ZombieDeathAnimString,FALSE,0);
    trauma+=0.5f;
}

void ZombieSpawnParticle(float x, float y){
    CreateParticle(x,y,0.5f,120.f,0.1f,MENU_RED,CP_Vector_Zero(),ZombieSpawnAnimString,TRUE,0);
}
void ZombieToPlayerParticle(float x,float y,int health){
    CP_Vector dirToPlayer = CP_Vector_Set((CP_System_GetWindowWidth()*0.05f)-x,(GetGridPlayingArea()/2)+GetGridTopBuffer() - y);
    dirToPlayer = CP_Vector_Normalize(dirToPlayer);
    CreateParticle(x,y,2.f,GetCellSize()*((float)health/(float)3),0,MENU_RED,dirToPlayer,ZombieAnim,TRUE,0);
    trauma+=0.5f;
}

void CreateParticleEmission(float x, float y,int burstCount, float duration){
    for(short i=0; i<burstCount; ++i){
            for(short j =0; j<20;++j){
                float angle = (float)360/20*j;
                CP_Vector forceDirection = CP_Vector_Scale(AngleToVector(angle),8.f);
                CreateParticle(x,y,1.0f,55.f,0.f,MENU_RED,forceDirection,SparkleAnimString,TRUE,(duration/(float)burstCount)*i);
            }
    }
}

void CreateMovementAnimation(int x, int y, int moveSpeed, float duration){
    for(short i=0; i<moveSpeed; ++i){
        CreateParticle(GridXToPosX(x),GridYToPosY(y),1.0f,GetCellSize(),0.f,MENU_GRAY,CP_Vector_Zero(),"<<<",FALSE,(duration*i));
    }
}

//Function that handles the animation and drawing of the particle to screen. Sets the color but not the alignment.
void DrawParticle(Particle* particlePointer){
    if(particlePointer->lifeTime >= 0 && particlePointer->timeBeforeActive <=0){

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
        singleCharFrame[1]='\0';    //set the null char or drawtext will have undefined behaviour reading indefinitely
        CP_Font_DrawText(&singleCharFrame, particlePointer->x, particlePointer->y);
    }
}

//Function that is required to be called to update all particles. If particles don't work, check that this is in update loop.
void UpdateEffects(void){
    //Testing radial particle. Not sure if input should be checked here. Maybe it should be.
    // if(CP_Input_MouseClicked()){
    //     // ZombieDeathParticle(CP_Input_GetMouseX(),CP_Input_GetMouseY());
    //     // NukeParticle(CP_Input_GetMouseX(),CP_Input_GetMouseY());
    //     PlaySoundEx(TETROMINOEXPLODE,CP_SOUND_GROUP_SFX);
    //     SendDamage(PosXToGridX(CP_Input_GetMouseX()),PosYToGridY(CP_Input_GetMouseY()),1);
    // }
    // if(CP_Input_MouseClicked())
    // {
    //     CreateParticleEmission(CP_Input_GetMouseX(),CP_Input_GetMouseY(),5,0.5f);
    //     //  RadialParticle(CP_Input_GetMouseX(),CP_Input_GetMouseY(),20,8.f);
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

//RANDOM MATH UTILS FUNCTION!! WILL PROBABLY MOVE IT TO A UTILS.C!!
CP_Vector AngleToVector(float degreeAngle){
    float angleRadians = CP_Math_Radians(degreeAngle);
    //Note, if the starting vector points up, the calculations will be different.
    float x = (VECTOR_UP.x*cosf(angleRadians)-VECTOR_UP.y*sinf(angleRadians));
    float y = (VECTOR_UP.x*sinf(angleRadians)+VECTOR_UP.y*cosf(angleRadians));
    return CP_Vector_Set(x,y);
}