#include "cprocessing.h"
#include "splashscreen.h"

int main(void)
{
	CP_Engine_SetNextGameState(Splash_screen_init, Splash_screen_update, Splash_screen_exit);
	CP_Engine_Run();
	return 0;
}
