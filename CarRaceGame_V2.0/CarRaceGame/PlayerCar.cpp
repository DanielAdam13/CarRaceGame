#include "pch.h"
#include "PlayerCar.h"
#include "utils.h"

PlayerCar::PlayerCar(const Vector2f& pos, const float width, const float height, const Color4f& color, float screenWidth, float screenHeight)
	: Car::Car(pos, width, height),
	m_Color{ color },
	m_Speed{ 400.f },
	m_ScreenWidth{ screenWidth },
	m_ScreenHeight{ screenHeight }
{
}

void PlayerCar::Draw() const
{
	glPushMatrix();
		glTranslatef(GetBounds().left, GetBounds().bottom, 0);
		glRotatef(180.f, 0, 1, 0);

		utils::SetColor(this->m_Color);
		utils::FillRect( -m_Width , m_Height * 0.2f, m_Width, m_Height * 0.6f);
		utils::FillRect( m_Width * 0.25f - m_Width , 0.f, m_Width / 4, m_Height);
		utils::FillRect( m_Width * 0.65f - m_Width, 0.f, m_Width / 4, m_Height);

	glPopMatrix();

	utils::SetColor(Color4f{ 1,0,0,1 });
	utils::DrawRect(GetBounds());
}

void PlayerCar::Update(const Uint8* pStates, float elapsedSec)
{
	if (pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A])
	{
		m_Position.x -= m_Speed * elapsedSec;
	}
	else if (pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W])
	{
		m_Position.y += m_Speed * 0.7f * elapsedSec;
	}
	else if (pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D])
	{
		m_Position.x += m_Speed * elapsedSec;
	}
	else if (pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S])
	{
		m_Position.y -= m_Speed * 0.7f * elapsedSec;
	}

	SetBounds();
}

void PlayerCar::SetBounds()
{
	if (m_Position.x < m_Width / 2 + 1.f)
	{
		m_Position.x = m_Width / 2 + 1.f;
	}
	if (m_Position.x > m_ScreenWidth - m_Width / 2 - 1.f)
	{
		m_Position.x = m_ScreenWidth - m_Width / 2 - 1.f;
	}
	if (m_Position.y < 1.f)
	{
		m_Position.y = 1.f;
	}
	if (m_Position.y > m_ScreenHeight - m_Height - 1.f)
	{
		m_Position.y = m_ScreenHeight - m_Height - 1.f;
	}
}

