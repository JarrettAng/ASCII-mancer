#include "Credits.h"
#include "ColorTable.h"
#include "UIManager.h"
#include "MainMenu.h"

#define NUM_OF_CREDITS 7

/*
Developed by:
Instructors:
Created at:
President:
Executives:
Special thanks:
Copyright:
*/
Text creditHeaders[NUM_OF_CREDITS];
Text creditNames[NUM_OF_CREDITS];

int scrollSpeed = 200;

Button backBtn;

#pragma region FORWARD_DECLARATIONS
void InitCreditsTexts(void);
void InitCreditsButtons(void);
void RenderCreditsCutoff(void);
void ScrollCreditTexts(void);
static void LoadMainMenu(void);
#pragma endregion

void CreditsInit(void){
	InitCreditsTexts();
	InitCreditsButtons();
}

void CreditsUpdate(void){
	CP_Graphics_ClearBackground(MENU_BLACK);
	RenderTexts();
	RenderTextBoxes();
	RenderCreditsCutoff();
	RenderButtons();

	HandleButtonClick();
	ScrollCreditTexts();
}

void CreditsExit(void){
	ClearInteractCache();
	FreeUI();
}

void InitCreditsTexts(){
	/*==============================Text Settings========================*/

	Text* p_headerText = creditHeaders;
	Text* p_namesText = creditNames;

	// Offset of names from its header.
	float nameOffSet = GetWindowHeight() / 10;

	TextData headerTextData = {
		.color = MENU_RED,
		.font = CP_Font_Load("Assets/PressStart2P-Regular.ttf"),
		.hAlign = CP_TEXT_ALIGN_H_CENTER,
		.vAlign = CP_TEXT_ALIGN_V_MIDDLE,
		.textSize = 80 * GetHeightScale()
	};

	TextData namesTextData = {
	.color = MENU_WHITE,
	.font = CP_Font_Load("Assets/PressStart2P-Regular.ttf"),
	.hAlign = CP_TEXT_ALIGN_H_CENTER,
	.vAlign = CP_TEXT_ALIGN_V_MIDDLE,
	.textSize = 50 * GetHeightScale()
	};

	Rect headerRect = {
		.x = GetWindowWidth() / 2,
		.y = GetWindowHeight(),
		.width = GetWindowWidth()
	};

	Rect namesRect = {
		.x = 0,
		.y = GetWindowHeight(),
		.width = GetWindowWidth()
	};

	TextData memberHeader = headerTextData;
	memberHeader.text = "DEVELOPED BY";
	InitializeText(p_headerText++, headerRect, memberHeader);

	TextData memberNames = namesTextData;
	memberNames.text =
		"Amadeus Jinhan Chia\n\n"
		"Ang Jiawei Jarrett\n\n"
		"Justine Carlo Villa Ilao\n\n"
		"Muhammad Farhan Bin Ahmad\n\n"
		"Tan Jun Rong";
	namesRect.y = headerRect.y + nameOffSet;
	InitializeTextBox(p_namesText++, namesRect, memberNames);

	TextData instructorHeader = headerTextData;
	instructorHeader.text = "INSTRUCTORS";
	headerRect.y += namesTextData.textSize * 13 + headerTextData.textSize + nameOffSet;
	InitializeText(p_headerText++, headerRect, instructorHeader);

	TextData instructorNames = namesTextData;
	instructorNames.text =
		"Cheng Ding Xiang\n\n"
		"Gerald Wong";
	namesRect.y = headerRect.y + nameOffSet;
	InitializeTextBox(p_namesText++, namesRect, instructorNames);

	TextData createdAtHeader = headerTextData;
	createdAtHeader.text = "CREATED AT";
	headerRect.y += namesTextData.textSize * 6 + headerTextData.textSize + nameOffSet;
	InitializeText(p_headerText++, headerRect, createdAtHeader);

	TextData createdAtNames = namesTextData;
	createdAtNames.text =
		"DigiPen Institute of Technology Singapore";
	namesRect.y = headerRect.y + nameOffSet;
	InitializeTextBox(p_namesText++, namesRect, createdAtNames);

	TextData presidentHeader = headerTextData;
	presidentHeader.text = "PRESIDENT";
	headerRect.y += namesTextData.textSize * 4 + headerTextData.textSize + nameOffSet;
	InitializeText(p_headerText++, headerRect, presidentHeader);

	TextData presidentName = namesTextData;
	presidentName.text = "Claude Comair";
	namesRect.y = headerRect.y + nameOffSet;
	InitializeTextBox(p_namesText++, namesRect, presidentName);

	TextData executiveHeader = headerTextData;
	executiveHeader.text = "EXECUTIVES";
	headerRect.y += namesTextData.textSize * 3 + headerTextData.textSize + nameOffSet;
	InitializeText(p_headerText++, headerRect, executiveHeader);

	TextData executiveNames = namesTextData;
	executiveNames.text =
		"Jason CHU\n\n"
		"Christopher COMAIR\n\n"
		"Michael GATS\n\n"
		"Michele COMAIR\n\n"
		"Raymond YAN\n\n"
		"Samir ABOU SAMRA\n\n"
		"Prasanna GHALI\n\n"
		"John BAUER\n\n"
		"Dr. Erik MOHRMANN\n\n"
		"Melvin GONSALVEZ\n\n"
		"Angela KUGLER\n\n"
		"Dr. Charles DUBA\n\n"
		"Ben ELLINGER\n\n"
		"Johnny DEEK";
	namesRect.y = headerRect.y + nameOffSet;
	InitializeTextBox(p_namesText++, namesRect, executiveNames);

	TextData specialThanksHeader = headerTextData;
	specialThanksHeader.text = "SPECIAL THANKS";
	headerRect.y += namesTextData.textSize * 30 + headerTextData.textSize + nameOffSet;
	InitializeText(p_headerText++, headerRect, specialThanksHeader);

	TextData specialThanksNames = namesTextData;
	specialThanksNames.text =
		"Ligma Johnson\n\n"
		"Ben Dover\n\n"
		"Xi Jinping\n\n"
		"Mike Sugandese\n\n"
		"Rick Astley\n\n"
		"Joe\n\n"
		"Winnie the Pooh\n\n"
		"\n\n"
		;

	namesRect.y = headerRect.y + nameOffSet;
	InitializeTextBox(p_namesText++, namesRect, specialThanksNames);

	TextData copyRightHeader = headerTextData;
	copyRightHeader.text = "";
	headerRect.y += namesTextData.textSize * 12 + headerTextData.textSize + nameOffSet;
	InitializeText(p_headerText, headerRect, copyRightHeader);

	TextData copyRightNames = namesTextData;
	copyRightNames.text =
		"All content copyright 2022 Digipen Institute of Technology Singapore.\n\n"
		"All Rights Reserved";
	namesRect.y = headerRect.y + nameOffSet;
	copyRightNames.textSize = 20;
	InitializeTextBox(p_namesText, namesRect, copyRightNames);
}

void InitCreditsButtons(){
	/*========================Back Button===========================*/
	GraphicData backBtnGraphicsData = {
		.color = TRANSPERANT,
		.strokeColor = TRANSPERANT,
		.strokeWeight = 0,
		.imagePosMode = CP_POSITION_CENTER
	};

	TextData backBtnTextData = {
		.color = MENU_WHITE,
		.font = CP_Font_Load("Assets/PressStart2P-Regular.ttf"),
		.textSize = 70 * GetHeightScale(),
		.hAlign = CP_TEXT_ALIGN_H_CENTER,
		.vAlign = CP_TEXT_ALIGN_V_MIDDLE,
		.text = "BACK"
	};

	Rect backBtnRect = {
		.x = GetWindowWidth() / 2,
		.y = GetWindowHeight() - GetWindowHeight() / 8,
		.heigth = backBtnTextData.textSize,
		.width = 400 * GetWidthScale(),
	};
	InitializeButton(&backBtn, backBtnRect, backBtnGraphicsData, backBtnTextData, LoadMainMenu);
	/*==============================================================*/
}

void RenderCreditsCutoff(){
	CP_Settings_Fill(MENU_BLACK);
	// Top cutoff
	CP_Graphics_DrawRect(GetWindowWidth() / 2, 0, GetWindowWidth(), GetWindowHeight() / 6);
	// Bottom cutoff
	CP_Graphics_DrawRect(GetWindowWidth() / 2, GetWindowHeight(), GetWindowWidth(), GetWindowHeight() / 3);
}

void ScrollCreditTexts(){
	for (short i = 0; i < NUM_OF_CREDITS; ++i){
		creditHeaders[i].transform.y -= scrollSpeed * CP_System_GetDt();
		creditNames[i].transform.y -= scrollSpeed * CP_System_GetDt();
	}

	if (creditNames[NUM_OF_CREDITS - 1].transform.y <= 0){
		for (short i = 0; i < NUM_OF_CREDITS; ++i){
			creditHeaders[i].transform.y = creditHeaders[i].transform.cachedPos.y;
			creditNames[i].transform.y = creditNames[i].transform.cachedPos.y;
		}
	}
}

void LoadMainMenu(){
	CP_Engine_SetNextGameState(MainMenuInit, MainMenuUpdate, MainMenuExit);
}

