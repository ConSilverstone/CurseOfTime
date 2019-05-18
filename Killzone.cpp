#include "Killzone.h"
#include "Framework/AssetManager.h"

Killzone::Killzone()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/Killzone.png"));
	m_blocksMovement = true;
}