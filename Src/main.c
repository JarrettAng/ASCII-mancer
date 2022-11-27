/*!
@file	  main.c
@author	  Amadeus Chia (amadeusjinhan.chia@digipen.edu)
@author	  Ang Jiawei Jarrett (a.jiaweijarrett@digipen.edu)
@author	  Justine Carlo Villa Ilao (justine.c@digipen.edu)
@author	  Muhammad Farhan Bin Ahmad (b.muhammadfarhan@digipen.edu)
@author	  Tan Jun Rong (t.junrong@digipen.edu)
@date     27/11/2022
@brief    This source file is called when the program first starts and switches the scene to splashscreen

@license  Copyright © 2022 DigiPen, All rights reserved.
________________________________________________________________________________________________________*/

#include "cprocessing.h"
#include "Splashscreen.h"

int main(void)
{
	CP_Engine_SetNextGameState(SplashScreenInit, SplashScreenUpdate, SplashScreenExit);
	CP_Engine_Run();
	return 0;
}
