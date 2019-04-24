// Project Includes
#include "Level.h"
#include "Framework/AssetManager.h"
#include "Wall.h"
#include "Player.h"
#include "Dirt.h"
#include "Box.h"
#include "Diamond.h"
#include "Exit.h"
#include "Spike.h"

// Library Includes
#include <iostream>
#include <fstream>

Level::Level()
	: m_cellSize(64.0f)
	, m_currentLevel(0)
	, m_background()
	, m_contents()
	, m_currentScore(0)
	, m_levelDiamonds(0)
	, m_pendingLoad()
	, m_pendingReload(false)
	, m_velocity(0.0f, 0.0f)
	, m_hasGravity(false)
	, m_touchingGround(false)
	, m_player(nullptr)
	, m_collisionList()
{
	LoadLevel(1);
}

void Level::Draw(sf::RenderTarget & _target)
{
	// Create and update
	sf::View camera = _target.getDefaultView();

	//Adjust camera as needed
    camera.setCenter(m_player->getPosition());

	// Draw game world to the window
	_target.setView(camera);

	// Draw game objects
	for (int y = 0; y < m_background.size(); ++y)
	{
		for (int x = 0; x < m_background[y].size(); ++x)
		{
			_target.draw(m_background[y][x]);
		}
	}

	// rows
	for (int y = 0; y < m_contents.size(); ++y)
	{
		// cells
		for (int x = 0; x < m_contents[y].size(); ++x)
		{
			//Sticky outies (grid objects)
			for (int z = 0; z < m_contents[y][x].size(); ++z)
			{
				m_contents[y][x][z]->Draw(_target);
			}
		}
	}

	// Reset View
	_target.setView(_target.getDefaultView());
}

void Level::Update(sf::Time _frameTime)
{
	// rows
	for (int y = 0; y < m_contents.size(); ++y)
	{
		// cells
		for (int x = 0; x < m_contents[y].size(); ++x)
		{
			//Sticky outies (grid objects)
			for (int z = 0; z < m_contents[y][x].size(); ++z)
			{
				m_contents[y][x][z]->Update(_frameTime);
			}
		}
	}

	// If the player needs to reload
	if (m_pendingReload == true) 
	{
		// Reload level
		LoadLevel(m_currentLevel);

		//Set pending reload to false
		m_pendingReload = false;
	}

	// If the player needs to reload
	if (m_pendingLoad != 0)
	{
		// load new level
		LoadLevel(m_pendingLoad);

		//Set pending load to false
		m_pendingLoad = 0;
	}

	// Collision detection
	for (int i = 0; i < m_collisionList.size(); ++i)
	{

		GameObject* handler = m_collisionList[i].first;
		GameObject* collider = m_collisionList[i].second;

		if (handler->IsActive() && collider->IsActive())
		{
			if (handler->GetBounds().intersects(collider->GetBounds()))
			{
				handler->Collide(*collider);
			}
		}
	}
}

void Level::Input(sf::Event _gameEvent)
{
	// rows
	for (int y = 0; y < m_contents.size(); ++y)
	{
		// cells
		for (int x = 0; x < m_contents[y].size(); ++x)
		{
			//Sticky outies (grid objects)
			for (int z = 0; z < m_contents[y][x].size(); ++z)
			{
				m_contents[y][x][z]->Input(_gameEvent);
			}
		}
	}
	

	/////////////////////////////////////////////////////
	//////////////DEV COMMANDS///////////////////////////
	/////////////////////////////////////////////////////
	if (_gameEvent.type == sf::Event::KeyPressed)
	{
		// Oh a key was pressed, which one was it?
		if (_gameEvent.key.code == sf::Keyboard::BackSpace)
		{
			ReloadLevel();
		}
	}

	if (_gameEvent.type == sf::Event::KeyPressed)
	{
		// Oh a key was pressed, which one was it?
		if (_gameEvent.key.code == sf::Keyboard::Escape)
		{
			exit(0);
		}
	}
	if (_gameEvent.type == sf::Event::KeyPressed)
	{
		// Oh a key was pressed, which one was it?
		if (_gameEvent.key.code == sf::Keyboard::Enter)
		{
			LoadNextLevel();
		}
	}
	///////////////////////////////////////////////////////
	//////////////////////////////////////////////////////
}


void Level::LoadLevel(int _levelToLoad)
{
	// Clean up the old level

	// Delete any data already in the level
	// rows
	for (int y = 0; y < m_contents.size(); ++y)
	{
		// cells
		for (int x = 0; x < m_contents[y].size(); ++x)
		{
			//Sticky outies (grid objects)
			for (int z = 0; z < m_contents[y][x].size(); ++z)
			{
				delete m_contents[y][x][z];
			}
		}
	}
	
	// Clear out our lists
	m_background.clear();
	m_contents.clear();
	m_collisionList.clear();

	// Create a buffer for loading the next level

		// Set the current level
		m_currentLevel = _levelToLoad;

		// Set up the new level

		// Open our file for reading
		std::ifstream inFile;
		std::string fileName = "levels/Level" + std::to_string(m_currentLevel) + ".txt";
		inFile.open(fileName);

		// Make sure the file was opened
		if (!inFile)
		{
			std::cerr << "Unable to open file" + fileName;
			exit(1); // Call System to stop program with error
		}

		// Set the starting xand y coordinates used to position level objects
		int x = 0;
		int y = 0;

		// Create the first row in our grid
		m_background.push_back(std::vector<sf::Sprite>());
		m_contents.push_back(std::vector<std::vector< GridObject*> >());

		// Create the player first as other objects will need to reference it
		Player* player = new Player();
		m_player = player;

		// Read each character one by one from the file...
		char ch;
		// Each time, try to read the next character
		// If successful, execute body of loop
		// the "noskipws" means our input from the file will include the
		// white space (spaces and new lines)
		while (inFile >> std::noskipws >> ch)
		{
			// Perform actions based on what character was read in

			if (ch == ' ')
			{
				++x;
			}
			else if (ch == '\n')
			{
				++y;
				x = 0;

				// Create a new row in our grid
				m_background.push_back(std::vector<sf::Sprite>());
				m_contents.push_back(std::vector<std::vector< GridObject*> >());
			}
			else
			{
				// This is going to be some object (or empty space), so we need a background image
				// Create Background Sprite
				m_background[y].push_back(sf::Sprite(AssetManager::GetTexture("graphics/ground.png")));
				m_background[y][x].setPosition(x*m_cellSize, y*m_cellSize);

				// Create an empty vector for our grid contents in this cell
				m_contents[y].push_back(std::vector<GridObject*>());

				if (ch == '-')
				{
					// Do nothing - Empty space
				}
				else if (ch == 'W')
				{
					Wall* wall = new Wall();
					wall->SetLevel(this);
					wall->SetGridPosition(x, y);
					m_contents[y][x].push_back(wall);
					m_collisionList.push_back(std::make_pair(player, wall));
				}
				else if (ch == 'P')
				{
					player->SetLevel(this);
					player->SetGridPosition(x, y);
					m_contents[y][x].push_back(player);
					m_player = player;
				}
				else if (ch == 'R')
				{
					Dirt* dirt = new Dirt();
					dirt->SetLevel(this);
					dirt->SetGridPosition(x, y);
					m_contents[y][x].push_back(dirt);
				}
				else if (ch == 'B')
				{
					Box* box = new Box();
					box->SetLevel(this);
					box->SetGridPosition(x, y);
					m_contents[y][x].push_back(box);
				}
				else if (ch == 'D')
				{
					Diamond* diamond = new Diamond();
					diamond->SetLevel(this);
					diamond->SetGridPosition(x, y);
					m_contents[y][x].push_back(diamond);
					m_levelDiamonds++;
				}
				else if (ch == 'C')
				{
					Exit* exit = new Exit();
					exit->SetLevel(this);
					exit->SetGridPosition(x, y);
					m_contents[y][x].push_back(exit);
				}
				else if (ch == 'S')
				{
					Spike* spike = new Spike();
					spike->SetLevel(this);
					spike->SetGridPosition(x, y);
					m_contents[y][x].push_back(spike);
					m_collisionList.push_back(std::make_pair(player, spike));
				}
				else
				{
					std::cerr << "Unrecognised character in level file: " << ch;
				}
			}
		}

		m_currentScore = 0;
		// Close the file now that we are done with it
		inFile.close();
}

void Level::ReloadLevel()
{
	m_levelDiamonds = 0;
	LoadLevel(m_currentLevel);
}

void Level::LoadNextLevel()
{
	m_levelDiamonds = 0;
		m_pendingLoad = m_currentLevel + 1;
}

void Level::SetCurrentScore()
{
	m_currentScore++;
}

float Level::GetCellSize()
{
	return m_cellSize;
}

bool Level::LevelComplete()
{
	// Spawn the exit gate if the current score is equal to the number of diamonds that
	// were in the level
	if (m_currentScore == m_levelDiamonds)
	{
		// reset the max diamonds in level (for next level to say how many it has)
		return true;
		
	}
		return false;
}

bool Level::MoveObjectTo(GridObject * _toMove, sf::Vector2i _targetPos)
{
	// Don't trust other code, make sure _toMove is a valid pointer
	// Also check that our target position is within the grid
	if (_toMove != nullptr && _targetPos.y >= 0 && _targetPos.y < m_contents.size()
		&& _targetPos.x >= 0 && _targetPos.x < m_contents[_targetPos.y].size())
	{
		// Get the current position of the grid object
		sf::Vector2i oldPos = _toMove->GetGridPosition();

		// Find the object in the list using an iterator
		// and find the method
		auto it = std::find(m_contents[oldPos.y][oldPos.x].begin(),
			m_contents[oldPos.y][oldPos.x].end(),
			_toMove);

		// If we found the object at this location,
		// it will NOT equal the end of the vector
		if (it != m_contents[oldPos.y][oldPos.x].end())
		{
			//We found the object!

			//Remove it from the old position
			m_contents[oldPos.y][oldPos.x].erase(it);

			// Add it to its new position
			m_contents[_targetPos.y][_targetPos.x].push_back(_toMove);

			// Tell the object it's new position
			_toMove->SetGridPosition(_targetPos);

			// Return success
			return true;
		}
	}

	// return failure
	return false;
}

bool Level::deleteObjectAt(GridObject * _toDelete, sf::Vector2i _targetLocation)
{
	if (_toDelete != nullptr && _targetLocation.y >= 0 && _targetLocation.y < m_contents.size()
		&& _targetLocation.x >= 0 && _targetLocation.x < m_contents[_targetLocation.y].size())
	{
		// Get the current position of the grid object
		sf::Vector2i oldPos = _toDelete->GetGridPosition();

		// Find the object in the list using an iterator
		// and find the method
		auto it = std::find(m_contents[oldPos.y][oldPos.x].begin(),
			m_contents[oldPos.y][oldPos.x].end(),
			_toDelete);

		// If we found the object at this location,
		// it will NOT equal the end of the vector
		if (it != m_contents[oldPos.y][oldPos.x].end())
		{
			//We found the object!

			//Remove it from the old position
			m_contents[oldPos.y][oldPos.x].erase(it);

			// Return success
			return true;
		}
	}

	// return failure
	return false;
}

std::vector<GridObject*> Level::GetObjectAt(sf::Vector2i _targetPos)
{
	// Don't trust the data
	// Make sure the coordinates are within the vector size
	if (_targetPos.y >= 0 && _targetPos.y < m_contents.size()
		&& _targetPos.x >= 0 && _targetPos.x < m_contents[_targetPos.y].size())
	{
		// Get the contents
		// Return them
		return m_contents[_targetPos.y][_targetPos.x];
	}

	// Default return
	// Return an empty vector with nothing in it
	// (default constructor)
	return std::vector<GridObject*>();
}