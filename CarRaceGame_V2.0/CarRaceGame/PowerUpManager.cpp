#include "pch.h"
#include "PowerUpManager.h"
#include "PowerUp.h"
#include "utils.h"
#include "PlayerCar.h"

PowerUpManager::PowerUpManager()
	:PowerUpManager::PowerUpManager({})
{
}

PowerUpManager::PowerUpManager(const std::vector<Vector2f>& laneStartPositions)
	:m_PowerUpsVector{},
	m_SpawnAccuSec{ 0.f },
	m_Cooldown{ 15.f },
	m_StartPositions{ laneStartPositions },
	m_CurrentFlickerStatus{ true }
{
	m_StartPositions.reserve(5);
	m_PowerUpsVector.reserve(10);
}

PowerUpManager::~PowerUpManager()
{
	for (size_t i{}; i < m_PowerUpsVector.size(); ++i)
	{
		if (m_PowerUpsVector[i] != nullptr)
		{
			delete m_PowerUpsVector[i];
			m_PowerUpsVector[i] = nullptr;
		}
	}
}

PowerUpManager& PowerUpManager::operator=(PowerUpManager&& other) noexcept
{
	for (size_t i{}; i < other.m_StartPositions.size(); ++i)
	{
		m_StartPositions.push_back(std::move(other.m_StartPositions[i]));
	}
	for (size_t j{}; j < other.m_PowerUpsVector.size(); ++j)
	{
		delete m_PowerUpsVector[j];
		m_PowerUpsVector[j] = std::move(other.m_PowerUpsVector[j]);
	}

	m_SpawnAccuSec = std::move(other.m_SpawnAccuSec);
	m_Cooldown = std::move(other.m_Cooldown);
	
	return *this;
}

void PowerUpManager::UpdatePowerUps(float elapsedSec, float& parallaxSpeed, PlayerCar& playerCar)
{
	m_SpawnAccuSec += elapsedSec;

	if (m_SpawnAccuSec >= m_Cooldown)
	{
		const int randNr{ rand() % 4 };
		
		m_PowerUpsVector.push_back(new PowerUp(randNr, m_StartPositions[randNr + 1], 20.f));
		m_SpawnAccuSec -= m_Cooldown;
	}

	for (size_t i{}; i < m_PowerUpsVector.size(); ++i)
	{
		if (m_PowerUpsVector[i] != nullptr)
		{
			if (!m_PowerUpsVector[i]->ShouldDelete())
			{
				if (utils::IsOverlapping(playerCar.GetBounds(), m_PowerUpsVector[i]->GetBounds()))
				{
					m_PowerUpsVector[i]->AbsorbPowerUp();
				}

				if (m_PowerUpsVector[i]->GetCurrentState() == PowerUp::State::taken)
				{
					m_PowerUpsVector[i]->AffectPlayer(elapsedSec, playerCar, parallaxSpeed);

					if (playerCar.AboutToEnd(m_PowerUpsVector[i]) && m_PowerUpsVector[i]->GetTitle() != "HEALTH")
					{
						static float flickerAcc{ 0.f };
						flickerAcc += elapsedSec;

						if (flickerAcc >= 0.2f)
						{
							m_CurrentFlickerStatus = !m_CurrentFlickerStatus;
							flickerAcc -= 0.2f;
						}
						playerCar.SetFlickeringStatus(m_CurrentFlickerStatus);
					}
				}
				else
				{
					if (m_PowerUpsVector[i]->GetBounds().left > -m_PowerUpsVector[i]->GetBounds().width)
					{
						m_PowerUpsVector[i]->MovePowerUp(elapsedSec, parallaxSpeed);
					}
					else
					{
						delete m_PowerUpsVector[i];
						m_PowerUpsVector[i] = nullptr;
					}
				}
			}
			else
			{
				delete m_PowerUpsVector[i];
				m_PowerUpsVector[i] = nullptr;
			}
		}
	}
}

void PowerUpManager::DrawPowerUps() const
{
	for (PowerUp* power : m_PowerUpsVector)
	{
		if (power != nullptr)
		{
			if (power->GetCurrentState() == PowerUp::State::moving)
			{
				power->Draw();
			}
		}
	}
}

void PowerUpManager::ClearPowerUps()
{
	for (size_t i{}; i < m_PowerUpsVector.size(); ++i)
	{
		delete m_PowerUpsVector[i];
		m_PowerUpsVector[i] = nullptr;
	}
}
