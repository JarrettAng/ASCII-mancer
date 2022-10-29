//Header files here
#include "Grid.h"
#include "Particles.h"
#include "WaveSystem.h"
#include "Screenshake.h"
void gameLevelInit(void){
    test_init();
    InitWaveSystem();
}
void gameLevelUpdate(void){
    UpdateCameraShaker();
    //MAIN GAME LOOP GOES HERE
    UpdateWave();




    //DRAWING AND UPDATING OF GRID
    test_update();



    //UPDATE VFX
    UpdateEffects();

    //CLEAR SCREEN
    CP_Graphics_ClearBackground(BLACK);
}
void gameLevelExit(void){

}