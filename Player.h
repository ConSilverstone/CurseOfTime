#pragma once

//Project Includes
#include "SFML/Audio.hpp"
#include "GridObject.h"

class Player : public GridObject
{
//public:
//	GridObject();
//
//	void SetGridPosition(sf::Vector2i _newPosition);
//	void SetGridPosition(int _x, int _y);
//
//	void SetLevel(Level* _newLevel);
//
//	sf::Vector2i GetGridPosition();
//	bool GetBlocksMovement();
//
//	 Overriding Methods
//	void Update(sf::Time _frameTime);
//
//protected:
//	bool m_blocksMovement;
//	sf::Vector2i m_gridPosition;
//	Level* m_level;
//// Moving Objects
//	sf::Vector2f m_velocity; // pixels per second
//	sf::Vector2f m_previousPosition;

public:
	Player();

	//Overriding Methods
	virtual void Update(sf::Time _frameTime);
	virtual void Collide(GameObject& _collider);

	// Public Player Methods
	int GetTimer();
	void ChangeTimer(int _changeBy);

private:
	//Data
	sf::Sound m_playerMoveSound;
	sf::Sound m_playerBumpingSound;
	sf::Vector2i m_pendingMove;
	float m_timerCountdown;

	//State
	bool m_touchingGround;
	bool m_touchingWall;
	bool m_gameStart;
};
