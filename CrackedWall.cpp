#include "CrackedWall.h"
#include "Framework/AssetManager.h"

CrackedWall::CrackedWall()
{
	m_sprite.setTexture(AssetManager::GetTexture("graphics/CrackedWall.png"));
	m_blocksMovement = true;
}