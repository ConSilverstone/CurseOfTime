#pragma once

//Project Includes
#include "SFML/Audio.hpp"
#include "GridObject.h"

class Player : public GridObject
{
public:
	Player();

	virtual void Update(sf::Time _frameTime);
	
private:
	//Data
	bool AttemptMove(sf::Vector2i _direction);
	sf::Sound m_playerMoveSound;
	sf::Sound m_playerBumpingSound;
	sf::Vector2i m_pendingMove;

	//State
	sf::Vector2f m_velocity;
};
