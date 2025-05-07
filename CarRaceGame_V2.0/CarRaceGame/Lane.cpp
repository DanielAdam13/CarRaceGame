#include "pch.h"
#include "Lane.h"
#include "Car.h"
#include "PlayerCar.h"
#include "utils.h"

Lane::Lane(const Vector2f& lanePosition, float carWidth, float carHeight, float minCarSpawnCooldown)
	:m_LanePosition{ lanePosition },
	m_Cars{},
	m_MinCarSpawnCooldown{ minCarSpawnCooldown },
	m_CarWidth{ carWidth },
	m_CarHeight{ carHeight },
	m_AccuSec{ 0.f },
	m_RandomSec{rand() % 1 + minCarSpawnCooldown}
{
	m_Cars.reserve(100);
}

Lane::~Lane()
{
	for (size_t i{}; i < m_Cars.size(); ++i)
	{
		delete m_Cars[i];
		m_Cars[i] = nullptr;
	}
	m_Cars.clear();
}

void Lane::Draw() const
{
	for (size_t i{}; i < m_Cars.size(); ++i)
	{
		if (m_Cars[i] != nullptr)
		{
			m_Cars[i]->Draw();
		}
	}
}

void Lane::HandleCars(float elapsedSec, PlayerCar* player, float& parallaxSpeed)
{
	m_AccuSec += elapsedSec;

	if (m_AccuSec > m_RandomSec)
	{
		int randOffset{ rand() % 400 + 100 };
		m_Cars.push_back(new Car(Vector2f{ m_LanePosition.x + randOffset, m_LanePosition.y - m_CarHeight / 2 }, m_CarWidth, m_CarHeight));

		m_AccuSec -= m_RandomSec;

		m_RandomSec = float((rand() % 15) / 10) + m_MinCarSpawnCooldown;

		if (parallaxSpeed <= 1.5f)
		{
			m_RandomSec += 2.f;
		}
	}

	for (size_t i{}; i < m_Cars.size(); ++i)
	{
		if (m_Cars[i] != nullptr)
		{
			if (!player->IsInvincible())
			{
				if (utils::IsOverlapping(m_Cars[i]->GetHitbox(), player->GetHitbox()))
				{
					player->Hit();
				}
			}

			if (m_Cars[i]->GetBounds().left < -m_Cars[i]->GetBounds().width)
			{
				delete m_Cars[i];
				m_Cars[i] = nullptr;
			}
			else
			{
				m_Cars[i]->Update(elapsedSec, parallaxSpeed);
			}
		}
	}
}

Vector2f Lane::GetDrawPosition() const
{
	return Vector2f(m_LanePosition);
}
