// Project Includes
# include "Player.h"
# include "Framework/AssetManager.h"
# include "Level.h"
# include "SFML/Audio.hpp"
# include "Diamond.h"
# include "Exit.h"
# include "Box.h"
# include "Wall.h"
# include "Spike.h"
# include "Killzone.h"
# include "Potion.h"
# include "Collagen.h"
# include "CrackedWall.h"
# include "WaterSide.h"
# include "WaterDown.h"

// Constants
#define SPEED 200.0f
#define JUMP_SPEED -370.0f
#define HydrogenFloat -150.0f
#define GRAVITY 220.0f
#define PotionXVelocity 500.0f
#define PotionYVelocity -300.0f

Player::Player()
	: GridObject()
	, m_pendingMove(0, 0)
	, m_playerMoveSound()
	, m_playerBumpingSound()
	, m_touchingSurface(false)
	, m_touchingWall(false)
	, m_keyBeenPressed(false)
	, m_potionBeenThrown(false)
	, m_timerCountdown(60.00f)
	, m_keyDelay(0.5f)
	, m_potionDelay(3.0f)
	, m_numericalPotionState(0)
	, m_gameStart(false)
	, m_ShouldWeCheckMusic(true)
	, m_gameStartBeenChecked(false)
	, m_potion(nullptr)
	, potionState(none)
	, previousPotionState(none)
	, m_ElementSelect()
	, m_Jump()
	, m_Landing()
	, m_OutOfTime()
	, m_Walking()
	, m_WaterWalk()
	, m_soundDelay(0.2f)
	, m_gameSoundStarted(false)
	, m_hasTouchedWaterRight(false)
	, m_hasTouchedWaterLeft(false)
	, m_isPlayerFacingRight(true)
	, m_isPlayerThrowingDown(false)
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandLeft.png"));
	m_blocksMovement = true;

	////////////////
	/// SOUNDS /////
	////////////////
	m_ElementSelect.setBuffer(AssetManager::GetSoundBuffer("audio/ElementSelect.wav"));
	m_Jump.setBuffer(AssetManager::GetSoundBuffer("audio/Jump.wav"));
	m_Landing.setBuffer(AssetManager::GetSoundBuffer("audio/Land.wav"));
	m_OutOfTime.setBuffer(AssetManager::GetSoundBuffer("audio/OutofTime.wav"));
	m_Walking.setBuffer(AssetManager::GetSoundBuffer("audio/Walking.wav"));
	m_WaterWalk.setBuffer(AssetManager::GetSoundBuffer("audio/WaterWalk.wav"));
}

void Player::Update(sf::Time _frameTime)
{
	// First, assume no keys are pressed
	m_velocity.x = 0.0f;

	////////////////
	/// CONTROLS ///
	////////////////

	// Use the keyboard function to check 
	// which keys are currently held down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && m_touchingSurface == true)
	{
		// The player can jump so long as Hydrogen is not active //
		if (potionState != Hydrogen)
		{
			if (m_soundDelay <= 0 && m_touchingSurface == true)
			{
				m_Jump.play();
				m_soundDelay = 0.2f;
			}

			m_velocity.y = JUMP_SPEED;
			m_touchingSurface = false;
			m_gameStart = true;
			// Start the countdown to the next instance the player jump sound sound be played
			m_gameSoundStarted = true;

			// Keep track of if the player is trying to throw a potion straight down //
			m_isPlayerThrowingDown = false;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		// The play can move left or right so long as Hydrogen is not active //
		if (potionState != Hydrogen)
		{
			if (m_soundDelay <= 0 && m_touchingSurface == true)
			{
				m_Walking.play();
				m_soundDelay = 0.2f;
			}

			m_velocity.x = -SPEED;
			m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandLeft.png"));
			// The game has now begun
			m_gameStart = true;
			// Start the countdown to the next instance the player walking sound sound be played
			m_gameSoundStarted = true;
			// The player is not facing right for the purpose of throwning a potion
			m_isPlayerFacingRight = false;

			// Keep track of if the player is trying to throw a potion straight down //
			m_isPlayerThrowingDown = false;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && m_touchingSurface == false)
	{
		m_velocity.y = SPEED;
		m_gameStart = true;

		// If the player has pressed this key, it means that they will drop off the roof of they haven't aleady done so. 
		// This will allow them to jump again.
		m_touchingSurface = false;

		// Keep track of if the player is trying to throw a potion straight down //
		m_isPlayerThrowingDown = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		// The play can move left or right so long as Hydrogen is not active //
		if (potionState != Hydrogen)
		{

			if (m_soundDelay <= 0 && m_touchingSurface == true)
			{
				m_Walking.play();
				m_soundDelay = 0.2f;
			}

			m_velocity.x = SPEED;
			m_sprite.setTexture(AssetManager::GetTexture("graphics/player/playerStandRight.png"));
			// The game has now begun
			m_gameStart = true;
			// Start the countdown to the next instance the player walking sound sound be played
			m_gameSoundStarted = true;
			// The player is facing right for the purpose of throwning a potion
			m_isPlayerFacingRight = true;

			// Keep track of if the player is trying to throw a potion straight down //
			m_isPlayerThrowingDown = false;
		}
	}

	/////////////
	// Potions //
	/////////////

	// ELEMENT CONTROLS //
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		// The player has pressed the 1 key, first we need to check if this element is already active.
		if (potionState == Collagen && m_keyBeenPressed == false)
		{
			m_ElementSelect.play();
			// It is active, meaning that the player wishes to deactivate this element
			potionState = none;
			m_numericalPotionState = 0;
			m_keyBeenPressed = true;
		} 
		else if (potionState != Collagen && m_keyBeenPressed == false)
		{
			m_ElementSelect.play();
			// It is not active, meaning that the player wishes to activate this element
			potionState = Collagen;
			m_numericalPotionState = 1;
			m_keyBeenPressed = true;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		// The player has pressed the 1 key, first we need to check if this element is already active.
		if (potionState == Electricity && m_keyBeenPressed == false)
		{
			m_ElementSelect.play();
			// It is active, meaning that the player wishes to deactivate this element
			potionState = none;
			m_numericalPotionState = 0;
			m_keyBeenPressed = true;
		}
		 else if (potionState != Electricity && m_keyBeenPressed == false)
		{
			m_ElementSelect.play();
			// It is not active, meaning that the player wishes to activate this element
			potionState = Electricity;
			m_numericalPotionState = 2;
			m_keyBeenPressed = true;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
	{
		// The player has pressed the 1 key, first we need to check if this element is already active.
		if (potionState == Sulphur && m_keyBeenPressed == false)
		{
			m_ElementSelect.play();
			// It is active, meaning that the player wishes to deactivate this element
			potionState = none;
			m_numericalPotionState = 0;
			m_keyBeenPressed = true;
		}
		else if (potionState != Sulphur && m_keyBeenPressed == false)
		{
			m_ElementSelect.play();
			// It is not active, meaning that the player wishes to activate this element
			potionState = Sulphur;
			m_numericalPotionState = 3;
			m_keyBeenPressed = true;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
	{
		// The player has pressed the 4 key, first we need to check if this element is already active.
		if (potionState == Hydrogen && m_keyBeenPressed == false)
		{
			m_ElementSelect.play();
			// It is active, meaning that the player wishes to deactivate this element
			potionState = none;
			m_numericalPotionState = 0;
			m_keyBeenPressed = true;
		}
		else if (potionState != Hydrogen && m_keyBeenPressed == false)
		{
			m_ElementSelect.play();
			// It is not active, meaning that the player wishes to activate this element
			potionState = Hydrogen;
			m_numericalPotionState = 4;
			m_keyBeenPressed = true;
		}
	}

	// Throwing a potion
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && potionState == Sulphur && m_potionBeenThrown == false)
	{
		m_level->CreatePotion();
		if (m_potion != nullptr)
			{ 
				m_potion->SetPosition(m_sprite.getPosition());
			}
		m_potionBeenThrown = true;
		m_gameStart = true;
	}

	//Apply gravity to our velocity
	// If we are not touching any surface on the level or (we are not touching the roof while collagen is active)
	// We don't want gravity to kick in if hydrogen is active
	if (m_touchingSurface == false && m_gameStart == true && potionState != Hydrogen)
	{
			float velocityChange = (GRAVITY * _frameTime.asSeconds()) * 2;
			m_velocity.y += velocityChange;
	}

	//Move sprite base on velocity
	sf::Vector2f positionChange = m_velocity * _frameTime.asSeconds();

	//Set the player back to not touching ground or walls by default, 
	//so that the player will drop if they aren't standing on the ground.
	//Has to be before update for level spawing so they don't just drop through a platform has they spawn in.
	if (m_touchingSurface == true)
		m_touchingSurface = false;
	if (m_touchingWall == true)
		m_touchingWall = false;

	//If the game has started, down down from 60 seconds
	if (m_gameStart == true)
	{
		m_timerCountdown -= _frameTime.asSeconds();
	}
	// A a potion key has been pressed, give a 0.5 second delay before the player can select an element again.
	if (m_keyBeenPressed == true)
	{
		m_keyDelay -= _frameTime.asSeconds();
	}
	//If a potion has been thrown, give a 3 second delay before the player can throw a potion again.
	if (m_potionBeenThrown == true)
	{
		m_potionDelay -= _frameTime.asSeconds();
	}
	// If the walking sound started, count down to the next interval when the walking sound would be played
	if (m_gameSoundStarted == true)
	{
		m_soundDelay -= _frameTime.asSeconds();
	}
	//If the count down reaches 0, everything is reset for the current level.
	if (m_timerCountdown <= 0)
	{
		m_OutOfTime.play();
		m_gameStart = false;
		m_timerCountdown = 60.0f;
		m_level->ReloadLevel();
		m_ShouldWeCheckMusic = true;
	}

	// The key delay reaches 0, allow the player to use an element again.
	if (m_keyDelay <= 0)
	{
		m_keyBeenPressed = false;
		m_keyDelay = 0.5f;
	}
	// The potion delay reaches 0, allow the player to throw a potion.
	if (m_potionDelay <= 0)
	{
		m_potionBeenThrown = false;
		m_potionDelay = 3.0f;
	}

	////////////////
	// HYDROGEN ////
	////////////////
	// If hydrogen IS active, slowing move the player upwards until it is turned off //
	if (potionState == Hydrogen)
	{
		m_velocity.y = HydrogenFloat;
	}

	///////////////
	/// MUSIC /////
	///////////////
	if (m_ShouldWeCheckMusic == true)
	{
		m_level->MusicPlayer();
		m_ShouldWeCheckMusic = false;
	}
	if (m_gameStart == true && m_gameStartBeenChecked == false)
	{
		m_level->MusicPlayer();
		m_gameStartBeenChecked = true;
	}

	//// Call the update function manually on 
	//// the parent class
	//// This will actually move the character
	GridObject::Update(_frameTime);
}

void Player::Collide(GameObject& _collider)
{

	///////////////////
	// DYNAMIC CASTS //
	///////////////////
	Wall* wallCollider = dynamic_cast<Wall*>(&_collider);
	Spike* spikeCollider = dynamic_cast<Spike*>(&_collider);
	Box* boxCollider = dynamic_cast<Box*>(&_collider);
	Killzone* killzoneCollider = dynamic_cast<Killzone*>(&_collider);
	Exit* exitCollider = dynamic_cast<Exit*>(&_collider);
	CrackedWall* crackedCollider = dynamic_cast<CrackedWall*>(&_collider);
	WaterDown* waterCollider = dynamic_cast<WaterDown*>(&_collider);

	//////////////////////////////////////////////////////////////////////////////////////////
	// PLAYER ////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	//Get the collider for the player
	sf::FloatRect playerCollider = m_sprite.getGlobalBounds();

	// Create feet collider
	sf::FloatRect feetCollider = playerCollider;
	// Set our feet top to be 10 pixels from the bottom of the player collider
	feetCollider.top += playerCollider.height - 5;
	// Set our feet collider height to be 5 pixels
	feetCollider.height = 5;
	// Shorten the width to not mess with the other colliders
	feetCollider.width -= 10;
	feetCollider.left += 5;

	// Create head collider
	sf::FloatRect headCollider = playerCollider;
	// Set our head top to be 5 pixels from the top of the player collider
	headCollider.top += 5;
	// Set our head collider height to be 5 pixels
	headCollider.height = 5;
	// Shorten the width to not mess with the other colliders
	headCollider.width -= 5;
	headCollider.left += 2.5;

	//Create a collider for the right hand side of the player
	sf::FloatRect rightCollider = playerCollider;
	//Set it to the right of the player - 10
	rightCollider.left += playerCollider.width - 10;
	// Set our right side collider width to be 5 pixels
	rightCollider.width = 5;
	// Shorten the height as to not mess with the other colliders
	rightCollider.height -= 10;

	//Create a collider for the right hand side of the player
	sf::FloatRect leftCollider = playerCollider;
	// Set our head collider height to be 5 pixels
	leftCollider.height -= 4;
	leftCollider.top += 2;
	// Set our left side collider width to be 5 pixels
	leftCollider.width = 5;
	// Need to remove the gap between the player and the wall
	leftCollider.left += 5;

	//////////////////////////////////////////////////////////////////////////////////////////
	// WALLS /////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	//Record whether we used to be touching the ground
	bool wereTouchingSurface = m_touchingSurface;
	//Assume we did not collide
	m_touchingSurface = false;
	//Record whether we used to be touching a wall
	bool wereTouchingWall = m_touchingWall;
	//Assume we did not collide
	m_touchingWall = false;

	// Only do something if the thing
	// we touched was a wall

	// If it was a wall we hit, we need to more ourselves
	// outside the wall's bounds, aka back where we were
	if (wallCollider != nullptr)
	{
		// Create platform top collider
		sf::FloatRect platformTop = wallCollider->GetBounds();
		platformTop.height = 10;

		// Create platform bottom collider
		sf::FloatRect platformBottom = wallCollider->GetBounds();
		//Set it to the bottom of the platform - 5
		platformBottom.top += wallCollider->GetBounds().height - 5;
		platformBottom.height = 10;
		// Shorten the width to not mess with the other colliders
		platformBottom.width -= 10;
		platformBottom.left += 5;

		//Create a collider for the right hand side of the wall
		sf::FloatRect wallRight = wallCollider->GetBounds();
		//Set it to the right of the platform -5
		wallRight.left += wallRight.width - 5;
		// Set our right side collider width to be 5 pixels
		wallRight.width = 5;
		// Shorten the height to not mess with the other colliders
		wallRight.height -= 10;
		wallRight.top += 5;

		//Create a collider for the left hand side of the platform
		sf::FloatRect wallLeft = wallCollider->GetBounds();
		// Set our left side collider width to be 5 pixels
		wallLeft.width = 5;
		// Shorten the height to not mess with the other colliders
		wallLeft.height -= 10;
		wallLeft.top += 5;

		if (m_hasTouchedWaterLeft == true)
		{
			//Create a collider for the wall.
			sf::FloatRect wallElectricKnockback = wallCollider->GetBounds();

			if (leftCollider.intersects(wallElectricKnockback))
			{
				// The player has merged with the right side of a wall, send them forward 2 cells
				m_sprite.setPosition(wallCollider->getPosition().x - wallCollider->GetBounds().width * 2, m_sprite.getPosition().y);
				m_hasTouchedWaterLeft = false;
			}
		}

		// If when the player jumps using electricity, we should check if they would be in a wall.
		if (m_hasTouchedWaterRight == true)
		{
			//Create a collider for the wall.
			sf::FloatRect wallElectricKnockback = wallCollider->GetBounds();

			if (rightCollider.intersects(wallElectricKnockback))
			{
				// The player has merged with the left side of a wall, send them back 2 cells
				m_sprite.setPosition(wallCollider->getPosition().x + wallCollider->GetBounds().width * 2, m_sprite.getPosition().y);
				m_hasTouchedWaterRight = false;
			}
		}

		// Are the feet touching the top of the platform?
		if (feetCollider.intersects(platformTop))
		{
			// We are now touching the ground!
			m_touchingSurface = true;

			//We have touched the ground
			m_velocity.y = 0;
			m_sprite.setPosition(m_sprite.getPosition().x, wallCollider->getPosition().y - m_sprite.getGlobalBounds().height);

			// If the game hasn't sent the player back from being inside a wall, then we no longer need to check if they have touched water
			m_hasTouchedWaterLeft = false;
			m_hasTouchedWaterRight = false;
		}

		// Is the head touching the bottom of the platform?
		if (headCollider.intersects(platformBottom))
		{
			// We are now touching the ground!
			// Is the player's active element collagen?

			if (potionState == Collagen)
			{
				//The player is no longer able to move up and isn't affected by gravity
				m_touchingSurface = true;

				//Yes it is, sticky movement
				//We have touched the roof
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, wallCollider->getPosition().y + wallCollider->GetBounds().height); //FOR THE WATER INVERT THESE VALUES //
			}
			else
			{
				//Do not reset the player's ability to move up
				m_touchingSurface = false;
				// No it is not, normal movement
				//Check if we are jumping
				if (wereTouchingSurface == false && m_velocity.y < 0)
				{
					//We have touched the roof
					m_velocity.y = 0;
					m_sprite.setPosition(m_sprite.getPosition().x, wallCollider->getPosition().y + wallCollider->GetBounds().height);
				}
			}
		}

		if (rightCollider.intersects(wallLeft))
		{
			m_touchingWall = true;

			//// Is the player's active element collagen?

			if (potionState == Collagen)
			{
				// Yes it is, sticky movement
				//Reset the player's ability to move up
				m_touchingSurface = true;

				//if (wereTouchingWall == false && m_velocity.x >= 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			}
			else
			{
				//Do not reset the player's ability to move up
				m_touchingSurface = false;
				// No it is not, normal movement

					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
			}
		}

		if (leftCollider.intersects(wallRight))
		{
			m_touchingWall = true;
			
			// Is the player's active element collagen?

			if (potionState == Collagen)
			{
				//Reset the player's ability to move up
				m_touchingSurface = true;
				// Yes it is, sticky movement
				//if (wereTouchingWall == false && m_velocity.x <= 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			}
			else
			{
				//Do not reset the player's ability to move up
				m_touchingSurface = false;
				// No it isn't, normal movement
				m_velocity.x = 0;
				m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// BOXES /////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	if (boxCollider != nullptr)
	{
		// We did hit a box, now we need to move it based on the velocity of the player!

			// Create platform top collider
			sf::FloatRect platformTop = boxCollider->GetBounds();
			platformTop.height = 10;

			// Create platform bottom collider
			sf::FloatRect platformBottom = boxCollider->GetBounds();
			//Set it to the bottom of the platform - 5
			platformBottom.top += boxCollider->GetBounds().height - 5;
			platformBottom.height = 10;
			// Shorten the width to not mess with the other colliders
			platformBottom.width -= 10;
			platformBottom.left += 5;

			//Create a collider for the right hand side of the platform
			sf::FloatRect platformRight = boxCollider->GetBounds();
			//Set it to the right of the platform -5
			platformRight.left += platformRight.width - 5;
			// Set our right side collider width to be 5 pixels
			platformRight.width = 5;
			// Shorten the height to not mess with the other colliders
			platformRight.height -= 10;
			platformRight.top += 5;

			//Create a collider for the left hand side of the platform
			sf::FloatRect platformLeft = boxCollider->GetBounds();
			// Set our left side collider width to be 5 pixels
			platformLeft.width = 5;
			// Shorten the height to not mess with the other colliders
			platformLeft.height -= 10;
			platformLeft.top += 5;

			// Are the feet touching the top of the platform?
			if (feetCollider.intersects(platformTop))
			{
				// We are now touching the ground!
				m_touchingSurface = true;

				//We have touched the ground
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, boxCollider->getPosition().y - m_sprite.getGlobalBounds().height);
			}

			// Is the head touching the bottom of the platform?
			if (headCollider.intersects(platformBottom))
			{
				// We are now touching the ground!
				//Do not reset the player's ability to move up
				m_touchingSurface = false;
				// No it is not, normal movement
				//Check if we are jumping
				if (wereTouchingSurface == false && m_velocity.y < 0)
				{
					//We have touched the roof
					m_velocity.y = 0;
					m_sprite.setPosition(m_sprite.getPosition().x, boxCollider->getPosition().y + boxCollider->GetBounds().height);
				}
			}

			if (rightCollider.intersects(platformLeft))
			{
				m_touchingWall = true;

				// The target position we want to move the box towards is to the right 1 cell
				sf::Vector2i targetPos(1, 0);

					// Attempt to push!
					bool pushSucceeded = boxCollider->AttemptPush(targetPos);

					//Do not reset the player's ability to move up
					m_touchingSurface = false;
					// No it is not, normal movement
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
			}

			if (leftCollider.intersects(platformRight))
			{
				m_touchingWall = true;

				// The target position we want to move the box towards is to the left 1 cell
				sf::Vector2i targetPos(-1, 0);

				//Do not reset the player's ability to move up
				m_touchingSurface = false;
				// Attempt to push!
				bool pushSucceeded = boxCollider->AttemptPush(targetPos);

				// No it isn't, normal movement
				m_velocity.x = 0;
				m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
			}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// EXIT //////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	if (exitCollider != nullptr)
	{
		// We hit the exit, meaning that the player has beaten the level, reset and load next level.
			m_gameStart = false;
			m_timerCountdown = 60.0f;
			m_level->LoadNextLevel();
			m_ShouldWeCheckMusic = true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// KILLZONE //////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	if (killzoneCollider != nullptr)
	{
		// We did hit a killzone! Kill the player
		m_level->ReloadLevel();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// SPIKES ////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	
	if (spikeCollider != nullptr)
	{
		// Create platform top collider
		sf::FloatRect spikeTipTopOrBottom = spikeCollider->GetBounds();
		spikeTipTopOrBottom.width = 10;

		//Create a collider for the sides of the spike
		sf::FloatRect SpikeTipSides = spikeCollider->GetBounds();
		// Set the rect to the middle of the spikes
		SpikeTipSides.height = (SpikeTipSides.height / 2) - 10;
		// Shorten the height for the +
		SpikeTipSides.height = 10;


		// Are the feet touching the top of the platform?
		if (feetCollider.intersects(spikeTipTopOrBottom))
		{
			m_OutOfTime.play();
			m_level->ReloadLevel();
			m_ShouldWeCheckMusic = true;
		}

		// Is the head touching the bottom of the platform?
		if (headCollider.intersects(spikeTipTopOrBottom))
		{
			m_OutOfTime.play();
			m_level->ReloadLevel();
			m_ShouldWeCheckMusic = true;
		}

		if (rightCollider.intersects(SpikeTipSides))
		{
			m_OutOfTime.play();
			m_level->ReloadLevel();
			m_ShouldWeCheckMusic = true;
		}

		if (leftCollider.intersects(SpikeTipSides))
		{
			m_OutOfTime.play();
			m_level->ReloadLevel();
			m_ShouldWeCheckMusic = true;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// CRACKED WALL //////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////

	if (crackedCollider != nullptr)
	{
		// Create platform top collider
		sf::FloatRect platformTop = crackedCollider->GetBounds();
		platformTop.height = 10;

		// Create platform bottom collider
		sf::FloatRect platformBottom = crackedCollider->GetBounds();
		//Set it to the bottom of the platform - 5
		platformBottom.top += crackedCollider->GetBounds().height - 5;
		platformBottom.height = 10;
		// Shorten the width to not mess with the other colliders
		platformBottom.width -= 10;
		platformBottom.left += 5;

		//Create a collider for the right hand side of the platform
		sf::FloatRect platformRight = crackedCollider->GetBounds();
		//Set it to the right of the platform -5
		platformRight.left += platformRight.width - 5;
		// Set our right side collider width to be 5 pixels
		platformRight.width = 5;

		//Create a collider for the left hand side of the platform
		sf::FloatRect platformLeft = crackedCollider->GetBounds();
		// Set our left side collider width to be 5 pixels
		platformLeft.width = 5;

		// Are the feet touching the top of the platform?
		if (feetCollider.intersects(platformTop))
		{
			// We are now touching the ground!
			m_touchingSurface = true;

			//Check if we are falling downward
			if (wereTouchingSurface == false && m_velocity.y > 0)
			{
				//We have touched the ground
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, crackedCollider->getPosition().y - m_sprite.getGlobalBounds().height);
			}

		}

		// Is the head touching the bottom of the platform?
		if (headCollider.intersects(platformBottom))
		{
			// We are now touching the ground!

			//Do not reset the player's ability to move up
			m_touchingSurface = false;
			// No it is not, normal movement
			//Check if we are jumping
			if (wereTouchingSurface == false && m_velocity.y < 0)
			{
				//We have touched the roof
				m_velocity.y = 0;
				m_sprite.setPosition(m_sprite.getPosition().x, crackedCollider->getPosition().y + crackedCollider->GetBounds().height);
			}
		}

		if (rightCollider.intersects(platformLeft))
		{
			m_touchingWall = true;

			//Do not reset the player's ability to move up
			m_touchingSurface = false;
			// No it is not, normal movement
			if (wereTouchingWall == false && m_velocity.x > 0)
			{
				m_velocity.x = 0;
				m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
			}
			
		}

		if (leftCollider.intersects(platformRight))
		{
			m_touchingWall = true;

			//Do not reset the player's ability to move up
			m_touchingSurface = false;
			// No it isn't, normal movement
			if (wereTouchingWall == false && m_velocity.x < 0)
			{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// WATER /////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	
	if (waterCollider != nullptr)
	{
		// Create platform top collider
		sf::FloatRect platformTop = waterCollider->GetBounds();
		platformTop.height = 10;

		// Create platform bottom collider
		sf::FloatRect platformBottom = waterCollider->GetBounds();
		//Set it to the bottom of the platform - 5
		platformBottom.top += waterCollider->GetBounds().height - 5;
		platformBottom.height = 10;
		// Shorten the width to not mess with the other colliders
		platformBottom.width -= 10;
		platformBottom.left += 5;

		//Create a collider for the right hand side of the platform
		sf::FloatRect platformRight = waterCollider->GetBounds();
		//Set it to the right of the platform -5
		platformRight.left += platformRight.width - 5;
		// Set our right side collider width to be 5 pixels
		platformRight.width = 5;

		//Create a collider for the left hand side of the platform
		sf::FloatRect platformLeft = waterCollider->GetBounds();
		// Set our left side collider width to be 5 pixels
		platformLeft.width = 5;

		// Are the feet touching the top of the platform?
		if (feetCollider.intersects(platformTop))
		{
			// We are now touching the ground!
			m_touchingSurface = true;

			//We have touched the ground
			m_velocity.y = 0;
			m_sprite.setPosition(m_sprite.getPosition().x, waterCollider->getPosition().y - m_sprite.getGlobalBounds().height);
		}

		if (rightCollider.intersects(platformLeft))
		{
			m_touchingWall = true;

			//// Is the player's active element Electricity?

			if (potionState == Electricity)
			{
				// If electricity is active and the player touches water, push them to the right until they no longer are touching water (zap movement)

				m_WaterWalk.play();
				m_velocity.x = 0;
				m_sprite.setPosition(waterCollider->getPosition().x + waterCollider->GetBounds().width, m_sprite.getPosition().y);

				// To prevent the player from flickering between which side they have touched, just check that the opposite is not true.
				if (m_hasTouchedWaterRight == false)
				{
					m_hasTouchedWaterLeft = true;
					m_hasTouchedWaterRight = false;
				}
			}
			else
			{
				//Do not reset the player's ability to move up
				m_touchingSurface = false;
				// No it is not, normal movement
				if (wereTouchingWall == false && m_velocity.x > 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			}
		}

		if (leftCollider.intersects(platformRight))
		{
			m_touchingWall = true;

			// Is the player's active element electricity?

			if (potionState == Electricity)
			{
					m_WaterWalk.play();
					// If electricity is active and the player touches water, push them to the left until they no longer are touching water (zap movement)
					m_velocity.x = 0;
					m_sprite.setPosition(waterCollider->getPosition().x - waterCollider->GetBounds().width + 30, m_sprite.getPosition().y);

					// To prevent the player from flickering between which side they have touched, just check that the opposite is not true.
					if (m_hasTouchedWaterLeft == false)
					{
						m_hasTouchedWaterRight = true;
						m_hasTouchedWaterLeft = false;
					}
			}
			else
			{
				//Do not reset the player's ability to move up
				m_touchingSurface = false;
				// No it isn't, normal movement
				if (wereTouchingWall == false && m_velocity.x < 0)
				{
					m_velocity.x = 0;
					m_sprite.setPosition(m_previousPosition.x, m_sprite.getPosition().y);
				}
			}
		}
	}

}

int Player::GetTimer()
{
	return m_timerCountdown;
}

int Player::GetDelay()
{
	return m_potionDelay;
}

int Player::GetPotionState()
{
	return m_numericalPotionState;
}

bool Player::GetGameStart()
{
	return m_gameStart;
}

bool Player::GetPlayerDirectionRight()
{
	// If the player is facing the right this will be true.
	return m_isPlayerFacingRight;
}

bool Player::GetPlayerDirectionDown()
{
	// If the player is pressing down this will be true.
	return m_isPlayerThrowingDown;
}

void Player::ChangeTimer(int _changeBy)
{
	m_timerCountdown += _changeBy;
}

void Player::ChangeDelay(int _changeBy)
{
	m_potionDelay += _changeBy;
}

void Player::SetPotion(Potion* _potion)
{
	m_potion = _potion;
}
