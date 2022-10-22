#include "cprocessing.h"
#include "splashscreen.h"

int main(void)
{
	CP_Engine_SetNextGameState(SplashScreenInit, SplashScreenUpdate, SplashScreenExit);
	CP_Engine_Run();
	return 0;
}
