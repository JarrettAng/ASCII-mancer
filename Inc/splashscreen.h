/*!
@file	  splashscreen.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@course	  CSD1401 - Software Engineering Project 1
@section  Section B
@tutorial Splashscreen
@date     14/09/2022
@brief    This header file contains 3 functions for the SetNextGameState and 4 function
		  declarations as I arranged my functions in splashscreen.c by order of execution
*//*______________________________________________________________________*/

#pragma once
void splash_screen_init(void);
void splash_screen_update(void);
void splash_screen_exit(void);

void process_input(void);
void update(void);
void render(void);
float math_pow(float, int);
