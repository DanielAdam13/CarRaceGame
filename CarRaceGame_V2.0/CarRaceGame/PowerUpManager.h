#pragma once
#include <vector>
class PowerUp;
class PlayerCar;

class PowerUpManager
{
public:
	explicit PowerUpManager();
	explicit PowerUpManager(const std::vector<Vector2f>& laneStartPositions);
	~PowerUpManager();

	PowerUpManager& operator=(PowerUpManager&& other) noexcept;

	void UpdatePowerUps(float elapsedSec, float& parallaxSpeed, PlayerCar& playerCar);
	void DrawPowerUps() const;

	void ClearPowerUps();

private:
	std::vector<PowerUp*> m_PowerUpsVector;
	float m_SpawnAccuSec;
	float m_Cooldown;

	std::vector<Vector2f> m_StartPositions;
	bool m_CurrentFlickerStatus;
};

