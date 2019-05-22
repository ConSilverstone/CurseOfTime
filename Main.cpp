// Library Includes
#include <SFML/Window.hpp>	
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <cstdlib>
#include <ctime>

// Project Includes
#include "Framework/AssetManager.h"
#include "Level.h"
#include "Player.h"

// The main() Function - entry point for our program
int main()
{
	// -----------------------------------------------
	// Game Setup
	// -----------------------------------------------

	// Window - to draw to the screen
	sf::RenderWindow gameWindow;
	gameWindow.create(sf::VideoMode::getDesktopMode(), "Curse of Time", sf::Style::Titlebar | sf::Style::Close);

	// Seed our random number generator 
	srand(time(NULL));

	// Create AssetManager
	AssetManager assets;

	// Game Clock - to keep track of time passed each frame
	sf::Clock gameClock;

	// Create the game level
	Level ourLevel;

	// -----------------------------------------------
	// Game Loop
	// -----------------------------------------------
	while (gameWindow.isOpen())
	{
		// -----------------------------------------------
		// Input Section
		// -----------------------------------------------

		// Check all events since the last frame and process 
		// each one until there are no more
		sf::Event gameEvent;
		while (gameWindow.pollEvent(gameEvent))
		{
			//Pass event to input objects
			ourLevel.Input(gameEvent);

			// Did the player try to close the window?
			if (gameEvent.type == sf::Event::Closed)
			{
				// If so, call the close function on the window.
				gameWindow.close();
			}

		} // End event polling loop


		// Set the mouse to the center of the screen to prevent cpu drops from moving it.
		sf::Vector2i mousePosition = sf::Mouse::getPosition(gameWindow);
		sf::Mouse::setPosition(sf::Vector2i(0, 0), gameWindow);

		// -----------------------------------------------
		// Update Section
		// -----------------------------------------------
		// Get the time passed since the last frame and restart our game clock
		sf::Time frameTime = gameClock.restart();

		// Pass update to level
		ourLevel.Update(frameTime);
	
		// -----------------------------------------------
		// Draw Section
		// -----------------------------------------------
		// Clear the window to a single colour
		gameWindow.clear(sf::Color::Black);

		// Pass draw to level
		ourLevel.Draw(gameWindow);

		// Display the window contents on the screen
		gameWindow.display();

	} // End of Game Loop
	return 0;
} // End of main() Function 