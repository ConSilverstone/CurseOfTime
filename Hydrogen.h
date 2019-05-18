#pragma once

//Project Includes
#include "GridObject.h"
#include "Player.h"

class Hydrogen : public GridObject
{
public:
	Hydrogen();

	void Update(sf::Time _frameTime);

	// Public Score Methods
	void SetPlayer(Player* _player);

private:

	Player* m_player;

};
