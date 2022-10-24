#include <cprocessing.h>
#include "EnemyStats.h"


//Call this to set enemy stats before spawning them
void SetEnemy()
{
	switch (Type)
	{
	case 1://Normal
	{
		Enemy[enemy_Index].CreditCost = 1;
		Enemy[enemy_Index].Health = 1;
		Enemy[enemy_Index].MovementSpeed = 1;
		break;
	}
	case 2://Normal
	{
		Enemy[enemy_Index].CreditCost = 2;
		Enemy[enemy_Index].Health = 2;
		Enemy[enemy_Index].MovementSpeed = 1;
		break;
	}
	case 3://Normal
	{
		Enemy[enemy_Index].CreditCost = 3;
		Enemy[enemy_Index].Health = 3;
		Enemy[enemy_Index].MovementSpeed = 1;
		break;
	}
	enemy_Index++;
	}
}