#pragma once

#include "Framework/TextObject.h"

class Timer : public TextObject
{

public:

	Timer();

	virtual void Update(sf::Time _frameTime);
};
