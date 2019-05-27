#pragma once

//Project Includes
#include "GridObject.h"
#include "Player.h"
#include "SFML/Audio.hpp"

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
	bool m_isPushed; // We only want the box to run collision code and gravity if it has been pushed, to prevent lag//

	//Data
	float m_AmountMoved;

	//Sounds
	sf::Sound m_boxPush;
};