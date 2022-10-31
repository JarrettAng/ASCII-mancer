//Header files here
#include "Grid.h"
#include "Particles.h"
#include "WaveSystem.h"
#include "Screenshake.h"
#include "Hearts.h"
void gameLevelInit(void){
    test_init();
    InitWaveSystem();
    InitializeLife();
}
void gameLevelUpdate(void){
    UpdateCameraShaker();
    //MAIN GAME LOOP GOES HERE
    UpdateWave();




    //DRAWING AND UPDATING OF GRID
    test_update();

    //UPDATE HEARTS
    UpdateLife();

    //UPDATE VFX
    UpdateEffects();

    //CLEAR SCREEN
    CP_Graphics_ClearBackground(BLACK);
}
void gameLevelExit(void){
    ClearHearts();
}