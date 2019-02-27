#pragma once

//Project Includes
#include "GridObject.h"

class Diamond : public GridObject
{
public:
	Diamond();

	void Update(sf::Time _frameTime);

	bool canItFall(sf::Vector2i _direction);
	bool attemptFall(sf::Vector2i _direction);

private:

	float m_currentTime;
	float m_timeFallen;

};