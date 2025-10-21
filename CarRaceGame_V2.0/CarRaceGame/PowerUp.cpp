#include "pch.h"
#include "PowerUp.h"
#include "PlayerCar.h"
#include "utils.h"
#include <iostream>
#include "Texture.h"
#include "Car.h"
#include "Lane.h"

PowerUp::PowerUp(int index, const Vector2f& pos, float size)
	:m_PowerUpIndex{ index },
	m_Position{ pos },
	m_Size{ size },
	m_Color{ 1,1,0,1 },
	m_Title{},
	m_CurrentState{ State::moving },
	m_DurAccSec{ 0.f },
	m_PowerUpDuration{},
	m_DeleteMarker{ false },
	m_CanHeal{ true }
{
	switch (index)
	{
	case 0: // invincibility
		m_Title = "INVINCIBLE";
		m_PowerUpDuration = 5.f;
		m_Color = Color4f{ 0.f, 0.8f, 0.9f, 1 };
		break;
	case 1: // small
		m_Title = "SMALL";
		m_PowerUpDuration = 7.f;
		break;
	case 2: // break cars
		m_Title = "LIFESTEAL";
		m_PowerUpDuration = 8.f;
		m_Color = Color4f{ 1,0,0,1 };
		break;
	case 3:
		m_Title = "HEALTH";
		m_PowerUpDuration = 1.f;
		m_Color = Color4f{ 0.8f, 0, 0.9f, 1 };
		break;
	case 4:
		m_Title = "STOPWATCH";
		m_PowerUpDuration = 4.f;
		m_Color = Color4f{ 1,1,1,1 };
	}

	//std::cout << m_Title + " powerup CREATED" << std::endl;
}

void PowerUp::MovePowerUp(float elapsedSec, float& parallaxSpeed)
{
	float currentSpeed{ 350.f * parallaxSpeed / 7 };

	m_Position.x -= currentSpeed * elapsedSec;
}

void PowerUp::AffectPlayer(float elapsedSec, PlayerCar& player, float& parallaxSpeed)
{
	m_DurAccSec += elapsedSec;

	if (m_PowerUpIndex == 3 && m_CanHeal)
	{
		player.IncreaseHealth(1);
		m_CanHeal = false;
	}
	
	if (m_DurAccSec <= m_PowerUpDuration)
	{
		switch (m_PowerUpIndex)
		{
		case 0: // invincibility
			player.SetInvincibility();
			break;
		case 1: // small
			player.SetSize(0.4f);
			break;
		case 2: // break cars
			parallaxSpeed -= 0.03f * elapsedSec;
			player.SetLifeSteal(true);
			break;
		case 4: // freeze cars
			player.CarsFrozen(true);
			Car::stopCars = true;
			Lane::freezeCarSpawn = true;
			break;
		}
	}
	else
	{
		if (m_PowerUpIndex == 1)
		{
			player.SetSize(1.f);
		}
		else if (m_PowerUpIndex == 2)
		{
			player.SetLifeSteal(false);
		}
		else if (m_PowerUpIndex == 3)
		{
			m_CanHeal = true;
		}
		else if (m_PowerUpIndex == 4)
		{
			player.CarsFrozen(false);
			Car::stopCars = false;
			Lane::freezeCarSpawn = false;
		}
		m_DeleteMarker = true;
	}
}

void PowerUp::Draw() const
{
	utils::SetColor(m_Color);
	utils::FillEllipse(m_Position, m_Size, m_Size);

	
	/*m_DisplayTexture->Draw(Rectf{ m_Position.x, m_Position.y - m_DisplayTexture->GetHeight() * m_ScaleIdx / 2, 
		m_DisplayTexture->GetWidth() * m_ScaleIdx, m_DisplayTexture->GetHeight() * m_ScaleIdx});*/
	

	// Hitbox
	//utils::SetColor(Color4f{ 1,0,0,1 });
	//utils::DrawRect(GetBounds());
}

Rectf PowerUp::GetBounds() const
{
	return Rectf(m_Position.x - m_Size / 2, m_Position.y - m_Size / 2, m_Size, m_Size);
}

std::string PowerUp::GetTitle() const
{
	return m_Title;
}

void PowerUp::AbsorbPowerUp()
{
	m_CurrentState = State::taken;
	
	//std::cout << m_Title + " pickup TAKEN" << std::endl;
}

PowerUp::State PowerUp::GetCurrentState() const
{
	return m_CurrentState;
}

bool PowerUp::ShouldDelete() const
{
	return m_DeleteMarker;
}
