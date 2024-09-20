#ifndef _MAGE2D_OBJECT_H_
#define _MAGE2D_OBJECT_H_

#include "Types.h"                  // using custom types from the engine
#include "Window.h"                 // window used for the game

class Object
{
public:
	static Window*& window;         // game window
	static float& gameTime;         // time of the last frame
	float x, y, z;					// object coordinates

public:
	Object();
	virtual ~Object();

	// move the object by (deltaX, deltaY, deltaZ)
	virtual void Translate(float dx, float dy, float dz = 0.0f);

	// move the object to the specified coordinates (x, y, z)
	virtual void MoveTo(float px, float py, float pz);

	// move the object to the specified coordinates (x, y)
	virtual void MoveTo(float px, float py);

	// update object state
	virtual void Update() = 0;

	// draw the object on the screen
	virtual void Draw() = 0;
};

// move the object by (deltaX, deltaY, deltaZ)
inline void Object::Translate(float dx, float dy, float dz)
{
	x += dx; y += dy; z += dz;
}

// move the object to the specified coordinates (x, y, z)
inline void Object::MoveTo(float px, float py, float pz)
{
	x = px; y = py; z = pz;
}

// move the object to the specified coordinates (x, y)
inline void Object::MoveTo(float px, float py)
{
	x = px; y = py;
}

// -----------------------------------------------------------------------------

#endif
