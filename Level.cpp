// Project Includes
#include "Level.h"
#include "Framework/AssetManager.h"

//Project Includes (Objects)
#include "Wall.h"
#include "Player.h"
#include "Box.h"
#include "Diamond.h"
#include "Exit.h"
#include "Spike.h"
#include "Killzone.h"
#include "Timer.h"
#include "WaterDown.h"
#include "WaterSide.h"
#include "CrackedWall.h"
#include "Delay.h"

// Project Includes (Elemental Effects)
#include "Collagen.h"
#include "Hydrogen.h"
#include "Sulphur.h"
#include "Electricity.h"
#include "Potion.h"

// Library Includes
#include <iostream>
#include <fstream>
#include <SFML/Audio.hpp>

Level::Level()
	: m_cellSize(64.0f)
	, m_currentLevel(0)
	, m_background()
	, m_contents()
	, m_updateList()
	, m_drawListUI()
	, m_levelDiamonds(0)
	, m_pendingLoad()
	, m_pendingReload(false)
	, m_velocity(0.0f, 0.0f)
	, m_hasGravity(false)
	, m_hasPlayed(false)
	, m_touchingGround(false)
	, m_player(nullptr)
	, m_potion(nullptr)
	, m_cracked(nullptr)
	, m_wall(nullptr)
	, m_timer(nullptr)
	, m_delay(nullptr)
	, m_collisionList()
{
	LoadLevel(1);

	/////////////////
	/// MUSIC ///////
	/////////////////
	LevelViewTheme.openFromFile("audio/05ComeandFindMe.ogg");
	LevelPlayTheme.openFromFile("audio/Jumpshot.ogg");
	FinalLevelViewTheme.openFromFile("audio/09ComeandFindMe-Bmix.ogg");
	FinalLevelPlayTheme.openFromFile("audio/07We'retheResistors.ogg");
}

void Level::Draw(sf::RenderTarget & _target)
{

	// Create and update the camera
	sf::View camera = _target.getDefaultView();
	camera.setCenter(m_levelSizeX / 2, m_levelSizeY / 2);

	if (m_player->GetGameStart() == true)
	{
		//If the game has begun, set the camera to follow the player.
		camera.setCenter(m_player->getPosition());
	}
	else 
	{
		camera.zoom(m_levelZoom);
	}

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

	// Draw the potion upon update //
	if (m_potion)
	{
		m_potion->Draw(_target);
	}

	// Reset View
	_target.setView(_target.getDefaultView());
	
	// Draw UI objects
	for (int i = 0; i < m_drawListUI.size(); ++i)
	{
		if (m_drawListUI[i]->IsActive())
			m_drawListUI[i]->Draw(_target);
	}
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

	// Update all game objects
	for (int i = 0; i < m_updateList.size(); ++i)
	{
		if (m_updateList[i]->IsActive())
			m_updateList[i]->Update(_frameTime);
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
	// Handler is the thing moving, handler is the thing moved into
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


	// Update the potion to allow it to draw //
	if (m_potion != nullptr)
	{
		m_potion->Update(_frameTime);
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
	// Reset the X and Y setters for the camera
	m_levelSizeX = 0;
	m_levelSizeY = 0;
	m_levelZoom = 1;

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

	// Delete UI data already in the level
	for (int i = 0; i < m_updateList.size(); ++i)
	{
		delete m_updateList[i];
	}
	
	// Clear out our lists
	m_background.clear();
	m_updateList.clear();
	m_contents.clear();
	m_drawListUI.clear();
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

		// Create the player first as other objects will need to reference it (Collisions)
		Player* player = new Player();
		m_player = player;

		// Create lists of all objects being created by loading the level (for multi object collision)
		std::vector<GameObject*> walls;
		std::vector<GameObject*> boxes;
		std::vector<GameObject*> crackedWalls;
		std::vector<GameObject*> spikes;

		// Since we only want the camera's m_levelSizeX tracker to work for the first row, we need to prevent it from running on subsequent rows
		bool reachedSecondRow = false;

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

				//Everytime we take a new line, change where the camera centres on and the depth of the zoom
				m_levelSizeY += m_cellSize;
				m_levelZoom += 0.07f;
				reachedSecondRow = true;

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

				if (reachedSecondRow == false) 
				{
					m_levelSizeX += m_cellSize;
				}

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
					walls.push_back(wall);
				}
				else if (ch == 'P')
				{
					player->SetLevel(this);
					player->SetGridPosition(x, y);
					m_contents[y][x].push_back(player);
					m_player = player;
				}
				else if (ch == 'B')
				{
					Box* box = new Box();
					box->SetLevel(this);
					box->SetGridPosition(x, y);
					m_contents[y][x].push_back(box);
					m_collisionList.push_back(std::make_pair(player, box));
					boxes.push_back(box);
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
					m_collisionList.push_back(std::make_pair(player, exit));
				}
				else if (ch == 'S')
				{
					Spike* spike = new Spike();
					spike->SetLevel(this);
					spike->SetGridPosition(x, y);
					m_contents[y][x].push_back(spike);
					m_collisionList.push_back(std::make_pair(player, spike));
					spikes.push_back(spike);
				}
				else if (ch == 'K')
				{
					Killzone* killzone = new Killzone();
					killzone->SetLevel(this);
					killzone->SetGridPosition(x, y);
					m_contents[y][x].push_back(killzone);
					m_collisionList.push_back(std::make_pair(player, killzone));
				}
				else if (ch == '1')
				{
					WaterDown* waterdown = new WaterDown();
					waterdown->SetLevel(this);
					waterdown->SetGridPosition(x, y);
					m_contents[y][x].push_back(waterdown);
					m_collisionList.push_back(std::make_pair(player, waterdown));
				}
				else if (ch == '2')
				{
					WaterSide* waterside = new WaterSide();
					waterside->SetLevel(this);
					waterside->SetGridPosition(x, y);
					m_contents[y][x].push_back(waterside);
					m_collisionList.push_back(std::make_pair(player, waterside));
				}
				else if (ch == 'F')
				{
					CrackedWall* crackedwall = new CrackedWall();
					crackedwall->SetLevel(this);
					crackedwall->SetGridPosition(x, y);
					m_contents[y][x].push_back(crackedwall);
					m_collisionList.push_back(std::make_pair(player, crackedwall));
					crackedWalls.push_back(crackedwall);
				}
				else
				{
					std::cerr << "Unrecognised character in level file: " << ch;
				}
			}
		}

		// This is the code that allows collision between all boxes and all walls.
		for (int i = 0; i < boxes.size(); ++i)
		{
			// Each each box in this array...
			GameObject* thisParticularBox = boxes[i];

			for (int j = 0; j < walls.size(); ++j)
			{
				// ...and each wall in this array...
				GameObject* thisParticularWall = walls[j];

				// Create a pairing between the two.
				m_collisionList.push_back(std::make_pair(thisParticularBox, thisParticularWall));
			}
		}

		// This is the code that allows collision between boxes and other boxes
		for (int i = 0; i < boxes.size(); ++i)
		{
			// Each each box in this array...
			GameObject* thisBoxSet1 = boxes[i];

			for (int j = 0; j < boxes.size(); ++j)
			{
				// ...and each box in the same array...
				GameObject* thisBoxSet2 = boxes[j];

				// Create a pairing between the two.
				m_collisionList.push_back(std::make_pair(thisBoxSet1, thisBoxSet2));
			}
		}

		// This is the code that allows collision between boxes and spikes
		for (int i = 0; i < boxes.size(); ++i)
		{
			// Each each box in this array...
			GameObject* thisParticularBox = boxes[i];

			for (int j = 0; j < spikes.size(); ++j)
			{
				// ...and each box in the same array...
				GameObject* thisParticularSpike = spikes[j];

				// Create a pairing between the two.
				m_collisionList.push_back(std::make_pair(thisParticularBox, thisParticularSpike));
			}
		}

		// This is the code that allows collision between boxes and cracked walls
		for (int i = 0; i < boxes.size(); ++i)
		{
			// Each each box in this array...
			GameObject* thisParticularBox = boxes[i];

			for (int j = 0; j < crackedWalls.size(); ++j)
			{
				// ...and each box in the same array...
				GameObject* thisParticularCrackedWall = crackedWalls[j];

				// Create a pairing between the two.
				m_collisionList.push_back(std::make_pair(thisParticularBox, thisParticularCrackedWall));
			}
		}

		// UI Objects that are now drawn via level character scripts
		Timer* ourTimer = new Timer();
		ourTimer->SetPlayer(player);
		m_updateList.push_back(ourTimer);
		m_drawListUI.push_back(ourTimer);

		Delay* delay = new Delay();
		delay->SetPlayer(player);
		m_updateList.push_back(delay);
		m_drawListUI.push_back(delay);

		Collagen* collagen = new Collagen();
		collagen->SetPlayer(player);
		m_drawListUI.push_back(collagen);
		m_updateList.push_back(collagen);

		Hydrogen* hydrogen = new Hydrogen();
		hydrogen->SetPlayer(player);
		m_drawListUI.push_back(hydrogen);
		m_updateList.push_back(hydrogen);

		Sulphur* sulphur = new Sulphur();
		sulphur->SetPlayer(player);
		m_drawListUI.push_back(sulphur);
		m_updateList.push_back(sulphur);

		Electricity* electricity = new Electricity();
		electricity->SetPlayer(player);
		m_drawListUI.push_back(electricity);
		m_updateList.push_back(electricity);

		// Close the file now that we are done with it
		inFile.close();
}

void Level::ReloadLevel()
{
	LoadLevel(m_currentLevel);
	m_hasPlayed = false;
}

void Level::LoadNextLevel()
{
		m_pendingLoad = m_currentLevel + 1;
		m_hasPlayed = false;
}

void Level::CreatePotion()
{
	//Draw the potion for the purpose of throwing
	Potion* potion = new Potion();
	m_potion = potion;
	m_potion->SetLevel(this);
	m_potion->SetPlayer(m_player);
	m_player->SetPotion(m_potion);

	// rows
	for (int y = 0; y < m_contents.size(); ++y)
	{
		// cells
		for (int x = 0; x < m_contents[y].size(); ++x)
		{
			//Sticky outies (grid objects)
			for (int z = 0; z < m_contents[y][x].size(); ++z)
			{
				Wall* wall = dynamic_cast<Wall*>(m_contents[y][x][z]);
				CrackedWall* crackedWall = dynamic_cast<CrackedWall*>(m_contents[y][x][z]);

				if (wall != nullptr)
				{
					m_collisionList.push_back(std::make_pair(m_potion, wall));
				}

				if (crackedWall != nullptr)
				{
					m_collisionList.push_back(std::make_pair(m_potion, crackedWall));
				}
			}
		}
	}
}

void Level::MusicPlayer()
{
	/////////////////
	/// MUSIC ///////
	/////////////////

	// Set up the play conditions of the music
	if (m_player->GetGameStart() == false && m_currentLevel <= 4)
	{
		LevelPlayTheme.stop();
		LevelViewTheme.play();
		LevelViewTheme.setLoop(true);
	}
	if (m_player->GetGameStart() == true && m_currentLevel <= 4)
	{
		LevelViewTheme.stop();
		LevelPlayTheme.play();
		LevelPlayTheme.setLoop(true);
	}
	if (m_player->GetGameStart() == false && m_currentLevel >= 5)
	{
		LevelViewTheme.stop();
		LevelPlayTheme.stop();
		FinalLevelPlayTheme.stop();
		FinalLevelViewTheme.play();
		FinalLevelViewTheme.setLoop(true);
	}
	if (m_player->GetGameStart() == true && m_currentLevel >= 5)
	{
		LevelViewTheme.stop();
		LevelPlayTheme.stop();
		FinalLevelViewTheme.stop();
		FinalLevelPlayTheme.play();
		FinalLevelPlayTheme.setLoop(true);
	}
}

bool Level::SetHasPlayed(bool _hasPlayed)
{
	m_hasPlayed = _hasPlayed;
	return m_hasPlayed;
}

float Level::GetCellSize()
{
	return m_cellSize;
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

void Level::deletePotionAt(GridObject * _toDelete)
{
	if (_toDelete != nullptr)
	{
		if (m_potion != nullptr)
		{
			delete _toDelete;
			m_potion = nullptr;
		}
		//Find the object in the list using an iterator and the find method
		for (auto it = m_collisionList.begin(); it != m_collisionList.end(); )
		{
			// if second thing in pair is the "to be deleted" object, then, delete the pair
			if (it->first == _toDelete)
			{
				it = m_collisionList.erase(it); // returns pointer to next thing in list, so we don't want to add to it ourselves
			}
			else
				++it; // we didnt delete so add to it to go to the next thing in list
		}
	}
}

void Level::deleteObjectAt(GridObject * _toDelete)
{
	if (_toDelete != nullptr)
	{
		// Get the current position of the grid object
		sf::Vector2i oldPos = _toDelete->GetGridPosition();

		// If we found the object at this location,
		// it will NOT equal the end of the vector

		// Find the object in the list using an iterator
		// and find the method
		// Only run this if we know we aren't looking for a potion.
		auto it = std::find(m_contents[oldPos.y][oldPos.x].begin(),
			m_contents[oldPos.y][oldPos.x].end(),
			_toDelete);

		// If we found the object at this location,
		// it will NOT equal the end of the vector
		if (it != m_contents[oldPos.y][oldPos.x].end())
		{
			// We found the object!
			delete *it;

			// Remove it from the old position
			m_contents[oldPos.y][oldPos.x].erase(it);
		}

		//Find the object in the list using an iterator and the find method
		for (auto it = m_collisionList.begin(); it != m_collisionList.end(); )
		{
			// if second thing in pair is the "to be deleted" object, then, delete the pair
			if (it->second == _toDelete)
				it = m_collisionList.erase(it); // returns pointer to next thing in list, so we don't want to add to it ourselves
			else
				++it; // we didnt delete so add to it to go to the next thing in list
		}
	}
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