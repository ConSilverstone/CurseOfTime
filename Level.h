#pragma once

// Project Includes
#include "GridObject.h"
#include "Player.h"

// Library Includes
#include <SFML/Graphics.hpp>

class Potion;
class Delay;

class Level
{
public:

	Level();

	//General Functions
	void Draw(sf::RenderTarget& _target);
	void Update(sf::Time _frameTime);
	void Input(sf::Event _gameEvent);

	//Level Functions
	void LoadLevel(int _levelToLoad);
	void ReloadLevel();
	void LoadNextLevel();
	void CreatePotion();

	float GetCellSize();
	bool MoveObjectTo(GridObject* _toMove, sf::Vector2i _targetPos);
	bool deleteObjectAt(GridObject* _toDelete);
	std::vector<GridObject*> GetObjectAt(sf::Vector2i _targetPos);

private:

	const float m_cellSize;
	
	// To draw the camera, we need to know how big the level is.
	float m_levelSizeX;
	float m_levelSizeY;
	float m_levelZoom;

	int m_currentLevel;
	int m_levelDiamonds;
	int m_currentScore;
	int m_pendingLoad;
	bool m_pendingReload;
	
	// Class References
	Player* m_player;
	Potion* m_potion;
	Delay* m_delay;
	GameObject* m_wall;
	GameObject* m_timer;

	// GameObject Lists
	std::vector<std::pair<GameObject*, GameObject*> > m_collisionList;
	std::vector< std::vector <std::vector<GridObject*> > > m_contents;
	std::vector< std::vector <sf::Sprite> > m_background;
	std::vector<GameObject*> m_drawListUI;
	std::vector<GameObject*> m_updateList;

	//State
	sf::Vector2f m_velocity;
	bool m_hasGravity;
	bool m_touchingGround;
};