#include "WaterDown.h"
#include "Framework/AssetManager.h"

WaterDown::WaterDown()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/WaterDown.png"));
	m_blocksMovement = true;
}