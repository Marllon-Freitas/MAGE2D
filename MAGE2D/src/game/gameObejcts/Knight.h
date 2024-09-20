#ifndef _MAGE2D_SHANK_H_
#define _MAGE2D_SHANK_H_

#include "../../engine/Types.h"             
#include "../../engine/Object.h"            
#include "../../engine/Sprite.h"            

class Knight : public Object
{
private:
	Sprite* sprite;

public:
	Knight();
	~Knight();

	void Update();
	void Draw();
};

#endif