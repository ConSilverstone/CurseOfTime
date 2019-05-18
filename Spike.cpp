#include "Spike.h"
#include "Framework/AssetManager.h"

Spike::Spike()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/HighSpike.png"));
	m_blocksMovement = true;
}