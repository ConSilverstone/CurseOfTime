#pragma once

//Project Includes
#include "GridObject.h"

class Box : public GridObject
{
public:
	Box();

	//Overriding Methods
	void Update(sf::Time _frameTime);
	void Collide(GameObject& _collider);

	void BoxSlide(sf::Vector2f _velocity);

private:

	//State
	bool m_touchingGround;
	bool m_touchingWall;

};