#ifndef SCENE_MANAGER_2D_H
#define SCENE_MANAGER_2D_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Vector2.h"
#include "Text.h"

class CSceneManager2D : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_LIGHTENABLED,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_TEXT,
		GEO_INTRO,			// Done
		GEO_MENU,			// Done
		GEO_PLAY,			// Done
		GEO_LEVELSELECT,	// Done
		GEO_OPTION,			// Done
		GEO_EXIT,			// Done
		GEO_INSTRUCTION,	// Done
		GEO_OPTION1,		// Done
		GEO_OPTION2,		// Done
		GEO_OPTION3,		// Done
		GEO_OPTION4,		// Done
		GEO_OPTIONSEL,		// Done
		GEO_LEVELS,			// Done
		GEO_LEVEL1,			// Done
		GEO_LEVEL2,			// Done
		GEO_LEVEL3,			// Done
		GEO_SPLASH,			// Done
		GEO_HIGHSCORE,		// Done
		GEO_PAUSE,			// Done
		GEO_RESUME,			// Done
		GEO_PEXIT,			// Done
		GEO_SCORE,
		NUM_GEOMETRY,
	};

public:
	CSceneManager2D();
	CSceneManager2D(const int m_window_width, const int m_window_height);
	~CSceneManager2D();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderBackground();
	void RenderIntro();
	void RenderMenu();
	void RenderInstruction();
	void RenderOption();
	void RenderLevels();
	void RenderSplash();
	void RenderHighscore();
	void RenderPause();
	
	void Highscore();

	
	void Render2DMesh(Mesh *mesh, const bool enableLight, const int size = 1, const int x = 0, const int y = 0, const bool rotate = false, const bool flip = false);

	//Menu States
	bool ShowMenu;		// Show Menu Options
	bool ShowPlay;		// Show Play Option
	bool ShowLevel;		// show Level select Option
	bool ShowHighscore;	// Show Highscore Option
	bool ShowOption;	// Show Options Option 
	bool ShowExit;		// Show Exit Option

	//Option States
	bool Option1;		// Show Option1
	bool Option2;		// Show Option2
	bool Option3;		// Show Option3
	bool Option4;		// Show Option4

	//Option Selection
	bool Sel1;			// Show Option Selection1
	bool Sel2;			// Show Option Selection2

	//Level States
	bool ShowLevels;	// Show All Levels
	bool ShowLevel1;	// Show Level 1
	bool ShowLevel2;	// Show Level 2
	bool ShowLevel3;	// Show Level 3

	//Pause States
	bool PauseGame;		// Pause gameplay
	bool ShowPause;		// Show Pause Options
	bool ShowResume;	// Show Resume Option
	bool ShowExitt;		// Show Exit Option

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	float rotateAngle;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	float fps;

	// Window size
	int m_window_width;
	int m_window_height;

	int highscore[5];


};

#endif