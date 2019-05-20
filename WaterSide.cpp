#include "WaterSide.h"
#include "Framework/AssetManager.h"

WaterSide::WaterSide()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/WaterSide.png"));
	m_blocksMovement = true;
}