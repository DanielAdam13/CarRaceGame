#pragma once
class UIManager final
{
public:
	explicit UIManager();

	void Draw() const;
	void Update(float elapsedSec);

	int GetScore() const;

private:
	int m_Score;
	float m_AccuSec;
};

