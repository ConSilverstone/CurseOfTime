#pragma once

//Project Includes
#include "GridObject.h"
#include "AnimationSystem.h"

class WaterDown : public GridObject
{
public:
	WaterDown();

	//Overiding Methods
	void Update(sf::Time _frameTime);

private:

	AnimationSystem m_animationSystem;
};