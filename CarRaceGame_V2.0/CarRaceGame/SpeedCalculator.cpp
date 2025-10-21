#include "pch.h"
#include "SpeedCalculator.h"

SpeedCalculator::SpeedCalculator()
	:m_Score{ 0 },
	m_AccuSec{ 0.f }
{
}

void SpeedCalculator::Draw() const
{
}

void SpeedCalculator::Update(float elapsedSec)
{
	m_AccuSec += elapsedSec;

	if (m_AccuSec >= 0.02f)
	{
		m_Score+=1;
		m_AccuSec -= 0.02f;
	}
}

int SpeedCalculator::GetSpeed() const
{
	return m_Score;
}
