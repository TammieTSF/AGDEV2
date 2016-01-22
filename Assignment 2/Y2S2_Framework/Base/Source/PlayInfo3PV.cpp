#include "PlayInfo3PV.h"
#include "Application.h"
#include "MeshBuilder.h"

static const float CAMERA_SPEED = 200.f;

CPlayInfo3PV::CPlayInfo3PV(void)
	: theAvatarMesh(NULL)
	, theNode(NULL)
	, jumpspeed(0)
{
	Init();
}

CPlayInfo3PV::~CPlayInfo3PV(void)
{
	if (theAvatarMesh)
	{
		delete theAvatarMesh;
		theAvatarMesh = NULL;
	}
	if (theNode)
	{
		delete theNode;
		theNode = NULL;
	}
}

// Initialise this class instance
void CPlayInfo3PV::Init(void)
{
	curPosition.Set(0, 0, 0);
	curDirection.Set(0, 0, 1);
	Up.Set(0, 1, 0);

	// Initialise the Avatar's movement flags
	for (int i = 0; i < 255; i++){
		myKeys[i] = false;
	}
}

// Set Model
bool CPlayInfo3PV::SetModel(Mesh* theAvatarMesh)
{
	if (theAvatarMesh != NULL)
	{
		this->theAvatarMesh = theAvatarMesh;
		return true;
	}
	return false;
}

// Set Model
bool CPlayInfo3PV::TakeNode(CSceneNode* theNode)
{
	if (theNode != NULL)
	{
		this->theNode = theNode;
		return true;
	}
	return false;
}

// Returns true if the player is on ground
bool CPlayInfo3PV::isOnGround(void)
{
	return true;
}

// Returns true if the player is jumping upwards
bool CPlayInfo3PV::isJumpUpwards(void)
{
	return true;
}

// Returns true if the player is on freefall
bool CPlayInfo3PV::isFreeFall(void)
{
	return true;
}

// Set the player's status to free fall mode
void CPlayInfo3PV::SetOnFreeFall(bool isOnFreeFall)
{
	if (isOnFreeFall == true)
	{
		jumpspeed = 0;
	}
}

// Set the player to jumping upwards
void CPlayInfo3PV::SetToJumpUpwards(bool isOnJumpUpwards)
{
	if (isOnJumpUpwards == true)
	{
		jumpspeed = 15;
	}
}

// Set position x of the player
void CPlayInfo3PV::SetPos_x(float pos_x)
{
	curPosition.x = pos_x;
}

// Set position y of the player
void CPlayInfo3PV::SetPos_y(float pos_y)
{
	curPosition.y = pos_y;
}

// Set position y of the player
void CPlayInfo3PV::SetPos_z(float pos_z)
{
	curPosition.z = pos_z;
}

// Set Jumpspeed of the player
void CPlayInfo3PV::SetJumpspeed(int jumpspeed)
{
	this->jumpspeed = jumpspeed;
}

// Stop the player's movement
void CPlayInfo3PV::SetToStop(void)
{
	jumpspeed = 0;
}

/********************************************************************************
Hero Move Up Down
********************************************************************************/
void CPlayInfo3PV::MoveFrontBack(const bool mode, const float timeDiff, Camera3 &camera)
{
	if (mode) // move back
	{
		Vector3 temp;
		temp.Set(curDirection.x, 0, curDirection.z);
		curPosition -= (5.0 * temp);
		camera.position -= (5.0f * temp);
	}
	else // move front
	{
		Vector3 temp;
		temp.Set(curDirection.x, 0, curDirection.z);
		curPosition += (5.0 * temp);
		camera.position += (5.0f * temp);
	}
}

/********************************************************************************
Hero Move Left Right
********************************************************************************/
void CPlayInfo3PV::MoveLeftRight(const bool mode, const float timeDiff, Camera3 &camera)
{
	if (mode) // left
	{
		Vector3 temp;
		temp.Set(curDirection.x, 0, curDirection.z);
		temp = temp.Cross(Up);
		curPosition -= (5.0f * temp);
		camera.position -= (5.0f * temp);
	}
	else // right
	{
		Vector3 temp;
		temp.Set(curDirection.x, 0, curDirection.z);
		temp = temp.Cross(Up);
		curPosition += (5.0f * temp);
		camera.position += (5.0f * temp);
	}
}

// Get position x of the player
float CPlayInfo3PV::GetPos_x(void)
{
	return curPosition.x;
}

// Get position y of the player
float CPlayInfo3PV::GetPos_y(void)
{
	return curPosition.y;
}

// Get position y of the player
float CPlayInfo3PV::GetPos_z(void)
{
	return curPosition.z;
}

// Get position of the player
Vector3 CPlayInfo3PV::GetPosition()
{
	return curPosition;
}

// Get direction of the player
Vector3 CPlayInfo3PV::GetDirection()
{
	return curDirection;
}

// Get Jumpspeed of the player
int CPlayInfo3PV::GetJumpspeed(void)
{
	return jumpspeed;
}

// Update Jump Upwards
void CPlayInfo3PV::UpdateJumpUpwards()
{
	curPosition.y -= jumpspeed;
	jumpspeed -= 1;
	if (jumpspeed == 0)
	{
	}
}

// Update FreeFall
void CPlayInfo3PV::UpdateFreeFall()
{
	curPosition.y += jumpspeed;
	jumpspeed += 1;
}

// Constrain the position of the Hero to within the border
void CPlayInfo3PV::ConstrainHero(const int leftBorder, const int rightBorder,
	const int topBorder, const int bottomBorder, float timeDiff)
{
	if (curPosition.x < leftBorder)
	{
		curPosition.x = leftBorder;
	}
	else if (curPosition.x > rightBorder)
	{
		curPosition.x = rightBorder;
	}

	if (curPosition.y < topBorder)
		curPosition.y = topBorder;
	else if (curPosition.y > bottomBorder)
		curPosition.y = bottomBorder;
}

/********************************************************************************
Update Movement
********************************************************************************/
void CPlayInfo3PV::UpdateMovement(const unsigned char key, const bool status)
{
	myKeys[key] = status;
}

/********************************************************************************
Pitch. You can add in a deadzone here.
********************************************************************************/
void CPlayInfo3PV::Pitch(const double dt)
{
	//offset += Application::camera_pitch;

	if (Application::camera_pitch > 0.0)
		LookUp(-Application::camera_pitch * dt);

	else if (Application::camera_pitch < 0.0)
		LookDown(-Application::camera_pitch * dt);
}

/********************************************************************************
Yaw. You can add in a deadzone here.
********************************************************************************/
void CPlayInfo3PV::Yaw(const double dt)
{
	if (Application::camera_yaw > 0.0)
		TurnRight(Application::camera_yaw * dt);
	else if (Application::camera_yaw < 0.0)
		TurnLeft(Application::camera_yaw * dt);
}

/********************************************************************************
Turn right
********************************************************************************/
void CPlayInfo3PV::TurnRight(const double dt)
{
	float yaw = (float)(-CAMERA_SPEED * (float)dt);
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	curDirection = rotation * curDirection; //MatrixTransformation
}

/********************************************************************************
Turn left
********************************************************************************/
void CPlayInfo3PV::TurnLeft(const double dt)
{
	float yaw = (float)(-CAMERA_SPEED * (float)dt);
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	curDirection = rotation * curDirection; //MatrixTransformation
}

/********************************************************************************
LookUp
********************************************************************************/
void CPlayInfo3PV::LookUp(const double dt)
{
	float pitch = (float)(-CAMERA_SPEED * (float)dt);
	Vector3 view = (curDirection).Normalized();
	Vector3 right = view.Cross(Up);
	right.y = 0;
	right.Normalize();
	Up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	view = rotation * view;
	curDirection = view;
}

/********************************************************************************
LookDown
********************************************************************************/
void CPlayInfo3PV::LookDown(const double dt)
{
	float pitch = (float)(-CAMERA_SPEED * (float)dt);
	Vector3 view = (curDirection).Normalized();
	Vector3 right = view.Cross(Up);
	right.y = 0;
	right.Normalize();
	Up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	view = rotation * view;
	curDirection = view;
}

/********************************************************************************
Update
********************************************************************************/
void CPlayInfo3PV::Update(double dt, Camera3 &camera)
{
	curDirection = (camera.target - camera.position).Normalize();

	// WASD movement
	if (myKeys['w'] == true)
	{
		MoveFrontBack(false, dt, camera);
	}
	if (myKeys['s'] == true)
	{
		MoveFrontBack(true, dt, camera);
	}
	if (myKeys['a'] == true)
	{
		MoveLeftRight(true, dt, camera);
	}
	if (myKeys['d'] == true)
	{
		MoveLeftRight(false, dt, camera);
	}

	if (Application::camera_yaw != 0)
		Yaw(dt);

	if (Application::camera_pitch != 0)
		Pitch(dt);
}