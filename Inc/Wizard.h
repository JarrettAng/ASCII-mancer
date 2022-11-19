#pragma once
typedef struct Wizard {
	CP_Color color;
	float xpos;
	float ypos;
	float size;
	char* words;
}Wizard;

void InitWizard(void);
void UpdateWizard(void);
void DrawWizard(Wizard wiz_body_part);