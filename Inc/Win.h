#pragma once
#include "cprocessing.h"

#define WAVES_TO_WIN 5

void InitializeButtonsWinScreen(void);
void WinTextInit(void);
void CheckWinCondition(void);
void WinAnimation(void);

void WinInit(void);
void WinUpdate(void);
void WinExit(void);
