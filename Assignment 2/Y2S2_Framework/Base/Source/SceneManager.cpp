
#include "SceneManager.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

CSceneManager::CSceneManager(void)
	: m_cMinimap(NULL)
	, m_window_width(800)
	, m_window_height(600)
	, m_cSceneGraph(NULL)
	, m_cSpatialPartition(NULL)
	, m_cProjectileManager(NULL)
{
}

CSceneManager::CSceneManager(const int m_window_width, const int m_window_height)
	: m_cMinimap(NULL)
	, m_cAvatar(NULL)
	, m_cSceneGraph(NULL)
	, m_cSpatialPartition(NULL)
	, m_cProjectileManager(NULL)
{
	this->m_window_width = m_window_width;
	this->m_window_height = m_window_height;
}

CSceneManager::~CSceneManager(void)
{
	if (m_cProjectileManager)
	{
		delete m_cProjectileManager;
		m_cProjectileManager = NULL;
	}
	if (m_cSpatialPartition)
	{
		delete m_cSpatialPartition;
		m_cSpatialPartition = NULL;
	}

	if (m_cSceneGraph)
	{
		delete m_cSceneGraph;
		m_cSceneGraph = NULL;
	}

	if (m_cMinimap)
	{
		delete m_cMinimap;
		m_cMinimap = NULL;
	}
}

void CSceneManager::Init()
{
	// Blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(1, 1, 0);
	lights[1].color.Set(1, 1, 0.5f);
	lights[1].power = 0.4f;
	//lights[1].kC = 1.f;
	//lights[1].kL = 0.01f;
	//lights[1].kQ = 0.001f;
	//lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	//lights[1].cosInner = cos(Math::DegreeToRadian(30));
	//lights[1].exponent = 3.f;
	//lights[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_RAY] = MeshBuilder::GenerateRay("ray", 10.0f);
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_OBJECT] = MeshBuilder::GenerateOBJ("OBJ1", "OBJ//chair.obj");//MeshBuilder::GenerateCube("cube", 1);
	meshList[GEO_OBJECT]->textureID = LoadTGA("Image//chair.tga");
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 0, 0), 18, 36, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 1.f);
	//meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", 1, 1, 1);
	//meshList[GEO_TORUS] = MeshBuilder::GenerateCylinder("torus", 36, 36, 5, 1);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	meshList[GEO_CONE]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	meshList[GEO_CONE]->material.kSpecular.Set(0.f, 0.f, 0.f);

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("LEFT", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("RIGHT", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("TOP", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("BOTTOM", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("FRONT", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("BACK", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");

	// Load the ground mesh and texture
	meshList[GEO_GRASS_DARKGREEN] = MeshBuilder::GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_DARKGREEN]->textureID = LoadTGA("Image//grass_darkgreen.tga");
	meshList[GEO_GRASS_LIGHTGREEN] = MeshBuilder::GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_LIGHTGREEN]->textureID = LoadTGA("Image//grass_lightgreen.tga");

	meshList[GEO_CHAR] = MeshBuilder::GenerateOBJ("Avatar", "OBJ//cutie.obj");
	meshList[GEO_CHAR]->textureID = LoadTGA("Image//cute.tga");

	// Load the texture for minimap
	m_cMinimap = new CMinimap();
	m_cMinimap->SetBackground(MeshBuilder::GenerateMinimap("MINIMAP", Color(1, 1, 1), 1.f));
	m_cMinimap->GetBackground()->textureID = LoadTGA("Image//grass_darkgreen.tga");
	m_cMinimap->SetBorder(MeshBuilder::GenerateMinimapBorder("MINIMAPBORDER", Color(1, 1, 0), 1.f));
	m_cMinimap->SetAvatar(MeshBuilder::GenerateMinimapAvatar("MINIMAPAVATAR", Color(1, 1, 0), 1.f));

	// Initialise and load a model into it
	m_cAvatar = new CPlayInfo3PV();
	m_cAvatarNode = new CSceneNode();
	CModel* AvatarModel = new CModel();
	AvatarModel->Init();
	cout << m_cAvatarNode->SetNode(new CTransform(1, 0, 0), AvatarModel) << endl;
	m_cAvatar->SetModel(meshList[GEO_CHAR]);
	m_cAvatar->TakeNode(m_cAvatarNode);

	// Camera Position
	camera.Init(Vector3(m_cAvatar->GetPosition().x + camera.offsetx, m_cAvatar->GetPosition().y + camera.offsety, m_cAvatar->GetPosition().z + camera.offsetz), m_cAvatar->GetPosition(), Vector3(0, 1, 0));

	//Create a scenegraph
	m_cSceneGraph = new CSceneNode();
	CModel* newModel = new CModel();
	newModel->Init();
	cout << m_cSceneGraph->SetNode(new CTransform(20, -30, 20), newModel) << endl;

	newModel = new CModel();
	newModel->Init();
	cout << m_cSceneGraph->AddChild(new CTransform(0, 1, 0), newModel) << endl;

	m_cSpatialPartition = new CSpatialPartition();
	m_cSpatialPartition->Init(100, 100, 5, 5);
	for (int i = 0; i < m_cSpatialPartition->GetxNumOfGrid(); i++)
	{
		for (int j = 0; j < m_cSpatialPartition->GetyNumOfGrid(); j++)
		{
			m_cSpatialPartition->SetGridMesh(i, j, NULL);
		}
	}

	// Add the pointers to the scene graph to the spatial partition
	m_cSpatialPartition->AddObject(m_cSceneGraph);
	m_cSpatialPartition->AddObject(m_cAvatarNode);

	// Create the projectile manager
	m_cProjectileManager = new CProjectileManager();

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	rotateAngle = 0;
	score = 0;

	bLightEnabled = true;
}

Mesh* CSceneManager::IfGridEmpty(const int xIndex, const int yIndex)
{
	if (m_cSpatialPartition->theGrid[xIndex * m_cSpatialPartition->yNumOfGrid + yIndex].ListOfObjects.size() == 0)
	{
		Mesh* mesh = MeshBuilder::GenerateQuad("NOT FOUND", Color(0, 1, 0), 1.f);
		modelStack.Scale(m_cSpatialPartition->xGridSize, m_cSpatialPartition->yGridSize, 1);
		RenderMesh(mesh, false);
	}
	else
		return NULL;
}
Mesh* CSceneManager::IfGridNotEmpty(const int xIndex, const int yIndex)
{
	if (m_cSpatialPartition->theGrid[xIndex * m_cSpatialPartition->yNumOfGrid + yIndex].ListOfObjects.size() > 0)
	{
		Mesh* mesh = MeshBuilder::GenerateQuad("FOUND", Color(0, 0, 1), 1.f);
		modelStack.Scale(m_cSpatialPartition->xGridSize, m_cSpatialPartition->yGridSize, 1);
		RenderMesh(mesh, false);
	}
	else
		return NULL;
}

static float limitA = 0.f;
static float limitB = 0.2f;

static float limitC = 0.f;
static float limitD = 0.2f;

void CSceneManager::Update(double dt)
{
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (Application::IsKeyPressed('5'))
	{
		lights[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if (Application::IsKeyPressed('6'))
	{
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if (Application::IsKeyPressed('7'))
	{
		lights[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if (Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	else if (Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}

	if (Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(10.f * dt);
	if (Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(10.f * dt);
	if (Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(10.f * dt);
	if (Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(10.f * dt);
	if (Application::IsKeyPressed('O'))
		lights[0].position.y -= (float)(10.f * dt);
	if (Application::IsKeyPressed('P'))
		lights[0].position.y += (float)(10.f * dt);

	rotateAngle -= Application::camera_yaw;// += (float)(10 * dt);

	static float time = 0.0f;
	time += 1.0f / 20.0f;

	limitA += dt;
	if (limitA == limitB)
	{
		limitA = limitB;
	}

	limitC += dt;
	if (limitC == limitD)
	{
		limitC = limitD;
	}

	m_cAvatar->Update(dt, camera);
	camera.target = m_cAvatar->GetPosition();
	camera.Update(dt);

	m_cSpatialPartition->Update(camera.position,
		(camera.target - camera.position).Normalize()); // clear grid
	
	//m_cSceneGraph->ApplyTranslate(0.1, 0, 0);
	m_cSceneGraph->ApplyScale(10, 10, 10);
	m_cSceneGraph->GetNode(11)->ApplyRotate(5, 0, 1, 0);
	//m_cSceneGraph->GetNode(11)->ApplyTranslate(0, cos(time), 0);

	Vector3 AvatarNodePos = m_cAvatarNode->getTransform()->getTranslation();
	Vector3 PosDifference = m_cAvatar->GetPosition() - AvatarNodePos;

	m_cAvatarNode->ApplyTranslate(PosDifference.x, PosDifference.y, PosDifference.z);


	m_cSpatialPartition->AddObject(m_cSceneGraph); // add objects
	m_cSpatialPartition->AddObject(m_cAvatarNode); // add avatarnode

	// Update the Projectile Manager
	m_cProjectileManager->Update(dt);

	// Check for collisions for Projectiles
	// Render the projectiles
	Vector3 ProjectilePosition;
	Vector3 ProjectilePosition_End;
	for (int i = 0; i < m_cProjectileManager->GetMaxNumberOfProjectiles(); i++)
	{
		if (m_cProjectileManager->IsActive(i))
		{
			ProjectilePosition = m_cProjectileManager->theListOfProjectiles[i]->GetPosition();
			if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PROJ_TYPE_DISCRETE)
			{
				// Destroy the projectiles after collision
				if (m_cSpatialPartition->CheckForCollision(ProjectilePosition) == true)
				{
					m_cProjectileManager->RemoveProjectile(i);
					score += 10;
				}
					
			}
			else if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PROJ_TYPE_RAY)
			{
				ProjectilePosition_End = ProjectilePosition + m_cProjectileManager->theListOfProjectiles[i]->GetDirection()
					* m_cProjectileManager->theListOfProjectiles[i]->GetLength();

				// Destroy the ray projectiles after collision
				if (m_cSpatialPartition->CheckForCollision(ProjectilePosition, ProjectilePosition_End) == true)
				{
					m_cProjectileManager->RemoveProjectile(i);
					score += 10;
				}
			}
		}
	}

	fps = (float)(1.f / dt);
}

/********************************************************************************
Update Camera position
********************************************************************************/
void CSceneManager::UpdateCameraStatus(const unsigned char key, const bool status)
{
	camera.UpdateStatus(key, status);
}

/********************************************************************************
Update Avatar position
********************************************************************************/
void CSceneManager::UpdateAvatarStatus(const unsigned char key, const bool status)
{
	m_cAvatar->UpdateMovement(key, status);
}

/********************************************************************************
Update Weapon status
********************************************************************************/
void CSceneManager::UpdateWeaponStatus(const unsigned char key)
{
	if (key == WA_FIRE)
	{
		if (limitA > limitB)
		{
			// Add a ray object which starts at the camera position and moves in the camera's direction
			m_cProjectileManager->AddProjectile(m_cAvatar->GetPosition() + m_cAvatar->GetDirection() - Vector3(0,20,-5), 
				(camera.target - camera.position).Normalize(), 50.f);

			limitA = 0;
		} 
	}
	else if (key == WA_FIRE_SECONDARY)
	{
		if (limitC > limitD)
		{
			// Add a bullet object which starts at the camera position and moves in the camera's direction
			m_cProjectileManager->AddRayProjectile(m_cAvatar->GetPosition() + m_cAvatar->GetDirection() - Vector3(0, 20, -5), 
				(camera.target - camera.position).Normalize(), 50.f);


			limitC = 0;
		}
	}
}

/********************************************************************************
Start the rendering process by calling PreRendering()
********************************************************************************/
void CSceneManager::PreRendering(Mtx44 translate, bool enableLight)
{
	modelStack.PushMatrix();
	modelStack.MultMatrix(translate);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
}

/********************************************************************************
Complete the rendering process by calling the PostRender()
********************************************************************************/
void CSceneManager::PostRendering(void)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	modelStack.PopMatrix();
}

/********************************************************************************
Render text onto the screen with reference position in the middle of the image
********************************************************************************/
void CSceneManager::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.8f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

/********************************************************************************
Render text onto the screen
********************************************************************************/
void CSceneManager::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	//ortho.SetToOrtho(0, m_window_width, 0, m_window_height, -10, 10);
	ortho.SetToOrtho(0, m_window_width, 0, m_window_height, -10, 10);
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
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
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
	glEnable(GL_DEPTH_TEST);
}

/********************************************************************************
Render a mesh in 2D
********************************************************************************/
void CSceneManager::RenderMeshIn2D(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
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

/********************************************************************************
Render a mesh
********************************************************************************/
void CSceneManager::RenderMesh(Mesh *mesh, bool enableLight)
{
	if (mesh == NULL)
		return;

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
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
}

/********************************************************************************
Render mobile objects
********************************************************************************/
void CSceneManager::RenderGUI()
{
	// Render the crosshair
	RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 10.0f);

	// Render the crosshair
	// Note that Ortho is set to this size -> 	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	RenderMeshIn2D(m_cMinimap->GetAvatar(), false, 20.0f, 68, -48, true);
	RenderMeshIn2D(m_cMinimap->GetBorder(), false, 20.0f, 68, -48);
	RenderMeshIn2D(m_cMinimap->GetBackground(), false, 20.0f, 68, -48);

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "Projectiles: " << m_cProjectileManager->NumOfActiveProjectile;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 30, 0, 6);


	std::ostringstream s;
	s.precision(5);
	s << "Score: " << score;
	RenderTextOnScreen(meshList[GEO_TEXT], s.str(), Color(0, 1, 0), 30, 0, 16);
}

/********************************************************************************
Render mobile objects
********************************************************************************/
void CSceneManager::RenderMobileObjects()
{
	// Render LightBall
	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	//Draw the scene graph
	m_cSceneGraph->Draw(this);

	modelStack.PushMatrix();
	modelStack.Translate(0, -20, 0);
	m_cAvatarNode->Draw(this);
	modelStack.PopMatrix();
	
	// Render the projectile
	Vector3 ProjectilePosition;
	for (int i = 0; i < m_cProjectileManager->GetMaxNumberOfProjectiles(); i++)
	{
		if (m_cProjectileManager->IsActive(i))
		{
			modelStack.PushMatrix();
			ProjectilePosition = m_cProjectileManager->theListOfProjectiles[i]->GetPosition();
			modelStack.Translate(ProjectilePosition.x, ProjectilePosition.y, ProjectilePosition.z);

			if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PROJ_TYPE_DISCRETE)
			{
				RenderMesh(meshList[GEO_SPHERE], false);
			}
			else if (m_cProjectileManager->theListOfProjectiles[i]->GetType() == CProjectile::PROJ_TYPE_RAY)
			{
				glLineWidth(5.0f);
				float theta = Math::RadianToDegree(atan2(m_cProjectileManager->theListOfProjectiles[i]->GetDirection().x, m_cProjectileManager->theListOfProjectiles[i]->GetDirection().z));
				modelStack.Rotate(theta, 0, 1, 0);
				RenderMesh(meshList[GEO_RAY], false);
				glLineWidth(1.0f);
			}
			modelStack.PopMatrix();
		}
	}

	// Render the avatar
	modelStack.PushMatrix();
	modelStack.Translate(m_cAvatar->GetPos_x(), m_cAvatar->GetPos_y(), m_cAvatar->GetPos_z());
	modelStack.Translate(0, -60, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	float theta = Math::RadianToDegree(atan2(m_cAvatar->GetDirection().x, m_cAvatar->GetDirection().z));
	modelStack.Rotate(theta, 0, 1, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(m_cAvatar->theAvatarMesh, false);
	modelStack.PopMatrix();
}

/********************************************************************************
Render the lights in this scene
********************************************************************************/
void CSceneManager::RenderFixedObjects()
{
	RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	modelStack.Translate(-20, 0, -20);
	RenderMesh(meshList[GEO_OBJECT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(20, 0, -20);
	RenderMesh(meshList[GEO_OBJECT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	//RenderText(meshList[GEO_TEXT], "Hello World", Color(0, 1, 0));
	RenderText(meshList[GEO_TEXT], "DM2240 AGDEV", Color(0, 1, 0));
	modelStack.PopMatrix();
}

/********************************************************************************
Render the lights in this scene
********************************************************************************/
void CSceneManager::RenderLights()
{
	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	if (lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}
}

/********************************************************************************
Render the ground in this scene
********************************************************************************/
void CSceneManager::RenderGround()
{
	// Render the Spatial Partitions
	modelStack.PushMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(m_cSpatialPartition->xGridSize * 0.5, 0, m_cSpatialPartition->yGridSize * 0.5);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -99);

	for (int i = 0; i < m_cSpatialPartition->GetxNumOfGrid(); i++)
	{
		for (int j = 0; j < m_cSpatialPartition->GetyNumOfGrid(); j++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_cSpatialPartition->xGridSize * i,
				-m_cSpatialPartition->yGridSize * j, 0.0f);
			if (m_cSpatialPartition->theGrid[i * m_cSpatialPartition->yNumOfGrid + j].ListOfObjects.size() > 0)
			{
				IfGridNotEmpty(i, j);
			}
			else if (m_cSpatialPartition->theGrid[i * m_cSpatialPartition->yNumOfGrid + j].ListOfObjects.size() == 0)
			{
				IfGridEmpty(i, j);
			}
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

/********************************************************************************
Render the skybox in this scene
********************************************************************************/
void CSceneManager::RenderSkybox()
{
	//left
	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -100);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
}

/********************************************************************************
Render this scene
********************************************************************************/
void CSceneManager::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Set up the view
	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	RenderLights();
	RenderGround();
	RenderSkybox();
	RenderMobileObjects();
	RenderGUI();
}

/********************************************************************************
Exit process for this scene
********************************************************************************/
void CSceneManager::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
		meshList[i] = NULL;
	}

	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
