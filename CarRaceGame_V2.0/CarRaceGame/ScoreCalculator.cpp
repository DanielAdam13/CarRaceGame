#include "pch.h"
#include "ScoreCalculator.h"

ScoreCalculator::ScoreCalculator()
	:m_Score{ 0 },
	m_AccuSec{ 0.f }
{
}

void ScoreCalculator::Draw() const
{
}

void ScoreCalculator::Update(float elapsedSec)
{
	m_AccuSec += elapsedSec;

	if (m_AccuSec >= 0.02f)
	{
		m_Score+=1;
		m_AccuSec -= 0.02f;
	}
}

int ScoreCalculator::GetScore() const
{
	return m_Score;
}
