#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "menustate.h"
#include "OptionState.h"

COptionState COptionState::theOptionState;

void COptionState::Init()
{
#if GSM_DEBUG_MODE
	cout << "COptionState::Init\n" << endl;
#endif

	TwoDeeScene = new CSceneManager2D(800, 600);
	TwoDeeScene->Init();
}

void COptionState::Init(const int width, const int height)
{
#if GSM_DEBUG_MODE
	cout << "COptionState::Init\n" << endl;
#endif

	TwoDeeScene = new CSceneManager2D(height, width);
	TwoDeeScene->Init();
}

void COptionState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "COptionState::Cleanup\n" << endl;
#endif

	TwoDeeScene->Exit();
	delete TwoDeeScene;
	TwoDeeScene = NULL;
}

void COptionState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "COptionState::Pause\n" << endl;
#endif
}

void COptionState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "COptionState::Resume\n" << endl;
#endif
}

void COptionState::HandleEvents(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE

#endif
}

void COptionState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
#if GSM_DEBUG_MODE

#endif
}

void COptionState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right)
{
#if GSM_DEBUG_MODE

#endif
}

void COptionState::Update(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "COptionState::Update\n" << endl;
#endif
}

// Music
void COptionState::OffOptionMusic(bool OptionMusic)
{
	this->OptionMusic = OptionMusic;
}

//SFX
void COptionState::OffOptionSFX(bool OptionSFX)
{
	this->OptionSFX = OptionSFX;
}

void COptionState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
#if GSM_DEBUG_MODE
#endif

	if (Application::IsKeyPressed(VK_DOWN))
	{
		if (SelectY < 2)
		{
			SelectY++;
			Sleep(150);
		}
	}
	if (Application::IsKeyPressed(VK_UP))
	{
		if (SelectY > 1)
		{
			SelectY--;
			Sleep(150);
		}
	}
	switch (SelectY)
	{
	case 1:
		TwoDeeScene->Sel1 = true;
		TwoDeeScene->Sel2 = false;
		break;
	case 2:
		TwoDeeScene->Sel1 = false;
		TwoDeeScene->Sel2 = true;
		break;
	}

	if (Application::IsKeyPressed(VK_RIGHT))
	{
		if (SelectY == 1)
		{
			OffOptionMusic(true);
			if (OptionMusic == true)
			{
				if (TwoDeeScene->Option1) // Both On
				{
					TwoDeeScene->Option1 = false;
					TwoDeeScene->Option2 = false;
					TwoDeeScene->Option3 = false;
					TwoDeeScene->Option4 = true; // Music Off, SFX On
				}
				else if (TwoDeeScene->Option2) // Music On, SFX Off
				{
					TwoDeeScene->Option1 = false;
					TwoDeeScene->Option2 = false;
					TwoDeeScene->Option3 = true; // Both Off
					TwoDeeScene->Option4 = false;
				}
			}
		}
		if (SelectY == 2) //SFX
		{
			OffOptionSFX(true);
			if (OptionSFX == true)
			{
				if (TwoDeeScene->Option1) // Both On
				{
					TwoDeeScene->Option1 = false;
					TwoDeeScene->Option2 = true; // Music On, SFX Off
					TwoDeeScene->Option3 = false;
					TwoDeeScene->Option4 = false;
				}
				else if (TwoDeeScene->Option4) // Music Off, SFX On
				{
					TwoDeeScene->Option1 = false;
					TwoDeeScene->Option2 = false;
					TwoDeeScene->Option3 = true; // Both Off
					TwoDeeScene->Option4 = false;
				}
			}
		}
	}
	if (Application::IsKeyPressed(VK_LEFT))
	{
		if (SelectY == 1)
		{
			OffOptionMusic(false);
			if (OptionMusic == false)
			{
				if (TwoDeeScene->Option4) //Music Off, SFX On
				{
					TwoDeeScene->Option1 = true; // Both On
					TwoDeeScene->Option2 = false; // Music On, SFX Off
					TwoDeeScene->Option3 = false; // Both Off
					TwoDeeScene->Option4 = false; // Music Off, SFX On
				}
				else if (TwoDeeScene->Option3) // Music Off, SFX Off
				{
					TwoDeeScene->Option1 = false;
					TwoDeeScene->Option2 = true; // Music On, SFX Off
					TwoDeeScene->Option3 = false;
					TwoDeeScene->Option4 = false;
				}
			}
		}

		if (SelectY == 2) //SFX
		{
			OffOptionSFX(false);
			if (OptionSFX == false)
			{
				if (TwoDeeScene->Option2) // Music On, SFX Off
				{
					TwoDeeScene->Option1 = true; // Both On
					TwoDeeScene->Option2 = false;
					TwoDeeScene->Option3 = false;
					TwoDeeScene->Option4 = false;
				}
				else if (TwoDeeScene->Option3) // Both Off
				{
					TwoDeeScene->Option1 = false;
					TwoDeeScene->Option2 = false;
					TwoDeeScene->Option3 = false;
					TwoDeeScene->Option4 = true;
				}
			}
		}
	}

	if (Application::IsKeyPressed(VK_RETURN))
	{
		theGSM->ChangeState(CMenuState::Instance());
	}
}

void COptionState::Draw(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE

#endif

	TwoDeeScene->RenderOption();
}
