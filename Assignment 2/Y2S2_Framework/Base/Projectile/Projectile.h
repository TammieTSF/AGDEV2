#pragma once

#include "Vector3.h"
#include <stdio.h>

class CProjectile
{
public:

	CProjectile(void);
	~CProjectile(void);

	enum PROJECTILE_TYPE
	{
		PROJ_TYPE_DISCRETE,
		PROJ_TYPE_RAY,
		NUM_PROJ_TYPE,
	};

	// Update the status of this projectile
	void Update(const double dt);
	// Activate the projectile. true == active, false == inactive
	void SetActivate(bool m_bActivate);
	// get status of the projectile. true == active, false == inactive
	bool GetStatus(void);
	// Get the position of the projectile
	Vector3 GetPosition();
	// Get the direction of the projectile
	Vector3 GetDirection(void);
	// Get the length of the projectile
	float GetLength(void);
	// Get the projectile type
	int GetType(void);
	// Set the position of the projectile
	void SetPosition(Vector3 theNewPosition);
	// Set the direction of the projectile
	void SetDirection(Vector3 theNewDirection);
	// Set the length of the ray projectile
	void SetLength(const float theLength);
	// Get the projectile type
	void SetType(const int theType);
	// Set speed
	void SetSpeed(const float speed);
	// Set the lifetime
	void SetLifetime(const float Lifetime);
	// Get the lifetime
	float GetLifetime(void);

private:
	// Boolean flag to indicate if this projectile is active. If not active, then do not compute/update
	bool m_bActive;
	// The position of the projectile
	Vector3 thePosition;
	// The direction of the projectile
	Vector3 theDirection;
	// The length of the ray projectile
	float theLength;
	// Flag to indicate if this is a sweep projectile. 0 = descrete, 1 = sweep
	int theType;
	// Speed
	float speed;
	// Lifetime of bullet
	float LifeTime;

};
