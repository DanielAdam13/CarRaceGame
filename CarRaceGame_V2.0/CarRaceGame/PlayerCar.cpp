#include "pch.h"
#include "PlayerCar.h"
#include "utils.h"
#include "Texture.h"
#include "PowerUp.h"

PlayerCar::PlayerCar(const Vector2f& pos, const float width, const float height, const Color4f& color, float screenWidth, float screenHeight,
	const Texture* smallStatusText, const Texture* unbreakableStatusText, const Texture* invincibilityStatusText, const Texture* healText)
	: Car::Car(pos, width, height),
	m_Hp{ 3 },
	m_Color{ color },
	m_Speed{ 600.f },
	m_Angle{ 180.f },
	m_Hit{ false },
	m_TakeDamage{ false },
	m_InvincibilityTimer{ 0.f },
	m_Invicibility{ false },
	m_WheelSteerAngle{ 0.f },
	m_ScreenWidth{ screenWidth },
	m_ScreenHeight{ screenHeight },
	m_Size{ 1.f },
	m_PoweredUpInvincibility{ false },
	m_LifeSteal{ false },
	m_TookHeal{ false },
	m_InvincibilityText{ invincibilityStatusText },
	m_SmallStatusText{ smallStatusText },
	m_LifeStealStatusText{ unbreakableStatusText },
	m_HealText{ healText },
	m_DrawFlickeringStatus{ true }
{
}

void PlayerCar::Draw(const Vector2f& pos) const
{
	if (m_Hp > 0)
	{
		glPushMatrix();
		// Move car to its world position

		glTranslatef(m_Position.x, m_Position.y + m_Height / 2, 0);
		glRotatef(m_Angle, 0, 0, 1); // Car's orientation
		glTranslatef(m_Width / 2, m_Height / 2, 0);
		glScalef(m_Size, m_Size, 0);


		// --- Draw Car Body ---
		utils::SetColor(this->m_Color);
		utils::FillRect(-m_Width, -m_Height * 0.8f, m_Width, m_Height * 0.6f);

		// Rear Left Wheel
		glPushMatrix();
		glTranslatef(m_Width * 0.25f - m_Width + m_Width / 8, -m_Height + m_Height / 8, 0);
		glRotatef(m_WheelSteerAngle, 0, 0, 1);
		utils::FillRect(-m_Width / 8, -m_Height / 8, m_Width / 4, m_Height / 3);
		glPopMatrix();

		// Rear Right Wheel
		glPushMatrix();
		glTranslatef(m_Width * 0.65f - m_Width + m_Width / 8, -m_Height + m_Height / 8, 0);
		glRotatef(m_WheelSteerAngle, 0, 0, 1);
		utils::FillRect(-m_Width / 8, -m_Height / 8, m_Width / 4, m_Height / 3);
		glPopMatrix();

		// --- Draw Front Wheels (with steering) ---
		// Front Left Wheel
		glPushMatrix();
		glTranslatef(m_Width * 0.25f - m_Width + m_Width / 8, -m_Height * 0.25f + m_Height / 12, 0);
		glRotatef(m_WheelSteerAngle, 0, 0, 1); // Apply steering
		utils::FillRect(-m_Width / 8, -m_Height / 8, m_Width / 4, m_Height / 3);
		glPopMatrix();

		// Front Right Wheel
		glPushMatrix();
		glTranslatef(m_Width * 0.65f - m_Width + m_Width / 8, -m_Height * 0.25f + m_Height / 12, 0);
		glRotatef(m_WheelSteerAngle, 0, 0, 1); // Apply steering
		utils::FillRect(-m_Width / 8, -m_Height / 8, m_Width / 4, m_Height / 3);
		glPopMatrix();

		glPopMatrix();

		DrawPowerUpStatuses(pos);

		// Hitbox
		/*utils::SetColor(Color4f{ 1,0,0,1 });
		utils::DrawRect(GetBounds());
		utils::DrawRect(GetHitbox());
		utils::FillEllipse(m_Position, 5.f, 5.f);*/
	}
}

void PlayerCar::Update(float elapsedSec, float& parallaxSpeed, const Uint8* pStates, float bottomBorderY, float topBorderY)
{
	if (m_Hp > 0)
	{
		bool keyPressed{ false };

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

		if (m_Invicibility && !m_PoweredUpInvincibility)
		{
			m_InvincibilityTimer += elapsedSec;

			if (m_InvincibilityTimer >= 3.f)
			{
				m_Invicibility = false;
				m_InvincibilityTimer -= 3.f;
				m_TakeDamage = false;
			}
			parallaxSpeed += 0.3f * elapsedSec;
		}


		if ((pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A]) && (pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W]))
		{
			m_WheelSteerAngle = 30.f;
			m_Position += Vector2f{ -this->m_Speed / 2, this->m_Speed / 2 } *elapsedSec;
			keyPressed = true;
		}
		else if ((pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A]) && (pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S]))
		{
			m_WheelSteerAngle = -30.f;
			m_Position += Vector2f{ -this->m_Speed / 2, -this->m_Speed / 2 } *elapsedSec;
			keyPressed = true;
		}
		else if ((pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D]) && (pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W]))
		{
			m_WheelSteerAngle = 30.f;
			m_Position += Vector2f{ this->m_Speed / 2, this->m_Speed / 2 } *elapsedSec;
			keyPressed = true;
		}
		else if ((pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D]) && (pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S]))
		{
			m_WheelSteerAngle = -30.f;
			m_Position += Vector2f{ this->m_Speed / 2, -this->m_Speed / 2 } *elapsedSec;
			keyPressed = true;
		}
		else if (pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A])
		{
			m_WheelSteerAngle = 0.f;
			m_Position.x -= m_Speed * elapsedSec;
			keyPressed = true;
		}
		else if (pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W])
		{
			m_WheelSteerAngle = 30.f;
			m_Position.y += m_Speed * 0.8f * elapsedSec;
			keyPressed = true;
		}
		else if (pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D])
		{
			m_WheelSteerAngle = 0.f;
			m_Position.x += m_Speed * elapsedSec;
			keyPressed = true;
		}
		else if (pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S])
		{
			m_WheelSteerAngle = -30.f;
			m_Position.y -= m_Speed * 0.8f * elapsedSec;
			keyPressed = true;
		}


		SetBounds(bottomBorderY, topBorderY);

		static float wheelAcc{ 0.f };
		if (!keyPressed)
		{
			wheelAcc += elapsedSec;
			if (wheelAcc >= 0.5f)
			{
				m_WheelSteerAngle = 0.f;
				wheelAcc -= 0.5f;
			}
		}

		HandleInvincibility(elapsedSec, 6.f);

		static float healAcc{ 0.f };
		if (m_TookHeal)
		{
			healAcc += elapsedSec;
			if (healAcc >= 2.f)
			{
				healAcc -= 2.f;
				m_TookHeal = false;
			}
		}
	}
}

bool PlayerCar::IsInvincible() const
{
	return m_Invicibility || m_PoweredUpInvincibility;
}

void PlayerCar::Hit()
{
	if (!m_TakeDamage && !m_PoweredUpInvincibility)
	{
		m_Hp--;
		m_TakeDamage = true;
	}
	
	m_Hit = true;
}

void PlayerCar::SetSize(float index)
{
	m_Size = index;
}

void PlayerCar::SetInvincibility()
{
	m_PoweredUpInvincibility = true;
	m_InvincibilityTimer = 0.f;
}

void PlayerCar::SetUnbreakable(bool status)
{
	m_LifeSteal = status;
}

bool PlayerCar::IsUnreakable() const
{
	return m_LifeSteal;
}

Rectf PlayerCar::GetHitbox() const
{
	return Rectf(m_Position.x - m_Width / 3, m_Position.y, m_Width * m_Size * 0.45f, m_Height * m_Size);
}

Rectf PlayerCar::GetBounds() const
{
	return Rectf(m_Position.x - m_Width / 2, m_Position.y, m_Width * m_Size, m_Height * m_Size);
}

int PlayerCar::GetHealth() const
{
	return m_Hp;
}

void PlayerCar::IncreaseHealth(int index)
{
	m_Hp += index;
	m_TookHeal = true;
}

bool PlayerCar::AboutToEnd(const PowerUp* power)
{
	return power->m_DurAccSec >= power->m_PowerUpDuration - 2.f;
}

void PlayerCar::SetFlickeringStatus(bool flicker)
{
	m_DrawFlickeringStatus = flicker;
}

void PlayerCar::RestartPlayer(const Vector2f& pos)
{
	m_Hp = 3;
	SetPosition(pos);
}

void PlayerCar::SetPosition(const Vector2f& pos)
{
	m_Position = pos;
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

void PlayerCar::HandleInvincibility(float elapsedSec, float duration)
{
	if (m_PoweredUpInvincibility)
	{
		m_Invicibility = true;
		static float accSec{ 0.f };
		accSec += elapsedSec;

		if (accSec >= duration)
		{
			m_PoweredUpInvincibility = false;
			accSec -= duration;
			m_Invicibility = false;
		}
	}
}

void PlayerCar::DrawPowerUpStatuses(const Vector2f& pos) const
{
	if (m_DrawFlickeringStatus)
	{
		if (m_LifeSteal)
		{
			m_LifeStealStatusText->Draw(pos);
			utils::SetColor(Color4f{ 1,0,1,1 });
			utils::DrawRect(Vector2f{m_Position.x - m_Width / 2 - 5.f, m_Position.y - 5.f}, m_Width + 10.f, m_Height + 10.f, 3);
		}
		else if (m_Size < 1.f)
		{
			m_SmallStatusText->Draw(pos);
		}
		else if (m_TookHeal)
		{
			m_HealText->Draw(pos);
		}
		else if (m_Invicibility)
		{
			m_InvincibilityText->Draw(pos);
		}
	}
	
}

