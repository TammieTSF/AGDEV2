#pragma once
#include "gamestate.h"
#include "application.h"

class COptionState : public CGameState
{
public:
	void Init();
	void Init(const int width, const int height);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(CGameStateManager* theGSM);
	void HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status = true);
	void HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
		const int button_Left, const int button_Middle, const int button_Right);
	void Update(CGameStateManager* theGSM);
	void Update(CGameStateManager* theGSM, const double m_dElapsedTime);
	void Draw(CGameStateManager* theGSM);
	
	// Music
	void OffOptionMusic(bool OptionMusic);

	//SFX
	void OffOptionSFX(bool OptionSFX);
	
	static COptionState* Instance() {
		return &theOptionState;
	}

protected:
	COptionState() { }

private:
	static COptionState theOptionState;
	CSceneManager2D* TwoDeeScene;
	int SelectX = 1;
	int SelectY = 1;

	bool OptionMusic;
	bool OptionSFX;
};

