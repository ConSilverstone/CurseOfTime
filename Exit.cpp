#include "Exit.h"
#include "Framework/AssetManager.h"
#include "Level.h"

Exit::Exit()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/ExitPortal.png"));
	m_blocksMovement = true;
}

void Exit::Update(sf::Time _frameTime) 
{

}

