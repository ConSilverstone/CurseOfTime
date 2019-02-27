#pragma once

//Project Includes
#include "GridObject.h"

class Boulder : public GridObject
{
public:
	Boulder();

	void Update(sf::Time _frameTime);

	bool canItFall(sf::Vector2i _direction);
	bool attemptFall(sf::Vector2i _direction);
	bool attemptPush(sf::Vector2i _direction);

private:

	float m_currentTime;
	float m_timeFallen;
	bool hasSlid;

};