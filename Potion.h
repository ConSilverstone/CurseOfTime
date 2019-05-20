#pragma once

//Project Includes
#include "SFML/Audio.hpp"
#include "GridObject.h"

class Player;

class Potion : public GridObject
{
public:
	Potion();

	//Overriding Methods
	virtual void Update(sf::Time _frameTime);
	virtual void Collide(GameObject& _collider);

	//Data
	//An enum state structure to check which element is active, 
	//none is a state as the player may wish to deselect an element
	enum PotionEffects
	{
		none
		, Sulphur
		, Collagen
		, Hydrogen
		, Electricity
	};

	// Public Methods
	void SetPlayer(Player* _player);

private:
	
	// Class References
	Player* m_player;

};