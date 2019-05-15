#pragma once

#include "Framework/TextObject.h"
#include "Player.h"

class Timer : public TextObject
{

public:

	Timer();

	virtual void Update(sf::Time _frameTime);

	// Public Score Methods
	void SetPlayer(Player* _player);

private:

	Player* m_player;
};
