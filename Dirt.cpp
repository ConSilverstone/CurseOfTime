// Project Includes
#include "Dirt.h"
#include "Framework/AssetManager.h"

Dirt::Dirt()
	: GridObject()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/storage.png"));
	m_blocksMovement = true;
}
