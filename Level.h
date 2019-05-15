#pragma once

// Project Includes
#include "GridObject.h"

// Library Includes
#include <SFML/Graphics.hpp>

class Level
{
public:

	Level();

	void Draw(sf::RenderTarget& _target);
	void Update(sf::Time _frameTime);
	void Input(sf::Event _gameEvent);

	void LoadLevel(int _levelToLoad);
	void ReloadLevel();
	void LoadNextLevel();

	void SetCurrentScore();

	float GetCellSize();
	bool LevelComplete();
	bool MoveObjectTo(GridObject* _toMove, sf::Vector2i _targetPos);
	bool deleteObjectAt(GridObject* _toDelete, sf::Vector2i _targetLocation);
	std::vector<GridObject*> GetObjectAt(sf::Vector2i _targetPos);

private:

	const float m_cellSize;
	int m_currentLevel;
	int m_levelDiamonds;
	int m_currentScore;
	int m_pendingLoad;
	bool m_pendingReload;
	GameObject* m_player;
	GameObject* m_timer;

	// GameObject Lists
	std::vector<std::pair<GameObject*, GameObject*> > m_collisionList;
	std::vector< std::vector <std::vector<GridObject*> > > m_contents;
	std::vector< std::vector <sf::Sprite> > m_background;

	//State
	sf::Vector2f m_velocity;
	bool m_hasGravity;
	bool m_touchingGround;
};