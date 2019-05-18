#pragma once

//Project Includes
#include "SFML/Audio.hpp"
#include "GridObject.h"

class Potion : public GridObject
{
public:
	Potion();

	//Overriding Methods
	virtual void Update(sf::Time _frameTime);
	virtual void Collide(GameObject& _collider);

private:
	
	//Data
	enum PotionEffects
	{
		 Sulphur
		,Collagen
		,Hydrogen
		,Electricity
	};
};