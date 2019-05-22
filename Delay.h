#pragma once

#include "Framework/TextObject.h"
#include "Player.h"

// We have a delay for potions and elements, but we want to player to know how long that delay is //

class Delay : public TextObject
{

public:

	Delay();

	virtual void Update(sf::Time _frameTime);

	// Public Score Methods
	void SetPlayer(Player* _player);

private:

	Player* m_player;
};
