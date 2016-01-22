#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "menustate.h"
#include "introstate.h"

CIntroState CIntroState::theIntroState;

void CIntroState::Init()
{
	TwoDeeScene = new CSceneManager2D(800, 600);
	TwoDeeScene->Init();
}

void CIntroState::Init(const int width, const int height)
{
	TwoDeeScene = new CSceneManager2D(height, width);
	TwoDeeScene->Init();
}

void CIntroState::Cleanup()
{
	TwoDeeScene->Exit();
	delete TwoDeeScene;
	TwoDeeScene = NULL;
}

void CIntroState::Pause()
{
}

void CIntroState::Resume()
{
}

void CIntroState::HandleEvents(CGameStateManager* theGSM)
{
}

void CIntroState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
}

void CIntroState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							   const int button_Left, const int button_Middle, const int button_Right)
{
}

void CIntroState::Update(CGameStateManager* theGSM) 
{
}

void CIntroState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{	
	if (Application::IsKeyPressed(VK_SPACE))
	{
		theGSM->ChangeState(CMenuState::Instance());
	}
}

void CIntroState::Draw(CGameStateManager* theGSM) 
{
	TwoDeeScene->RenderIntro();
}
