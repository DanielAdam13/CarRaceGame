#pragma once
#include <vector>
class PowerUp;
class PlayerCar;
class Texture;
class SoundEffect;

class PowerUpManager
{
public:
	explicit PowerUpManager(const std::vector<Vector2f>& laneStartPosition, SoundEffect* pickup1, SoundEffect* pickup2);
	~PowerUpManager();

	PowerUpManager& operator=(PowerUpManager&& other) noexcept;

	void UpdatePowerUps(float elapsedSec, float& parallaxSpeed, PlayerCar& playerCar);
	void DrawPowerUps() const;

	void ClearPowerUps();

	static int pickedPowers;

private:
	std::vector<PowerUp*> m_PowerUpsVector;
	float m_SpawnAccuSec;
	float m_Cooldown;

	std::vector<Vector2f> m_StartPositions;
	bool m_CurrentFlickerStatus;

	SoundEffect* m_PickUpSFX1;
	SoundEffect* m_PickUpSFX2;
};

