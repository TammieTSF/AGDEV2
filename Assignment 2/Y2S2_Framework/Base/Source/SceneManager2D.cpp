#include "SceneManager2D.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include <string>
#include "Strategy_Kill.h"

CSceneManager2D::CSceneManager2D()
	: ShowMenu(true)
	, ShowPlay(false)
	, ShowLevel(false)
	, ShowOption(false)
	, ShowExit(false)
	, Option1(true)
	, Option2(false)
	, Option3(false)
	, Option4(false)
	, Sel1(true)
	, Sel2(false)
	, ShowLevels(true)
	, ShowLevel1(false)
	, ShowLevel2(false)
	, ShowLevel3(false)
	, ShowPause(true)
	, ShowResume(false)
	, ShowExitt(false)
	, PauseGame(false)
{
}

CSceneManager2D::CSceneManager2D(const int m_window_width, const int m_window_height)
{
	this->m_window_width = m_window_width;
	this->m_window_height = m_window_height;
}

CSceneManager2D::~CSceneManager2D()
{
}

void CSceneManager2D::Init()
{
	// Blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Switch on culling
	glEnable(GL_CULL_FACE);

	// Render mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Activate blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate and bind vertex array
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Load the shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Use our shader
	glUseProgram(m_programID);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	// Initialise the camera
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	// Create the meshes
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	// Render States

	// Splash
	meshList[GEO_SPLASH] = MeshBuilder::Generate2DMesh("GEO_SPLASH", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_SPLASH]->textureID = LoadTGA("Image//Splash.tga");

	// Intro
	meshList[GEO_INTRO] = MeshBuilder::Generate2DMesh("GEO_INTRO", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_INTRO]->textureID = LoadTGA("Image//Intro.tga");

	// Menu
	meshList[GEO_MENU] = MeshBuilder::Generate2DMesh("GEO_MENU", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_MENU]->textureID = LoadTGA("Image//Menu.tga");
	meshList[GEO_PLAY] = MeshBuilder::Generate2DMesh("GEO_PLAY", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_PLAY]->textureID = LoadTGA("Image//Play.tga");
	meshList[GEO_LEVELSELECT] = MeshBuilder::Generate2DMesh("GEO_LEVELSELECT", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_LEVELSELECT]->textureID = LoadTGA("Image//LevelSel.tga");
	meshList[GEO_HIGHSCORE] = MeshBuilder::Generate2DMesh("GEO_HIGHSCORE", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_HIGHSCORE]->textureID = LoadTGA("Image//HighScore.tga");
	meshList[GEO_OPTION] = MeshBuilder::Generate2DMesh("GEO_OPTION", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_OPTION]->textureID = LoadTGA("Image//Option.tga");
	meshList[GEO_EXIT] = MeshBuilder::Generate2DMesh("GEO_EXIT", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_EXIT]->textureID = LoadTGA("Image//Exit.tga");

	// Instruction
	meshList[GEO_INSTRUCTION] = MeshBuilder::Generate2DMesh("GEO_INSTRUCTION", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_INSTRUCTION]->textureID = LoadTGA("Image//Instructions.tga");

	// Music on, SFX on
	meshList[GEO_OPTION1] = MeshBuilder::Generate2DMesh("GEO_OPTION1", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_OPTION1]->textureID = LoadTGA("Image//OptionNN.tga");
	// Music on, SFX off
	meshList[GEO_OPTION2] = MeshBuilder::Generate2DMesh("GEO_OPTION2", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_OPTION2]->textureID = LoadTGA("Image//OptionNF.tga");
	// Music off, SFX off
	meshList[GEO_OPTION3] = MeshBuilder::Generate2DMesh("GEO_OPTION3", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_OPTION3]->textureID = LoadTGA("Image//OptionFF.tga");
	// Music off, SFX on
	meshList[GEO_OPTION4] = MeshBuilder::Generate2DMesh("GEO_OPTION4", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_OPTION4]->textureID = LoadTGA("Image//OptionFN.tga");
	// Option Selection
	meshList[GEO_OPTIONSEL] = MeshBuilder::Generate2DMesh("GEO_OPTIONSEL", Color(1, 1, 1), 0, 0, 75, 55);
	meshList[GEO_OPTIONSEL]->textureID = LoadTGA("Image//OptionSel.tga");

	// Render Level States
	meshList[GEO_LEVELS] = MeshBuilder::Generate2DMesh("GEO_OPTIONSEL", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_LEVELS]->textureID = LoadTGA("Image//Levels.tga");
	meshList[GEO_LEVEL1] = MeshBuilder::Generate2DMesh("GEO_OPTIONSEL", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_LEVEL1]->textureID = LoadTGA("Image//Level1.tga");
	meshList[GEO_LEVEL2] = MeshBuilder::Generate2DMesh("GEO_OPTIONSEL", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_LEVEL2]->textureID = LoadTGA("Image//Level2.tga");
	meshList[GEO_LEVEL3] = MeshBuilder::Generate2DMesh("GEO_OPTIONSEL", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_LEVEL3]->textureID = LoadTGA("Image//Level3.tga");

	// Render Pause States
	meshList[GEO_PAUSE] = MeshBuilder::Generate2DMesh("GEO_PAUSE", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_PAUSE]->textureID = LoadTGA("Image//Pause.tga");
	meshList[GEO_RESUME] = MeshBuilder::Generate2DMesh("GEO_RESUME", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_RESUME]->textureID = LoadTGA("Image//Resume.tga");
	meshList[GEO_PEXIT] = MeshBuilder::Generate2DMesh("GEO_PEXIT", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_PEXIT]->textureID = LoadTGA("Image//Exitt.tga");

	// Render Highscore
	meshList[GEO_SCORE] = MeshBuilder::Generate2DMesh("GEO_SCORE", Color(1, 1, 1), 0, 0, 800, 600);
	meshList[GEO_SCORE]->textureID = LoadTGA("Image//Score.tga");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	Highscore();

	rotateAngle = 0;
}

void CSceneManager2D::Highscore()
{
	const int size = 5;
	string value[size];
	ifstream is("highscore.txt");
	
	//std::string::size_type sz;

	int i = 0;

	while (!is.eof() && i < size)
	{
		is >> value[i++];
	}
	is.close();

	for (int j = 0; j < size; ++j)
	{
		istringstream convert(value[j]);
		if (!(convert >> highscore[j]))
			highscore[j] = 0;
		//highscore[j] = std::stoi(value[j], &sz);
	}
}


void CSceneManager2D::Update(double dt)
{
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	rotateAngle -= (float)Application::camera_yaw;// += (float)(10 * dt);

	camera.Update(dt);

	fps = (float)(1.f / dt);

}

/********************************************************************************
 Render text onto the screen
 ********************************************************************************/
void CSceneManager2D::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	//	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation((i*0.5f) + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

/********************************************************************************
 Render 2D Mesh
 ********************************************************************************/
void CSceneManager2D::Render2DMesh(Mesh *mesh, bool enableLight, int size, int x, int y, bool rotate, bool flip)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate((float)x, (float)y, 0);
	modelStack.Scale((float)size, (float)size, (float)size);
	if (rotate)
		modelStack.Rotate(rotateAngle, 0, 0, 1);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void CSceneManager2D::RenderIntro()
{
	// Render the Intro Screen
	Render2DMesh(meshList[GEO_INTRO], false, 1);
}

void CSceneManager2D::RenderMenu()
{
	// Render the Menu Screen
	if (ShowMenu) //If ShowMenu is true, render out menu page
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_MENU], false);
		modelStack.PopMatrix();
	}
	else if (ShowPlay) //If ShowPlay is true, render out menu page with play highlighted
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_PLAY], false);
		modelStack.PopMatrix();
	}
	else if (ShowLevel)  //If ShowLevel is true, render out menu page with level select highlighted
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_LEVELSELECT], false);
		modelStack.PopMatrix();
	}
	else if (ShowHighscore) //If ShowHighscore is true, render out menu page with highscore highlighted
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_HIGHSCORE], false);
		modelStack.PopMatrix();
	}
	else if (ShowOption) //If ShowOption is true, render out menu page with option highlighted
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_OPTION], false);
		modelStack.PopMatrix();
	}
	else if (ShowExit) //If ShowExit is true, render out menu page with exit highlighted
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_EXIT], false);
		modelStack.PopMatrix();
	}
}

void CSceneManager2D::RenderInstruction()
{
	modelStack.PushMatrix();
	Render2DMesh(meshList[GEO_INSTRUCTION], false);
	modelStack.PopMatrix();
}

void CSceneManager2D::RenderSplash()
{
	modelStack.PushMatrix();
	Render2DMesh(meshList[GEO_SPLASH], false);
	modelStack.PopMatrix();
}

void CSceneManager2D::RenderOption()
{
	if (Option1)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_OPTION1], false);
		modelStack.PopMatrix();
	}
	else if (Option2)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_OPTION2], false);
		modelStack.PopMatrix();
	}
	else if (Option3)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_OPTION3], false);
		modelStack.PopMatrix();
	}
	else if (Option4)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_OPTION4], false);
		modelStack.PopMatrix();
	}

	if (Sel1)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_OPTIONSEL], false, 1.5, 50, 525);
		modelStack.PopMatrix();
	}
	else if (Sel2)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_OPTIONSEL], false, 1.5, 90, 330);
		modelStack.PopMatrix();
	}
}

void CSceneManager2D::RenderLevels()
{
	if (ShowLevels)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_LEVELS], false);
		modelStack.PopMatrix();
	}
	else if (ShowLevel1)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_LEVEL1], false);
		modelStack.PopMatrix();
	}
	else if (ShowLevel2)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_LEVEL2], false);
		modelStack.PopMatrix();
	}
	else if (ShowLevel3)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_LEVEL3], false);
		modelStack.PopMatrix();
	}
}

void CSceneManager2D::RenderHighscore()
{
	modelStack.PushMatrix();
	Render2DMesh(meshList[GEO_SCORE], false);

	std::ostringstream ss;
	const int size = 5;
	for (int i = 0; i < size; ++i)
	{
		ss.str(std::string());
		ss << i + 1 << ". " << highscore[i];
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 1), 70, 300, 350 - (i * 80));
	}
	modelStack.PopMatrix();
}

void CSceneManager2D::RenderPause()
{
	// Render the Menu Screen
	if (ShowPause)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_PAUSE], false);
		modelStack.PopMatrix();
	}
	else if (ShowResume)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_RESUME], false);
		modelStack.PopMatrix();
	}
	else if (ShowExitt)
	{
		modelStack.PushMatrix();
		Render2DMesh(meshList[GEO_PEXIT], false);
		modelStack.PopMatrix();
	}
}

/********************************************************************************
 Render this scene
 ********************************************************************************/
void CSceneManager2D::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
}

/********************************************************************************
 Exit this scene
 ********************************************************************************/
void CSceneManager2D::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}