#include <iostream>
using namespace std;

#include "gamestate.h"
#include "GameStateManager.h"
#include "playstate.h"
#include "menustate.h"

CPlayState CPlayState::thePlayState;

void CPlayState::Init()
{
	counter = 0;

	#if TYPE_OF_VIEW == 3
		scene = new CSceneManager(800, 600);	// Use this for 3D gameplay
	#else
		scene = new CSceneManager2D(800, 600);	// Use this for 2D gameplay
	#endif
	scene->Init();
}

void CPlayState::Init(const int width, const int height)
{
	counter = 0;

	#if TYPE_OF_VIEW == 3
		scene = new CSceneManager(width, height);	// Use this for 3D gameplay
	#else
		scene = new CSceneManager2D(width, height);	// Use this for 2D gameplay
	#endif
	scene->Init();
	
}

void CPlayState::Cleanup()
{
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CPlayState::Pause()
{
}

void CPlayState::Resume()
{
}

void CPlayState::HandleEvents(CGameStateManager* theGSM)
{
}

void CPlayState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
	scene->UpdateAvatarStatus( key, status );
}

void CPlayState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right)
{
	if (button_Left == true)
		scene->UpdateWeaponStatus(scene->WA_FIRE);
	if (button_Right == true)
		scene->UpdateWeaponStatus(scene->WA_FIRE_SECONDARY);
}

void CPlayState::Update(CGameStateManager* theGSM)
{
	// Update the scene
	//scene->Update(0.16667);
}

void CPlayState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	// Update the scene
	
	/*if (Application::IsKeyPressed(VK_ESCAPE))
	{
		theGSM->ChangeState(CMenuState::Instance());
	}
	else*/
	scene->Update(m_dElapsedTime);
}

void CPlayState::Draw(CGameStateManager* theGSM)
{
	// Render the scene
	scene->Render();
}
