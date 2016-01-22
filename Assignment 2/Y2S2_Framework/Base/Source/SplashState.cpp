#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "introstate.h"
#include "Splashstate.h"

CSplashState CSplashState::theSplashState;

void CSplashState::Init()
{
	TwoDeeScene = new CSceneManager2D(800, 600);
	TwoDeeScene->Init();
}

void CSplashState::Init(const int width, const int height)
{
	TwoDeeScene = new CSceneManager2D(height, width);
	TwoDeeScene->Init();
}

void CSplashState::Cleanup()
{
	TwoDeeScene->Exit();
	delete TwoDeeScene;
	TwoDeeScene = NULL;
}

void CSplashState::Pause()
{

}

void CSplashState::Resume()
{
}

void CSplashState::HandleEvents(CGameStateManager* theGSM)
{
}

void CSplashState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
}

void CSplashState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right)
{
}

void CSplashState::Update(CGameStateManager* theGSM)
{
}

static float timer = 0.f;
void CSplashState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	timer += m_dElapsedTime;
	
	if (timer > 2)
		theGSM->ChangeState(CIntroState::Instance());
	
}

void CSplashState::Draw(CGameStateManager* theGSM)
{
	TwoDeeScene->RenderSplash();
}
