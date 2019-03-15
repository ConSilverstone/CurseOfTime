#pragma once

//Project Includes
#include "GridObject.h"
#include "SFML/Audio.hpp"

class Player : public GridObject
{
public:
	Player();

	//Overriding Methods
	virtual void Input(sf::Event _gameEvent);
	virtual void Update(sf::Time _frameTime);

	// Initally spawning the player for the camera//
	void Spawn();
	//For spawning the camera//
	

private:
	//Data
	bool AttemptMove(sf::Vector2i _direction);
	sf::Sound m_playerMoveSound;
	sf::Sound m_playerBumpingSound;
	sf::Vector2i m_pendingMove;

	//State
	sf::Vector2f m_velocity;
};
