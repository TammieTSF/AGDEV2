#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "menustate.h"
#include "highscorestate.h"

CHighScoreState CHighScoreState::theHighScoreState;

void CHighScoreState::Init()
{
	TwoDeeScene = new CSceneManager2D(800, 600);
	TwoDeeScene->Init();
}

void CHighScoreState::Init(const int width, const int height)
{
	TwoDeeScene = new CSceneManager2D(height, width);
	TwoDeeScene->Init();
}

void CHighScoreState::Cleanup()
{
	TwoDeeScene->Exit();
	delete TwoDeeScene;
	TwoDeeScene = NULL;
}

void CHighScoreState::Pause()
{
}

void CHighScoreState::Resume()
{
}

void CHighScoreState::HandleEvents(CGameStateManager* theGSM)
{
}

void CHighScoreState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
}

void CHighScoreState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							   const int button_Left, const int button_Middle, const int button_Right)
{
}

void CHighScoreState::Update(CGameStateManager* theGSM) 
{
}

void CHighScoreState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{	
	if (Application::IsKeyPressed(VK_SPACE))
	{
		theGSM->ChangeState(CMenuState::Instance());
	}
}

void CHighScoreState::Draw(CGameStateManager* theGSM) 
{
	TwoDeeScene->RenderHighscore();
}
