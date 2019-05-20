#pragma once

//Project Includes
#include "GridObject.h"
#include "Player.h"

class Electricity : public GridObject
{
public:
	Electricity();

	void Update(sf::Time _frameTime);

	// Public Score Methods
	void SetPlayer(Player* _player);

private:

	Player* m_player;

};