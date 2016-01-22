#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "menustate.h"
#include "InstructionState.h"
#include "OptionState.h"
#include "LevelSelectionState.h"
#include "HighscoreState.h"

CMenuState CMenuState::theMenuState;

void CMenuState::Init()
{
	TwoDeeScene = new CSceneManager2D(800, 600);
	TwoDeeScene->Init();
}

void CMenuState::Init(const int width, const int height)
{
	TwoDeeScene = new CSceneManager2D(height, width);
	TwoDeeScene->Init();
}

void CMenuState::Cleanup()
{
	TwoDeeScene->Exit();
	delete TwoDeeScene;
	TwoDeeScene = NULL;
}

void CMenuState::Pause()
{
}

void CMenuState::Resume()
{
}

void CMenuState::HandleEvents(CGameStateManager* theGSM)
{
}

void CMenuState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
}

void CMenuState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right)
{
}

void CMenuState::Update(CGameStateManager* theGSM) 
{
}

 
void CMenuState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	if (Application::IsKeyPressed(VK_DOWN))
	{
		if (Select < 5)
		{
			Select++;
			Sleep(150);
		}

	}
	if (Application::IsKeyPressed(VK_UP))
	{
		if (Select > 1)
		{
			Select--;
			Sleep(150);
		}
	}

	switch (Select)
	{
	case 1:
		TwoDeeScene->ShowMenu = false;
		TwoDeeScene->ShowPlay = true;
		TwoDeeScene->ShowLevel = false;
		TwoDeeScene->ShowHighscore = false;
		TwoDeeScene->ShowOption = false;
		TwoDeeScene->ShowExit = false;
		break;

	case 2:
		TwoDeeScene->ShowMenu = false;
		TwoDeeScene->ShowPlay = false;
		TwoDeeScene->ShowLevel = true;
		TwoDeeScene->ShowHighscore = false;
		TwoDeeScene->ShowOption = false;
		TwoDeeScene->ShowExit = false;
		break;

	case 3:
		TwoDeeScene->ShowMenu = false;
		TwoDeeScene->ShowPlay = false;
		TwoDeeScene->ShowLevel = false;
		TwoDeeScene->ShowHighscore = true;
		TwoDeeScene->ShowOption = false;
		TwoDeeScene->ShowExit = false;
		break;

	case 4:
		TwoDeeScene->ShowMenu = false;
		TwoDeeScene->ShowPlay = false;
		TwoDeeScene->ShowLevel = false;
		TwoDeeScene->ShowHighscore = false;
		TwoDeeScene->ShowOption = true;
		TwoDeeScene->ShowExit = false;
		break;

	case 5:
		TwoDeeScene->ShowMenu = false;
		TwoDeeScene->ShowPlay = false;
		TwoDeeScene->ShowLevel = false;
		TwoDeeScene->ShowHighscore = false;
		TwoDeeScene->ShowOption = false;
		TwoDeeScene->ShowExit = true;
		break;
	}

	if (Application::IsKeyPressed(VK_RETURN) || Application::IsKeyPressed(VK_SPACE))
	{
		if (Select == 1)
		{
			theGSM->ChangeState(CInstructionState::Instance());
		}
		if (Select == 2)
		{
			theGSM->ChangeState(CLevelSelectionState::Instance());
		}
		if (Select == 3)
		{
			theGSM->ChangeState(CHighScoreState::Instance());
		}
		if (Select == 4)
		{
			theGSM->ChangeState(COptionState::Instance());
		}
		if (Select == 5)
		{
			exit(0);
		}
	}
}

void CMenuState::Draw(CGameStateManager* theGSM) 
{
	TwoDeeScene->RenderMenu();
}
