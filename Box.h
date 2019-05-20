#pragma once

//Project Includes
#include "GridObject.h"
#include "Player.h"

class Box : public GridObject
{
public:
	Box();

	//Overriding Methods
	void Update(sf::Time _frameTime);
	void Collide(GameObject& _collider);

	void BoxSlide(sf::Vector2f _velocity);

	//Data
	Player* m_player;

private:

	//State
	bool m_touchingGround;
	bool m_touchingWall;

};