#include "Particles.h"
#include "testfile.h"
#include "Screenshake.h"
#include "SoundManager.h"
//Particle Array. Will use some form of Object Pooling.
//ParticleCount defined in particles.h, 1000. 
Particle particleArray[PARTICLECOUNT];
int particleIndex =0;

//This is where I'll declare "anim strings for animation"
char* SparkleAnimString = "x+*\".\0";
char* ZombieDeathAnimString = "ZZZZ    ZZZZ    ZZZZ    ZZZNNzznncu*\'`\0";
char* NukeAnimString = "@Oo*\'\0";

//Function that handles creating of particle.
void CreateParticle(float xPos, float yPos, float lifeTime, float size,float gravityScale, CP_Color color,CP_Vector force,const char* animString,BOOL isRGB){

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
        .isRGB = isRGB
    };

    //If the index happens to be >=MAXPARTICLES, then loop back and start reusing particles from index 0 onwards.
    //I don't think I should even need to check if the particle is still alive since it's a Queue sort of thing.  

    particleArray[particleIndex%(PARTICLECOUNT)] = newParticle;
    particleIndex++;

}

//Function that updates a given particle. Used in Update effects forloop. Handles particle movement and lifetime.
void UpdateParticle(Particle* particlePointer){

    if(particlePointer->lifeTime > 0){

        //Add velocity first THEN add position for SI Euler. 
        //Doing the other way around is Explicit Euler and inaccurate!
        //There will be NO COLLISIONS. These are simple particles.
        particlePointer->xVelocity= particlePointer->force.x;
        particlePointer->yVelocity= particlePointer->force.y;//+(9.81f*particlePointer->gravityScale);

        particlePointer->x += particlePointer->xVelocity;
        particlePointer->y += particlePointer->yVelocity;

        particlePointer->lifeTime -= CP_System_GetDt();
        particlePointer->force.y += particlePointer->gravityScale * 9.81f*CP_System_GetDt();
        //Deduct life time
        //Animate particle over lifetime
        //if Lifetime = 0, hide, particle, reset pos
        // if (particlePointer->force.y > CP_System_GetWindowHeight()) particlePointer->force.y = 0;

    }
    else{
        ResetParticle(particlePointer);
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
        CreateParticle(x,y,1.0f, 55.f,0.f, MENU_RED, forceDirection,SparkleAnimString,TRUE);
    }
}


void RadialParticle(float x, float y,int particleCount,float force){
    for(short i =0; i<particleCount;++i){
        float angle = (float)360/particleCount*i;
        CP_Vector forceDirection = CP_Vector_Scale(AngleToVector(angle),force);
        CreateParticle(x,y,1.0f,55.f,0.f,MENU_RED,forceDirection,SparkleAnimString,TRUE);
    }
}

void ZombieDeathParticle(float x, float y){
    CreateParticle(x,y,1.8f,50.f,0,MENU_RED,CP_Vector_Zero(),ZombieDeathAnimString,FALSE);
    trauma+=0.5f;
}

void NukeParticle(float x, float y){
    CreateParticle(x,y,0.5f,120.f,0.1f,MENU_RED,CP_Vector_Zero(),NukeAnimString,TRUE);
    trauma+=1.f;
}


//Function that handles the animation and drawing of the particle to screen. Sets the color but not the alignment.
void DrawParticle(Particle* particlePointer){
    // CP_Font currentFont = CP_Font_Load("Assets/PressStart2P-Regular.ttf");
    // CP_Font_Set(CP_Font_GetDefault());
    if(particlePointer->lifeTime > 0){

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
    // CP_Font_Set(currentFont);
}

//Function that is required to be called to update all particles. If particles don't work, check that this is in update loop.
void UpdateEffects(void){
    //TODO : Emitters maybe
    //Testing radial particle. Not sure if input should be checked here. Maybe it should be.
    if(CP_Input_MouseClicked()){
        // ZombieDeathParticle(CP_Input_GetMouseX(),CP_Input_GetMouseY());
        NukeParticle(CP_Input_GetMouseX(),CP_Input_GetMouseY());
        PlaySoundEx(TETROMINOEXPLODE,CP_SOUND_GROUP_SFX);
    }
    if(CP_Input_MouseClicked())
    {
         RadialParticle(CP_Input_GetMouseX(),CP_Input_GetMouseY(),20,8.f);
    }
    //Main loop for handling particle movement and rendering.
    for(short i =0; i< particleIndex%PARTICLECOUNT; ++i){
        //Maybe I can just check if lifetime>0 here so I don't have to check twice? 
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