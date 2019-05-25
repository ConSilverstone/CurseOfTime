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
	bool AttemptPush(sf::Vector2i _direction);

	//Data
	Player* m_player;

private:

	//State
	bool m_touchingGround;
	bool m_touchingWall;

	// Data
	float m_AmountMoved;
};