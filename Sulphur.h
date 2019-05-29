#pragma once

//Project Includes
#include "GridObject.h"
#include "Player.h"
#include "Potion.h"

class Sulphur : public GridObject
{
public:
	Sulphur();

	// Overiding Methods
	void Update(sf::Time _frameTime);

	// Public Score Methods
	void SetPlayer(Player* _player);

private:

	//Class References
	Potion* m_potion;
	Player* m_player;

};