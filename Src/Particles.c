#include "Particles.h"
#include "testfile.h"

//Particle Array. Will use some form of Object Pooling.
//Probably do a #define for MAXPARTICLES = 1000 or something
Particle particleArray[PARTICLECOUNT];
int particleIndex =0;

//This is where I'll declare "anim strings for animation"
char* SparkleAnimString = "x+*\".\0";
char* ZombieDeathAnimString = "Z Z Z ZNzncu*\'`\0";


//TODO: OBJECT POOLING

//Function that handles creating of particle.
void CreateParticle(float xPos, float yPos, float lifeTime, float size,float gravityScale, CP_Color color,CP_Vector force,const char* animString){

    Particle newParticle;
    newParticle.x = xPos;
    newParticle.y = yPos;
    newParticle.lifeTime = lifeTime;
    newParticle.cachedLifeTime = lifeTime;
    newParticle.size = size;
    newParticle.gravityScale = gravityScale;
    newParticle.color = color;
    newParticle.xVelocity=0;
    newParticle.yVelocity=0;
    newParticle.force = force;
    newParticle.animString = animString;
    newParticle.gravityScale =0.f;

    //If the index happens to be >=MAXPARTICLES, then loop back and start reusing particles from index 0 onwards.
    //I don't think I should even need to check if the particle is still alive since it's a Queue sort of thing.  

    particleArray[particleIndex%(PARTICLECOUNT)] = newParticle;
    particleIndex++;

}

//Function that updates a given particle. Used in Update effects forloop. Handles particle movement and lifetime.
void UpdateParticles(Particle* particlePointer){

    if(particlePointer->lifeTime > 0){

        //Add velocity first THEN add position for SI Euler. 
        //Doing the other way around is Explicit Euler and inaccurate!
        //There will be NO COLLISIONS. These are simple particles.
        // particlePointer->force.y += particlePointer->gravityScale * 9.81f;
        particlePointer->xVelocity= particlePointer->force.x;
        particlePointer->yVelocity=particlePointer->force.y+(9.81f*particlePointer->gravityScale);

        particlePointer->x += particlePointer->xVelocity;
        particlePointer->y += particlePointer->yVelocity;

        particlePointer->lifeTime -= CP_System_GetDt();
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
void ResetParticle(Particle* particlePointer){

    particlePointer->x = 0;
    particlePointer->y = 0;
    particlePointer->xVelocity=0;
    particlePointer->yVelocity=0;
    particlePointer->lifeTime =0;
    particlePointer->force = CP_Vector_Zero();

}

//Temp function for testing the radial particles and it's anim
//Maybe I'll make custom "particle functions" for things like spawning/despawning of enemies etc.
void RadialParticle(float x, float y){

    int randomParticleCount = CP_Random_RangeInt(8,30);
    //CP_Vector gravity = CP_Vector_Scale(VECTOR_DOWN,9.81f);
    for(short i = 0;i<(short)randomParticleCount;++i)
    {
        float randomForceVariance = CP_Random_RangeFloat(8.f,15.f);
        float angle = (float)(360/randomParticleCount)*i+(CP_Random_Gaussian()*2.f);
        CP_Vector forceDirection = CP_Vector_Scale(AngleToVector(angle),randomForceVariance);
        //forceDirection = CP_Vector_Add(forceDirection,gravity);
        CreateParticle(x,y,0.5f, 55.f,3.f, MENU_RED, forceDirection,SparkleAnimString);
    }

}
void ZombieDeathParticle(float x, float y){
    CreateParticle(x,y,1.2f,50.f,0,MENU_RED,CP_Vector_Zero(),ZombieDeathAnimString);
}


//Function that handles the animation and drawing of the particle to screen. Sets the color but not the alignment.
void DrawParticle(Particle* particlePointer){
    // CP_Font currentFont = CP_Font_Load("Assets/PressStart2P-Regular.ttf");
    // CP_Font_Set(CP_Font_GetDefault());
    if(particlePointer->lifeTime > 0){
        CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER,CP_TEXT_ALIGN_V_MIDDLE);
        CP_Settings_TextSize(particlePointer->size);
        
        float elapsedLifeTime = particlePointer->cachedLifeTime-particlePointer->lifeTime;
        float timeStep = elapsedLifeTime/particlePointer->cachedLifeTime;
        int totalFrames = (int)strlen(particlePointer->animString);
        int frameCount = (int)CP_Math_LerpFloat(0,(float)(totalFrames),timeStep);
        //Lerp 2 values at one time for Razer RGB. 
        CP_ColorHSL startColor = CP_ColorHSL_FromColor(particlePointer->color);
        CP_ColorHSL endColor = CP_ColorHSL_Create(startColor.h-1,startColor.s,startColor.l,startColor.a);
        CP_ColorHSL HSLColor = CP_ColorHSL_Lerp(startColor,endColor,timeStep); 
        CP_Color particleColor = CP_Color_FromColorHSL(HSLColor);
        CP_Settings_Fill(particleColor);
        
        char test2[2];
        memcpy(&test2,&particlePointer->animString[frameCount],1);
        test2[1]='\0';
        CP_Font_DrawText(&test2, particlePointer->x, particlePointer->y);
    }
    // CP_Font_Set(currentFont);
}

//Function that is required to be called to update all particles. If particles don't work, check that this is in update loop.
void UpdateEffects(void){
    //Testing radial particle. Not sure if input should be checked here. Maybe it should be.
    if(CP_Input_MouseClicked()){
        ZombieDeathParticle(CP_Input_GetMouseX(),CP_Input_GetMouseY());
    }
    if(CP_Input_MouseClicked(1))
    {
         RadialParticle(CP_Input_GetMouseX(),CP_Input_GetMouseY());
    }

    //Main loop for handling particle movement and rendering.
    for(short i =0; i< particleIndex%PARTICLECOUNT; ++i){
        //Maybe I can just check if lifetime>0 here so I don't have to check twice? 
        UpdateParticles(&particleArray[i]);
        DrawParticle(&particleArray[i]);
    }
}



//RANDOM MATH UTILS FUNCTION!! WILL PROBABLY MOVE IT TO A UTILS.C!!
CP_Vector AngleToVector(float degreeAngle){
    CP_Vector startVector = VECTOR_UP;
    float angleRadians = CP_Math_Radians(degreeAngle);
    //Note, if the starting vector points up, the calculations will be different.
    float x = (VECTOR_UP.x*cosf(angleRadians)-VECTOR_UP.y*sinf(angleRadians));
    float y = (VECTOR_UP.x*sinf(angleRadians)+VECTOR_UP.y*cosf(angleRadians));
    return CP_Vector_Set(x,y);
}