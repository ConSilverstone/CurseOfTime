#include "Spike.h"
#include "Framework/AssetManager.h"

Spike::Spike()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/DoorClosed.png"));
	m_blocksMovement = true;
}