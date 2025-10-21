#pragma once
#include "Car.h"
class Texture;
class PowerUp;
class SoundEffect;

class PlayerCar final : public Car
{
public:
	explicit PlayerCar(const Vector2f& pos, const float width, const float height, const Color4f& color, float screenWidth, float screenHeight, 
		const Texture* smallStatusText, const Texture* unbreakableStatusText, const Texture* invincibilityStatusText, const Texture* healText, const Texture* stopwatchText,
		SoundEffect* healSFX, SoundEffect* crashSFX);

	virtual void Draw(const Vector2f& pos) const override;
	virtual void Update(float elapsedSec, float& parallaxSpeed, const Uint8* pStates = nullptr, float bottomBorderY = 0.f, float topBorderY = 0.f);
	
	bool IsInvincible() const;
	void Hit();

	void SetSize(float index);
	void SetInvincibility();
	void SetLifeSteal(bool status);

	bool IsUnreakable() const;

	virtual Rectf GetHitbox() const override;
	virtual Rectf GetBounds() const override;

	int GetHealth() const;
	void IncreaseHealth(int index);

	bool AboutToEnd(const PowerUp* power);
	void SetFlickeringStatus(bool flicker);
	
	void RestartPlayer(const Vector2f& pos);

	void CarsFrozen(bool status);

	int TimesHit() const;

private:
	int m_Hp;
	enum class Movement {
		neutral,
		left,
		up,
		right,
		down
	};
	
	Color4f m_Color;
	float m_Speed;
	float m_Angle;
	bool m_Hit;
	bool m_TakeDamage;

	float m_InvincibilityTimer;
	bool m_Invicibility;

	float m_ScreenWidth;
	float m_ScreenHeight;

	float m_WheelSteerAngle;

	void SetBounds(float bottomBorderY, float topBorderY);

	float m_Size;
	bool m_PoweredUpInvincibility;

	void HandleInvincibility(float elapsedSec, float duration);

	bool m_LifeSteal;

	bool m_TookHeal;

	bool m_CarsFrozenFlag;

	const Texture* m_InvincibilityText;
	const Texture* m_SmallStatusText;
	const Texture* m_LifeStealStatusText;
	const Texture* m_HealText;
	const Texture* m_StopwatchText;

	void DrawPowerUpStatuses(const Vector2f& pos) const;

	bool m_DrawFlickeringStatus;

	void SetPosition(const Vector2f& pos);

	int m_TimesHit;

	SoundEffect* m_CrashSFX;
	SoundEffect* m_HealSFX;
};

