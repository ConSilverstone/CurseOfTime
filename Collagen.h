#pragma once

//Project Includes
#include "GridObject.h"
#include "Player.h"

class Collagen : public GridObject
{
public:
	Collagen();

	// Overriding Methods
	virtual void Update(sf::Time _frameTime);

	// Public Score Methods
	void SetPlayer(Player* _player);

private:

	Player* m_player;
};
