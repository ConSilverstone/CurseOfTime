#pragma once

//Project Includes
#include "SFML/Audio.hpp"
#include "GridObject.h"


// Forward Declaration //
class Potion;

class Player : public GridObject
{
//public:
//	GridObject();
//
//	void SetGridPosition(sf::Vector2i _newPosition);
//	void SetGridPosition(int _x, int _y);
//
//	void SetLevel(Level* _newLevel);
//
//	sf::Vector2i GetGridPosition();
//	bool GetBlocksMovement();
//
//	 Overriding Methods
//	void Update(sf::Time _frameTime);
//
//protected:
//	bool m_blocksMovement;
//	sf::Vector2i m_gridPosition;
//	Level* m_level;
//// Moving Objects
//	sf::Vector2f m_velocity; // pixels per second
//	sf::Vector2f m_previousPosition;

public:
	Player();

	//Overriding Methods
	virtual void Update(sf::Time _frameTime);
	virtual void Collide(GameObject& _collider);

	// Public Player Methods
	int GetTimer();
	int GetDelay();
	int GetPotionState();
	bool GetGameStart();
	bool GetPlayerDirectionRight();
	void ChangeTimer(int _changeBy);
	void ChangeDelay(int _changeBy);

	// To manipulate data in other code, we first need to set out pointer to mean something. //
	void SetPotion(Potion* _potion);

private:
	//Data
	sf::Sound m_playerMoveSound;
	sf::Sound m_playerBumpingSound;
	sf::Vector2i m_pendingMove;
	float m_timerCountdown;
	//For the Potion and PotionEffects
	float m_keyDelay;
	// Creating a numerical value for each potion state, to change the texture based on if they are active
	int m_numericalPotionState;
	//Add a delay for sounds in positions of continual update
	float m_soundDelay;

	//Class References
	Potion* m_potion;

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

	PotionEffects potionState;
	PotionEffects previousPotionState;

	//State
	bool m_touchingSurface;
	bool m_touchingRoof;
	bool m_touchingWall;
	bool m_gameStart;
	bool m_keyBeenPressed;
	bool m_walkingSoundStarted;
	bool m_isPlayerFacingRight;

	//Sound Effects
	sf::Sound m_boxPush;
	sf::Sound m_ElementSelect;
	sf::Sound m_Jump;
	sf::Sound m_Landing;
	sf::Sound m_OutOfTime;
	sf::Sound m_Walking;
	sf::Sound m_WaterWalk;
};
