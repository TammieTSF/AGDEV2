#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "menustate.h"
#include "LevelSelectionState.h"
#include "PlayState.h"

CLevelSelectionState CLevelSelectionState::theLevelSelectionState;

void CLevelSelectionState::Init()
{
#if GSM_DEBUG_MODE
	cout << "CLevelSelectionState::Init\n" << endl;
#endif
	TwoDeeScene = new CSceneManager2D(800, 600);
	TwoDeeScene->Init();
}

void CLevelSelectionState::Init(const int width, const int height)
{
#if GSM_DEBUG_MODE
	cout << "CLevelSelectionState::Init\n" << endl;
#endif

	TwoDeeScene = new CSceneManager2D(height, width);
	TwoDeeScene->Init();
}

void CLevelSelectionState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CLevelSelectionState::Cleanup\n" << endl;
#endif

	TwoDeeScene->Exit();
	delete TwoDeeScene;
	TwoDeeScene = NULL;
}

void CLevelSelectionState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CLevelSelectionState::Pause\n" << endl;
#endif
}

void CLevelSelectionState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CLevelSelectionState::Resume\n" << endl;
#endif
}

void CLevelSelectionState::HandleEvents(CGameStateManager* theGSM)
{
}

void CLevelSelectionState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
}

void CLevelSelectionState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right)
{
}

void CLevelSelectionState::Update(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CLevelSelectionState::Update\n" << endl;
#endif
}

void CLevelSelectionState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	if (Application::IsKeyPressed(VK_DOWN))
	{
		if (Select < 3)
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
		TwoDeeScene->ShowLevels = false;
		TwoDeeScene->ShowLevel1 = true;
		TwoDeeScene->ShowLevel2 = false;
		TwoDeeScene->ShowLevel3 = false;
		break;

	case 2:
		TwoDeeScene->ShowLevels = false;
		TwoDeeScene->ShowLevel1 = false;
		TwoDeeScene->ShowLevel2 = true;
		TwoDeeScene->ShowLevel3 = false;
		break;

	case 3:
		TwoDeeScene->ShowLevels = false;
		TwoDeeScene->ShowLevel1 = false;
		TwoDeeScene->ShowLevel2 = false;
		TwoDeeScene->ShowLevel3 = true;
		break;
	}

	if (Application::IsKeyPressed(VK_RETURN) || Application::IsKeyPressed(VK_SPACE))
	{
		if (Select == 1)
		{
			//theGSM->ChangeState(CInstructionState::Instance());
		}
		if (Select == 2)
		{
			//theGSM->ChangeState(CLevelSelectionState::Instance());
		}
		if (Select == 3)
		{
			///theGSM->ChangeState(COptionState::Instance());
		}
	}

	if (Application::IsKeyPressed(VK_LEFT))
	{
		theGSM->ChangeState(CMenuState::Instance());
	}
}

void CLevelSelectionState::Draw(CGameStateManager* theGSM)
{
	TwoDeeScene->RenderLevels();
}
