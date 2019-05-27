#pragma once

//Project Includes
#include "SFML/Audio.hpp"
#include "GridObject.h"


// Forward Declaration //
class Potion;

class Player : public GridObject
{
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
	bool GetPlayerDirectionDown();
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
	//For the PotionEffects
	float m_keyDelay;
	// For the thrown potion
	float m_potionDelay;
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
	bool m_touchingSurface; // Is the player touching a surface for jumping and collagen //
	bool m_touchingWall; // Is the player touching a wall to stop them from clipping through it //
	bool m_gameStart; // Has the game begun for the timer //
	bool m_keyBeenPressed; // Has a element key been pressed so stop flickering between on and off //
	bool m_potionBeenThrown; // Has a potion been thrown to stop the play from spamming them //
	bool m_gameSoundStarted; // Is the sound started so that we can add a delay to stop spam playing //

	bool m_isPlayerFacingRight; // Is the player facing the right or left to know which way we want to throw a potion //
	bool m_isPlayerThrowingDown; // Is the player attempting to throw the potion straight down? //

	bool m_ShouldWeCheckMusic; // Run the music player from level and find out what song should be playing right now // 
	bool m_gameStartBeenChecked; // Run the music player from level for new songs if the game has started but has only been checked once //

	//Sound Effects
	sf::Sound m_boxPush;
	sf::Sound m_ElementSelect;
	sf::Sound m_Jump;
	sf::Sound m_Landing;
	sf::Sound m_OutOfTime;
	sf::Sound m_Walking;
	sf::Sound m_WaterWalk;
};
