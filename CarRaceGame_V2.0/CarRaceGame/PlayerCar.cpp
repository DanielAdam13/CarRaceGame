#include "pch.h"
#include "PlayerCar.h"
#include "utils.h"

PlayerCar::PlayerCar(const Vector2f& pos, const float width, const float height, const Color4f& color, float screenWidth, float screenHeight)
	: Car::Car(pos, width, height),
	m_Color{ color },
	m_Speed{ 600.f },
	m_Angle{ 180.f },
	m_Hit{ false },
	m_InvincibilityTimer{ 0.f },
	m_Invicibility{ false },
	m_ScreenWidth{ screenWidth },
	m_ScreenHeight{ screenHeight }
{
}

void PlayerCar::Draw() const
{
	glPushMatrix();
		glTranslatef(m_Position.x, m_Position.y + m_Height / 2, 0);
		glRotatef(m_Angle, 0, 0, 1);
		glTranslatef(m_Width / 2, m_Height / 2, 0);

		utils::SetColor(this->m_Color);
		utils::FillRect( -m_Width , -m_Height * 0.8f, m_Width, m_Height * 0.6f);
		utils::FillRect( m_Width * 0.25f - m_Width, -m_Height, m_Width / 4, m_Height);
		utils::FillRect( m_Width * 0.65f - m_Width, -m_Height, m_Width / 4, m_Height);

	glPopMatrix();

	// Hitbox
	//utils::SetColor(Color4f{ 1,0,0,1 });
	//utils::DrawRect(GetBounds());
	//utils::DrawRect(GetHitbox());
	//utils::FillEllipse(m_Position, 5.f, 5.f);
}

void PlayerCar::Update(float elapsedSec, float& parallaxSpeed, const Uint8* pStates, float bottomBorderY, float topBorderY)
{
	if (m_Hit)
	{
		static float m_AccuSec{ 0.f };
		m_AccuSec += elapsedSec;
		m_Angle += 3.f;
		m_Speed = 0.f;
		parallaxSpeed -= 1.5f * elapsedSec;

		if (parallaxSpeed <= 0.f)
		{
			parallaxSpeed = 0.f;
		}

		if (m_AccuSec >= 1.5f)
		{
			m_AccuSec -= 1.5f;
			m_Hit = false;
			m_Angle = 180.f;
			m_Speed = 600.f;
			m_Invicibility = true;
		}
	}

	if (m_Invicibility)
	{
		m_InvincibilityTimer += elapsedSec;

		if (m_InvincibilityTimer >= 5.f)
		{
			m_Invicibility = false;
			m_InvincibilityTimer -= 5.f;
		}
		parallaxSpeed += 0.3f * elapsedSec;
	}


	if ((pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A]) && (pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W]))
	{
		m_Position += Vector2f{ -this->m_Speed / 2, this->m_Speed / 2 } *elapsedSec;
	}
	else if ((pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A]) && (pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S]))
	{
		m_Position += Vector2f{ -this->m_Speed / 2, -this->m_Speed / 2 } *elapsedSec;
	}
	else if ((pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D]) && (pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W]))
	{
		m_Position += Vector2f{ this->m_Speed / 2, this->m_Speed / 2 } *elapsedSec;
	}
	else if ((pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D]) && (pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S]))
	{
		m_Position += Vector2f{ this->m_Speed / 2, -this->m_Speed / 2 } *elapsedSec;
	}
	else if (pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A])
	{
		m_Position.x -= m_Speed * elapsedSec;
	}
	else if (pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W])
	{
		m_Position.y += m_Speed * 0.8f * elapsedSec;
	}
	else if (pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D])
	{
		m_Position.x += m_Speed * elapsedSec;
	}
	else if (pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S])
	{
		m_Position.y -= m_Speed * 0.8f * elapsedSec;
	}

	SetBounds(bottomBorderY, topBorderY);
}

bool PlayerCar::IsInvincible() const
{
	return m_Invicibility;
}

void PlayerCar::Hit()
{
	m_Hit = true;
}

Rectf PlayerCar::GetHitbox() const
{
	return Rectf(m_Position.x - m_Width / 3, m_Position.y + m_Height / 20, m_Width * 0.45f, m_Height * 0.9f);
}

void PlayerCar::SetBounds(float bottomBorderY, float topBorderY)
{
	if (m_Position.x < m_Width / 2 + 1.f)
	{
		m_Position.x = m_Width / 2 + 1.f;
	}
	if (m_Position.x > m_ScreenWidth - m_Width / 2 - 1.f)
	{
		m_Position.x = m_ScreenWidth - m_Width / 2 - 1.f;
	}
	if (m_Position.y < bottomBorderY)
	{
		m_Position.y = bottomBorderY;
	}
	if (m_Position.y > topBorderY - m_Height)
	{
		m_Position.y = topBorderY - m_Height;
	}
}

