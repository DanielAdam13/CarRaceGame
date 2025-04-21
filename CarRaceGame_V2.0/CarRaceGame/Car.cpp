#include "pch.h"
#include "Car.h"
#include "utils.h"

Car::Car(const Vector2f& pos, const float width, const float height)
	:m_Position{ pos },
	m_Speed{},
	m_Color{},
	m_Width{ width },
	m_Height{ height }
{
	int randNr{ rand() % 5 };
	switch (randNr)
	{
	case 0:
		m_Color = Color4f{ 0.95f, 0.3f, 0.95f, 1 };
		m_Speed = 250.f;
		break;
	case 1:
		m_Color = Color4f{ 0.7f, 0.7f, 0.7f, 1 };
		m_Speed = 175.f;
		break;
	case 2:
		m_Color = Color4f{ 0.3f, 0.85f, 1, 1 };
		m_Speed = 200.f;
		break;
	case 3:
		m_Color = Color4f{ 1, 0.7f, 0.3f, 1 };
		m_Speed = 350.f;
		break;
	case 4:
		m_Color = Color4f{ 0.55f, 1.f, 0.3f, 1 };
		m_Speed = 300.f;
		break;
	}
}

void Car::Draw() const
{
	utils::SetColor(m_Color);
	utils::FillRect(m_Position.x - m_Width / 2, m_Position.y + m_Height * 0.2f, m_Width, m_Height * 0.6f);
	utils::FillRect(m_Position.x + m_Width * 0.25f - m_Width / 2, m_Position.y , m_Width / 4, m_Height);
	utils::FillRect(m_Position.x + m_Width * 0.65f - m_Width / 2, m_Position.y , m_Width / 4, m_Height);

	utils::SetColor(Color4f{ 1,0,0,1 });
	utils::DrawRect(GetBounds());
}

void Car::Update(float elapsedSec)
{
	m_Position.x -= m_Speed * elapsedSec;
}

Rectf Car::GetBounds() const
{
	return Rectf{ m_Position.x - m_Width / 2, m_Position.y, m_Width, m_Height };
}
