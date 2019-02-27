#include "Exit.h"
#include "Framework/AssetManager.h"
#include "Level.h"

Exit::Exit()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/DoorClosed.png"));
	m_blocksMovement = true;
}

void Exit::Update(sf::Time _frameTime) 
{
	if (m_level->LevelComplete() == true) 
	{
	 m_sprite.setTexture(AssetManager::GetTexture("graphics/DoorOpen.png"));
	}
}

