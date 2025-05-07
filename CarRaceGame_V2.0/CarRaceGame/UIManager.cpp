#include "pch.h"
#include "UIManager.h"

UIManager::UIManager()
	:m_Score{ 0 },
	m_AccuSec{ 0.f }
{
}

void UIManager::Draw() const
{
}

void UIManager::Update(float elapsedSec)
{
	m_AccuSec += elapsedSec;

	if (m_AccuSec >= 0.02f)
	{
		m_Score+=1;
		m_AccuSec -= 0.02f;
	}
}

int UIManager::GetScore() const
{
	return m_Score;
}
