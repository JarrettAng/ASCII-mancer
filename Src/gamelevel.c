//Header files here
#include "Grid.h"
#include "Particles.h"


void gameLevelInit(void){
    test_init();
}
void gameLevelUpdate(void){

    //MAIN GAME LOOP GOES HERE





    //DRAWING AND UPDATING OF GRID
    test_update();



    //UPDATE VFX
    UpdateEffects();

    //CLEAR SCREEN
    CP_Graphics_ClearBackground(BLACK);
}
void gameLevelExit(void){

}