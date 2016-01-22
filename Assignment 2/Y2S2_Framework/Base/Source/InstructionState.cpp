#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "InstructionState.h"
#include "playstate.h"

CInstructionState CInstructionState::theInstructionState;

void CInstructionState::Init()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;
#endif

	TwoDeeScene = new CSceneManager2D(800, 600);
	TwoDeeScene->Init();
}

void CInstructionState::Init(const int width, const int height)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Init\n" << endl;
#endif

	TwoDeeScene = new CSceneManager2D(height, width);
	TwoDeeScene->Init();
}

void CInstructionState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Cleanup\n" << endl;
#endif

	TwoDeeScene->Exit();
	delete TwoDeeScene;
	TwoDeeScene = NULL;
}

void CInstructionState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Pause\n" << endl;
#endif
}

void CInstructionState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Resume\n" << endl;
#endif
}

void CInstructionState::HandleEvents(CGameStateManager* theGSM)
{
}

void CInstructionState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
}

void CInstructionState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
	const int button_Left, const int button_Middle, const int button_Right)
{
}

void CInstructionState::Update(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CInstructionState::Update\n" << endl;
#endif
}

void CInstructionState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	if (Application::IsKeyPressed(VK_SPACE))
	{
		theGSM->ChangeState(CPlayState::Instance());
	}
}

void CInstructionState::Draw(CGameStateManager* theGSM)
{
	TwoDeeScene->RenderInstruction();
}
