#include "Screenshake.h" 
/*
   _____  _____ _____  ______ ______ _   _  _____ _    _          _  ________ 
  / ____|/ ____|  __ \|  ____|  ____| \ | |/ ____| |  | |   /\   | |/ /  ____|
 | (___ | |    | |__) | |__  | |__  |  \| | (___ | |__| |  /  \  | ' /| |__   
  \___ \| |    |  _  /|  __| |  __| | . ` |\___ \|  __  | / /\ \ |  < |  __|  
  ____) | |____| | \ \| |____| |____| |\  |____) | |  | |/ ____ \| . \| |____ 
 |_____/ \_____|_|  \_\______|______|_| \_|_____/|_|  |_/_/    \_\_|\_\______|
                                                                              
  @author Amadeus Chia amadeusjinhan.chia@digipen.edu
  @brief Contains the implementation and logic of screenshaking. Note that you MUST include the UpdateCameraShaker function into the update loop in order to use it. 
  
  Increment Trauma by any value between 0 to 1 to induce screen shaking.
*/
float trauma;                       //Value clamped rom 0 to 1, used to control shake
float traumaMultiplier = 16.f;      //Shake POWERRR. Magic number for now.
float traumaMagnitude = 7.f;        //Magnitude, the range of movement of the camera translation
float traumaDecay = 1.3f;           //Speed at which the trauma decays at
float timeCounter = 0;              //time step for a smooooth transition in noise

//Returns a random float based a seed 
float GetPerlinFloat(float seed){
    //-0.5f*2f is a conversion to change the mapping of the values returned by noise.
    //We want values from -1 to 1 instead of 0 to 1.
    return(CP_Random_Noise(seed,timeCounter,0)-0.5f)*2.f;
}
//Returns a random vector based off of perlin floats. Currently locked into seeds 1 and 10 for x y respectively.
CP_Vector GetRandomVector(void){
    return CP_Vector_Set(GetPerlinFloat(1),GetPerlinFloat(10));
}
//Main function to update the position of the camera and do the shaking.
//NOTE: Has to be called FIRST before any background clears or it won't work!!
void UpdateCameraShaker(void){
    trauma = CP_Math_ClampFloat(trauma,0.f,1.f);        //Need to clamp the values. 
    if(trauma>0){
        //Increments the time exponentially by trauma*multiplier. This will get a smooth movement in the nosie function
        timeCounter+=CP_System_GetDt()*(float)pow(trauma,0.3f)*traumaMultiplier;
        //Get a new position scaled by the magnitude and trauma. 
        CP_Vector newPosition = CP_Vector_Scale(GetRandomVector(),traumaMagnitude*trauma);
        //Translate the canvas postion by those values.
        CP_Settings_Translate(newPosition.x*traumaMagnitude*trauma,newPosition.y*traumaMagnitude*trauma);
        //Decrement the trauma values down. technically it's 1.3f^2.
        trauma-=CP_System_GetDt()*traumaDecay*(trauma+0.3f);
    }
    else {
        //By right there should be some resetting of camera position here?
    }
}