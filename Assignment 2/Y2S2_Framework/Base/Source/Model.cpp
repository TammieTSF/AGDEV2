#include "Model.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#include <windows.h> // Header File For Windows
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library

#include <stdio.h>
#include "MeshBuilder.h"

CModel::CModel(void)
	: m_cModelMesh(NULL)
	, m_iCurrentResolution(HIGH_RESOLUTION)
	, theArrayOfMeshes(NULL)
{
	vTopLeft = Vector3(1.0f, 1.0f, 1.0f);
	vBottomRight = Vector3(-1.0f, -1.0f, -1.0f);
	red = 128;
	green = 255;
	blue = 0;

	theArrayOfMeshes = new Mesh*[NUM_RESOLUTIONS];
	for (int i = 0; i < NUM_RESOLUTIONS; i++)
		theArrayOfMeshes[i] = NULL;
}

CModel::~CModel(void)
{
	if (m_cModelMesh != NULL)
	{
		delete m_cModelMesh;
		m_cModelMesh = NULL;
	}
	if (theArrayOfMeshes != NULL)
	{
		for (int i = 0; i < NUM_RESOLUTIONS; i++)
			delete theArrayOfMeshes[i];

		delete[] theArrayOfMeshes;
		theArrayOfMeshes = NULL;
	}
}

void CModel::Init(void)
{
	m_cModelMesh = MeshBuilder::GenerateCube("cube", Color(1, 0, 0));

	theArrayOfMeshes[LOW_RESOLUTION] = MeshBuilder::GenerateCube("cube", Color(1, 0, 0));
	theArrayOfMeshes[MEDIUM_RESOLUTION] = MeshBuilder::GenerateCube("cube", Color(1, 0, 0));
	theArrayOfMeshes[HIGH_RESOLUTION] = MeshBuilder::GenerateCube("cube", Color(1, 0, 0));
}

void CModel::Init(Mesh* m_cModelMesh)
{
	this->m_cModelMesh = m_cModelMesh;
}

void CModel::Draw(bool m_bLight)
{
	// Display the mesh subjected to the current resolution
	theArrayOfMeshes[m_iCurrentResolution]->Render();

	if (m_cModelMesh)
		m_cModelMesh->Render();
}

// Get Current Resolution
int CModel::GetResolution(void)
{
	return m_iCurrentResolution;
}

// Set Current Resolution
void CModel::SetResolution(const int resolutionType)
{
	// Check that the supplied resolutionType is valid before setting it to m_iCurrentResolution
	if ((resolutionType >= LOW_RESOLUTION) && (resolutionType <= HIGH_RESOLUTION))
	{
		m_iCurrentResolution = resolutionType;
	}
}

void CModel::SetColor(const float red, const float green, const float blue)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
}

// Get the top left of the bounding box of the instance
Vector3 CModel::GetTopLeft(void)
{
	return Vector3(vTopLeft.x, vTopLeft.y, vTopLeft.z);
}

// Get the bottomright of the bounding box of the instance
Vector3 CModel::GetBottomRight(void)
{
	return Vector3(vBottomRight.x, vBottomRight.y, vBottomRight.z);
}